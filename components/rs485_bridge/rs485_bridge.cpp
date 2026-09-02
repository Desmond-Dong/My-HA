#include "rs485_bridge.h"
#include "rs485_page.h"

#ifdef USE_ESP32
#if defined(USE_ESP_IDF)

#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include "esphome/components/api/api_server.h"
#include "esphome/components/m5_tab5_camera/m5_tab5_camera.h"
#include "esphome/components/network/network.h"

#include <esp_heap_caps.h>

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
// Web handler (registered on ESPHome's web_server; runs on the httpd task)
// ---------------------------------------------------------------------------

// WebRequestHandler: routes /rs485/* endpoints on the shared web server.
class Rs485WebHandler : public AsyncWebHandler {
 public:
  explicit Rs485WebHandler(Rs485Bridge *bridge) : bridge_(bridge) {}

  bool canHandle(AsyncWebServerRequest *request) const override {
    const auto method = request->method();
    if (method != HTTP_GET && method != HTTP_POST)
      return false;
    char buf[AsyncWebServerRequest::URL_BUF_SIZE];
    StringRef url = request->url_to(buf);
    return url == "/rs485" || strncmp(url.c_str(), "/rs485/", 7) == 0;
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    const auto method = request->method();
    char buf[AsyncWebServerRequest::URL_BUF_SIZE];
    StringRef url = request->url_to(buf);

    if (method == HTTP_GET && (url == "/rs485" || url == "/rs485/")) {
      request->send(request->beginResponse(200, "text/html", (const uint8_t *) kPageHtml,
                                           strlen(kPageHtml)));
      return;
    }
    if (method == HTTP_GET && url == "/rs485/status") {
      this->handle_status(*request);
      return;
    }
    if (method == HTTP_GET && url == "/debug") {
      this->handle_debug(*request);
      return;
    }
    if (method == HTTP_GET && url == "/rs485/rx") {
      this->handle_rx(*request);
      return;
    }
    if (method == HTTP_POST && url == "/rs485/tx") {
      // Body chunks were streamed to the bus by handleBody(); reply now.
      char body[48];
      snprintf(body, sizeof(body), "{\"written\":%u,\"ok\":true}",
               static_cast<unsigned>(this->tx_written_));
      request->send(200, "application/json", body);
      this->tx_written_ = 0;
      return;
    }
    request->send(404, "text/plain", "not found");
  }

