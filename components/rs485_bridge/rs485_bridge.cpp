#include "rs485_bridge.h"
#include "rs485_page.h"

#ifdef USE_ESP32
#if defined(USE_ESP_IDF)

#include "esphome/core/log.h"
#include "esphome/core/application.h"

#include <cstdio>
#include <cstring>
#include <string>

namespace esphome {
namespace rs485_bridge {

static const char *const TAG = "rs485_bridge";

// ---------------------------------------------------------------------------
// UART (RS485 half-duplex)
// ---------------------------------------------------------------------------

bool Rs485Bridge::write_bytes_val(const uint8_t *data, size_t len) {
  if (len == 0)
    return true;
  if (this->server_ == nullptr)
    return false;
  int written = uart_write_bytes(this->uart_num_, data, len);
  if (written < 0) {
    ESP_LOGW(TAG, "UART write failed: %d", written);
    return false;
  }
  this->tx_total_ += written;
  return true;
}

void Rs485Bridge::push_rx(const uint8_t *data, size_t len) {
  if (len == 0)
    return;
  if (xSemaphoreTake(this->rx_mutex_, portMAX_DELAY) != pdTRUE)
    return;
  if (this->rx_ring_.size() + len > this->rx_hold_) {
    size_t drop = this->rx_ring_.size() + len - this->rx_hold_;
    if (drop >= this->rx_ring_.size()) {
      this->rx_ring_.clear();
    } else {
      this->rx_ring_.erase(this->rx_ring_.begin(), this->rx_ring_.begin() + drop);
    }
  }
  this->rx_ring_.insert(this->rx_ring_.end(), data, data + len);
  this->rx_waiting_ = this->rx_ring_.size();
  xSemaphoreGive(this->rx_mutex_);
}

// ---------------------------------------------------------------------------
// WebSocket client bookkeeping
// ---------------------------------------------------------------------------

void Rs485Bridge::ws_add_client(int fd) {
  if (fd < 0)
    return;
  if (xSemaphoreTake(this->ws_mutex_, portMAX_DELAY) != pdTRUE)
    return;
  for (int cfd : this->ws_clients_) {
    if (cfd == fd) {
      xSemaphoreGive(this->ws_mutex_);
      return;
    }
  }
  this->ws_clients_.push_back(fd);
  ESP_LOGD(TAG, "WebSocket client %d connected (%u total)", fd,
           static_cast<unsigned>(this->ws_clients_.size()));
  xSemaphoreGive(this->ws_mutex_);
}

void Rs485Bridge::ws_remove_client(int fd) {
  if (xSemaphoreTake(this->ws_mutex_, portMAX_DELAY) != pdTRUE)
    return;
  for (auto it = this->ws_clients_.begin(); it != this->ws_clients_.end(); ++it) {
    if (*it == fd) {
      this->ws_clients_.erase(it);
      ESP_LOGD(TAG, "WebSocket client %d disconnected (%u total)", fd,
               static_cast<unsigned>(this->ws_clients_.size()));
      break;
    }
  }
  xSemaphoreGive(this->ws_mutex_);
}

void Rs485Bridge::ws_broadcast(const uint8_t *data, size_t len) {
  if (this->server_ == nullptr || len == 0)
    return;
  if (xSemaphoreTake(this->ws_mutex_, portMAX_DELAY) != pdTRUE)
    return;
  httpd_ws_frame_t frame = {};
  frame.type = HTTPD_WS_TYPE_BINARY;
  frame.payload = (uint8_t *) data;
  frame.len = len;
  auto it = this->ws_clients_.begin();
  while (it != this->ws_clients_.end()) {
    esp_err_t ret = httpd_ws_send_frame_async(this->server_, *it, &frame);
    if (ret != ESP_OK) {
      ESP_LOGD(TAG, "WS send to %d failed (%s), dropping client", *it, esp_err_to_name(ret));
      it = this->ws_clients_.erase(it);
    } else {
      ++it;
    }
  }
  xSemaphoreGive(this->ws_mutex_);
}

void Rs485Bridge::ws_ping_all() {
  if (this->server_ == nullptr)
    return;
  if (xSemaphoreTake(this->ws_mutex_, portMAX_DELAY) != pdTRUE)
    return;
  if (this->ws_clients_.empty()) {
    xSemaphoreGive(this->ws_mutex_);
    return;
  }
  // Sending a ping lets us detect dead peers even when no RX data flows.
  auto it = this->ws_clients_.begin();
  while (it != this->ws_clients_.end()) {
    httpd_ws_frame_t frame = {};
    frame.type = HTTPD_WS_TYPE_PING;
    frame.payload = nullptr;
    frame.len = 0;
    esp_err_t ret = httpd_ws_send_frame_async(this->server_, *it, &frame);
    if (ret != ESP_OK) {
      it = this->ws_clients_.erase(it);
    } else {
      ++it;
    }
  }
  xSemaphoreGive(this->ws_mutex_);
}

// ---------------------------------------------------------------------------
// esp_http_server handlers
// ---------------------------------------------------------------------------

void Rs485Bridge::register_uri(const char *uri, httpd_method_t method,
                               esp_err_t (*handler)(httpd_req_t *), bool is_websocket) {
  httpd_uri_t hd_uri;
  memset(&hd_uri, 0, sizeof(hd_uri));
  hd_uri.uri = uri;
  hd_uri.method = method;
  hd_uri.handler = handler;
  hd_uri.user_ctx = this;
  hd_uri.is_websocket = is_websocket;
  // handle_ws_control_frames stays at its memset default (false): esp_http_server
  // auto-answers PING/CLOSE control frames, so only data frames reach our handler.
  esp_err_t ret = httpd_register_uri_handler(this->server_, &hd_uri);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to register URI %s: %s", uri, esp_err_to_name(ret));
  } else {
    ESP_LOGD(TAG, "Registered URI %s", uri);
  }
}

esp_err_t Rs485Bridge::handle_page(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, kPageHtml, HTTPD_RESP_USE_STRLEN);
}

esp_err_t Rs485Bridge::handle_ws(httpd_req_t *req) {
  auto *self = reinterpret_cast<Rs485Bridge *>(req->user_ctx);
  if (req->method == HTTP_GET) {
    // WebSocket handshake: register the new client fd.
    self->ws_add_client(httpd_req_to_sockfd(req));
    return ESP_OK;
  }

  httpd_ws_frame_t frame = {};
  esp_err_t ret = httpd_ws_recv_frame(req, &frame, 0);
  if (ret != ESP_OK) {
    ESP_LOGD(TAG, "WS recv header failed: %s", esp_err_to_name(ret));
    self->ws_remove_client(httpd_req_to_sockfd(req));
    return ret;
  }
  if (frame.len == 0)
    return ESP_OK;

  // With handle_ws_control_frames = false, PING/CLOSE are auto-answered by the
  // server, so only BINARY/TEXT data frames which we must pass to the bus reach here.
  std::vector<uint8_t> payload(frame.len);
  frame.payload = payload.data();
  ret = httpd_ws_recv_frame(req, &frame, frame.len);
  if (ret != ESP_OK) {
    ESP_LOGD(TAG, "WS recv payload failed: %s", esp_err_to_name(ret));
    self->ws_remove_client(httpd_req_to_sockfd(req));
    return ret;
  }
  if (frame.len > 0) {
    ESP_LOGD(TAG, "WS -> UART: %u bytes", static_cast<unsigned>(frame.len));
    self->write_bytes_val(payload.data(), frame.len);
  }
  return ESP_OK;
}

esp_err_t Rs485Bridge::handle_status(httpd_req_t *req) {
  auto *self = reinterpret_cast<Rs485Bridge *>(req->user_ctx);
  unsigned ws_count = 0;
  if (xSemaphoreTake(self->ws_mutex_, portMAX_DELAY) == pdTRUE) {
    ws_count = static_cast<unsigned>(self->ws_clients_.size());
    xSemaphoreGive(self->ws_mutex_);
  }
  uint32_t rx_waiting = 0;
  if (xSemaphoreTake(self->rx_mutex_, portMAX_DELAY) == pdTRUE) {
    rx_waiting = static_cast<uint32_t>(self->rx_ring_.size());
    xSemaphoreGive(self->rx_mutex_);
  }

  char body[512];
  snprintf(body, sizeof(body),
           "{\"baud_rate\":%u,\"tx_pin\":%u,\"rx_pin\":%u,\"dir_pin\":%u,"
           "\"data_bits\":%u,\"stop_bits\":%u,\"parity\":%u,\"port\":%u,"
           "\"rx_waiting\":%u,\"ws_clients\":%u,\"tx_total\":%u,\"rx_total\":%u}",
           static_cast<unsigned>(self->baud_rate_),
           static_cast<unsigned>(self->tx_pin_), static_cast<unsigned>(self->rx_pin_),
           static_cast<unsigned>(self->dir_pin_), static_cast<unsigned>(self->data_bits_),
           static_cast<unsigned>(self->stop_bits_), static_cast<unsigned>(self->parity_),
           static_cast<unsigned>(self->port_), rx_waiting, ws_count,
           static_cast<unsigned>(self->tx_total_), static_cast<unsigned>(self->rx_total_));
  httpd_resp_set_type(req, "application/json");
  return httpd_resp_send(req, body, HTTPD_RESP_USE_STRLEN);
}