  void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index,
                  size_t total) override {
    // Sequential chunks for a single request; stream straight to the bus.
    if (len == 0)
      return;
    if (total > 4096)
      return;
    ESP_LOGD(TAG, "POST /rs485/tx -> UART: %u bytes", static_cast<unsigned>(len));
    if (this->bridge_->write_bytes_val(data, len))
      this->tx_written_ += len;
  }

 protected:
  // Full device diagnostics as JSON (browser-friendly: open http://<ip>/debug).
  void handle_debug(AsyncWebServerRequest &request) {
    bool api_ok = api::global_api_server != nullptr && api::global_api_server->is_connected();
    char body[512];
    snprintf(body, sizeof(body),
             "{\"uptime_s\":%u,"
             "\"free_heap\":%u,\"min_free_heap\":%u,\"free_psram\":%u,"
             "\"cam_requesters\":%u,\"cam_frames\":%u,\"cam_failures\":%u,"
             "\"api_connected\":%s,\"network_connected\":%s}",
             static_cast<unsigned>(millis() / 1000U),
             static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)),
             static_cast<unsigned>(heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL)),
             static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_SPIRAM)),
             static_cast<unsigned>(m5_tab5_camera::M5Tab5Camera::stat_requesters_.load()),
             static_cast<unsigned>(m5_tab5_camera::M5Tab5Camera::stat_frames_.load()),
             static_cast<unsigned>(m5_tab5_camera::M5Tab5Camera::stat_failures_.load()),
             api_ok ? "true" : "false",
             network::is_connected() ? "true" : "false");
    request.send(200, "application/json", body);
  }

  void handle_status(AsyncWebServerRequest &request) const {
    auto *self = this->bridge_;
    uint32_t rx_waiting = 0;
    if (xSemaphoreTake(self->rx_mutex_, portMAX_DELAY) == pdTRUE) {
      rx_waiting = static_cast<uint32_t>(self->rx_ring_.size());
      xSemaphoreGive(self->rx_mutex_);
    }
    char body[384];
    snprintf(body, sizeof(body),
             "{\"baud_rate\":%u,\"tx_pin\":%u,\"rx_pin\":%u,\"dir_pin\":%u,"
             "\"data_bits\":%u,\"stop_bits\":%u,\"parity\":%u,"
             "\"rx_waiting\":%u,\"tx_total\":%u,\"rx_total\":%u}",
             static_cast<unsigned>(self->baud_rate_),
             static_cast<unsigned>(self->tx_pin_), static_cast<unsigned>(self->rx_pin_),
             static_cast<unsigned>(self->dir_pin_), static_cast<unsigned>(self->data_bits_),
             static_cast<unsigned>(self->stop_bits_), static_cast<unsigned>(self->parity_),
             static_cast<unsigned>(rx_waiting), static_cast<unsigned>(self->tx_total_),
             static_cast<unsigned>(self->rx_total_));
    request.send(200, "application/json", body);
  }

  void handle_rx(AsyncWebServerRequest &request) {
    auto *self = this->bridge_;
    bool clear = true;
    bool raw = false;
    if (request.hasParam("clear"))
      clear = request.getParam("clear")->value() != "0";
    if (request.hasParam("raw"))
      raw = request.getParam("raw")->value() == "1";

    std::vector<uint8_t> snapshot;
    if (xSemaphoreTake(self->rx_mutex_, portMAX_DELAY) == pdTRUE) {
      snapshot = self->rx_ring_;
      if (clear)
        self->rx_ring_.clear();
      self->rx_waiting_ = self->rx_ring_.size();
      xSemaphoreGive(self->rx_mutex_);
    }

    if (raw) {
      if (snapshot.empty()) {
        request.send(200, "application/octet-stream", "");
      } else {
        request.send(request.beginResponse(200, "application/octet-stream", snapshot.data(),
                                           snapshot.size()));
      }
      return;
    }

    static const char kHex[] = "0123456789abcdef";
    std::string hex;
    hex.reserve(snapshot.size() * 2 + 32);
    for (uint8_t b : snapshot) {
      hex.push_back(kHex[(b >> 4) & 0x0f]);
      hex.push_back(kHex[b & 0x0f]);
    }
    std::string body = "{\"n\":" + std::to_string(snapshot.size()) + ",\"hex\":\"" + hex + "\"}";
    request.send(200, "application/json", body.c_str());
  }

  Rs485Bridge *bridge_;
  uint32_t tx_written_{0};
};

// ---------------------------------------------------------------------------
// setup / loop
// ---------------------------------------------------------------------------

void Rs485Bridge::setup() {
  ESP_LOGI(TAG, "Setting up RS485 bridge (TX=%d RX=%d DIR=%d baud=%u ch=%d)",
           this->tx_pin_, this->rx_pin_, this->dir_pin_,
           static_cast<unsigned>(this->baud_rate_), static_cast<int>(this->uart_num_));

  this->rx_mutex_ = xSemaphoreCreateMutex();

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
}

void Rs485Bridge::loop() {
  // Lazily attach to ESPHome's web server once it is up (avoids setup-order
  // coupling); the handler only claims /rs485* URLs. WebServerBase::init()
  // creates the AsyncWebServer, so a non-null get_server() means it started.
  if (this->web_handler_ == nullptr) {
    auto *base = web_server_base::global_web_server_base;
    if (base != nullptr) {
      AsyncWebServer *server = base->get_server();
      if (server != nullptr) {
        this->web_handler_ = new Rs485WebHandler(this);  // NOLINT
        server->addHandler(this->web_handler_);
        ESP_LOGI(TAG, "RS485 web endpoints attached to ESPHome web server: http://<ip>/rs485/");
      }
    }
  }

  uint8_t buf[512];
  int n = uart_read_bytes(this->uart_num_, buf, sizeof(buf), 0);
  if (n > 0) {
    this->rx_total_ += n;
    this->push_rx(buf, static_cast<size_t>(n));
  } else if (n < 0) {
    ESP_LOGW(TAG, "uart_read_bytes failed: %d", n);
  }
}

void Rs485Bridge::dump_config() {
  ESP_LOGCONFIG(TAG, "Rs485Bridge:");
  ESP_LOGCONFIG(TAG, "  UART channel %d, pins TX=%d RX=%d DIR=%d", this->uart_num_,
                this->tx_pin_, this->rx_pin_, this->dir_pin_);
  ESP_LOGCONFIG(TAG, "  RS485 half-duplex, %u baud", static_cast<unsigned>(this->baud_rate_));
  ESP_LOGCONFIG(TAG, "  Web: /rs485/* on the ESPHome web_server (http://<ip>/rs485/)");
}

}  // namespace rs485_bridge
}  // namespace esphome

#endif  // USE_ESP_IDF
#endif  // USE_ESP32