esp_err_t Rs485Bridge::handle_tx(httpd_req_t *req) {
  auto *self = reinterpret_cast<Rs485Bridge *>(req->user_ctx);
  int total = req->content_len;
  if (total <= 0) {
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, "{\"written\":0,\"ok\":true}", HTTPD_RESP_USE_STRLEN);
  }
  constexpr int kMaxTx = 1024;
  if (total > kMaxTx) {
    httpd_resp_send_err(req, HTTPD_413_CONTENT_TOO_LARGE, "max 1024 bytes");
    return ESP_FAIL;
  }
  uint8_t payload[kMaxTx];
  int received = 0;
  while (received < total) {
    int r = httpd_req_recv(req, (char *) payload + received, total - received);
    if (r < 0) {
      ESP_LOGW(TAG, "POST /rs485/tx recv error: %d", r);
      return r;
    }
    if (r == 0)
      break;
    received += r;
  }
  bool ok = self->write_bytes_val(payload, received);
  char body[64];
  snprintf(body, sizeof(body), "{\"written\":%d,\"ok\":%s}", received, ok ? "true" : "false");
  httpd_resp_set_type(req, "application/json");
  return httpd_resp_send(req, body, HTTPD_RESP_USE_STRLEN);
}

esp_err_t Rs485Bridge::handle_rx(httpd_req_t *req) {
  auto *self = reinterpret_cast<Rs485Bridge *>(req->user_ctx);

  bool clear = true;
  bool raw = false;
  size_t qlen = httpd_req_get_url_query_len(req);
  if (qlen > 0) {
    std::string query;
    query.resize(qlen + 1);
    if (httpd_req_get_url_query_str(req, &query[0], query.size()) == ESP_OK) {
      if (strstr(query.c_str(), "clear=0") != nullptr)
        clear = false;
      if (strstr(query.c_str(), "raw=1") != nullptr)
        raw = true;
    }
  }

  std::vector<uint8_t> snapshot;
  if (xSemaphoreTake(self->rx_mutex_, portMAX_DELAY) == pdTRUE) {
    snapshot = self->rx_ring_;
    if (clear)
      self->rx_ring_.clear();
    self->rx_waiting_ = self->rx_ring_.size();
    xSemaphoreGive(self->rx_mutex_);
  }

  if (raw) {
    httpd_resp_set_type(req, "application/octet-stream");
    if (snapshot.empty())
      return httpd_resp_send(req, nullptr, 0);
    return httpd_resp_send(req, (const char *) snapshot.data(), snapshot.size());
  }

  static const char kHex[] = "0123456789abcdef";
  std::string hex;
  hex.reserve(snapshot.size() * 2 + 32);
  for (uint8_t b : snapshot) {
    hex.push_back(kHex[(b >> 4) & 0x0f]);
    hex.push_back(kHex[b & 0x0f]);
  }
  std::string body = "{\"n\":" + std::to_string(snapshot.size()) + ",\"hex\":\"" + hex + "\"}";
  httpd_resp_set_type(req, "application/json");
  return httpd_resp_send(req, body.c_str(), HTTPD_RESP_USE_STRLEN);
}

// ---------------------------------------------------------------------------
// setup / loop
// ---------------------------------------------------------------------------

void Rs485Bridge::setup() {
  ESP_LOGI(TAG, "Setting up RS485 bridge (TX=%d RX=%d DIR=%d baud=%u ch=%d)",
           this->tx_pin_, this->rx_pin_, this->dir_pin_,
           static_cast<unsigned>(this->baud_rate_), static_cast<int>(this->uart_num_));

  // --- UART in native RS485 half-duplex mode (SIT3088 DE/RE driven by RTS) ---
  uart_config_t uart_config = {};
  uart_config.baud_rate = this->baud_rate_;
  uart_config.data_bits = this->data_bits_;
  uart_config.parity = this->parity_;
  uart_config.stop_bits = this->stop_bits_;
  uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
  uart_config.rx_flow_ctrl_thresh = 0;
  uart_config.source_clk = UART_SCLK_DEFAULT;

  esp_err_t ret = uart_param_config(this->uart_num_, &uart_config);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "uart_param_config failed: %s", esp_err_to_name(ret));
    this->mark_failed();
    return;
  }
  ret = uart_set_pin(this->uart_num_, this->tx_pin_, this->rx_pin_, this->dir_pin_,
                     UART_PIN_NO_CHANGE);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "uart_set_pin failed: %s", esp_err_to_name(ret));
    this->mark_failed();
    return;
  }
  ret = uart_driver_install(this->uart_num_, this->rx_buffer_, this->tx_buffer_, 0, nullptr, 0);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "uart_driver_install failed: %s", esp_err_to_name(ret));
    this->mark_failed();
    return;
  }
  ret = uart_set_mode(this->uart_num_, UART_MODE_RS485_HALF_DUPLEX);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "uart_set_mode failed: %s", esp_err_to_name(ret));
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "RS485 UART configured in half-duplex mode");

  // --- HTTP server ---
  this->ws_mutex_ = xSemaphoreCreateMutex();
  this->rx_mutex_ = xSemaphoreCreateMutex();

  httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG();
  httpd_config.server_port = this->port_;
  httpd_config.stack_size = 8192;
  ret = httpd_start(&this->server_, &httpd_config);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "httpd_start failed: %s", esp_err_to_name(ret));
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "HTTP server started on port %u", static_cast<unsigned>(this->port_));

  this->register_uri("/rs485", HTTP_GET, &Rs485Bridge::handle_page, false);
  this->register_uri("/rs485/", HTTP_GET, &Rs485Bridge::handle_page, false);
  this->register_uri("/rs485/status", HTTP_GET, &Rs485Bridge::handle_status, false);
  this->register_uri("/rs485/tx", HTTP_POST, &Rs485Bridge::handle_tx, false);
  this->register_uri("/rs485/rx", HTTP_GET, &Rs485Bridge::handle_rx, false);
  this->register_uri("/rs485/ws", HTTP_GET, &Rs485Bridge::handle_ws, true);

  ESP_LOGI(TAG, "RS485 bridge ready: WS  ws://<ip>:%u/rs485/ws | Web http://<ip>:%u/rs485/",
           static_cast<unsigned>(this->port_), static_cast<unsigned>(this->port_));
}

void Rs485Bridge::loop() {
  if (this->server_ == nullptr)
    return;

  uint8_t buf[512];
  int n = uart_read_bytes(this->uart_num_, buf, sizeof(buf), 0);
  if (n > 0) {
    this->rx_total_ += n;
    this->push_rx(buf, static_cast<size_t>(n));
    this->ws_broadcast(buf, static_cast<size_t>(n));
  } else if (n < 0) {
    ESP_LOGW(TAG, "uart_read_bytes failed: %d", n);
  }

  uint32_t now = millis();
  if (now - this->last_ping_ms_ >= 5000) {
    this->last_ping_ms_ = now;
    this->ws_ping_all();
  }
}

void Rs485Bridge::dump_config() {
  ESP_LOGCONFIG(TAG, "Rs485Bridge:");
  ESP_LOGCONFIG(TAG, "  UART channel %d, pins TX=%d RX=%d DIR=%d", this->uart_num_,
                this->tx_pin_, this->rx_pin_, this->dir_pin_);
  ESP_LOGCONFIG(TAG, "  RS485 half-duplex, %u baud", static_cast<unsigned>(this->baud_rate_));
  ESP_LOGCONFIG(TAG, "  HTTP port %u", static_cast<unsigned>(this->port_));
}

}  // namespace rs485_bridge
}  // namespace esphome

#endif  // USE_ESP_IDF
#endif  // USE_ESP32