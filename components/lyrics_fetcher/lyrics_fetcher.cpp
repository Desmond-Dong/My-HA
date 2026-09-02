#include "lyrics_fetcher.h"

#ifdef USE_ESP32

#include "esphome/core/log.h"

#include <esp_http_client.h>

#include <cstring>

namespace esphome {
namespace lyrics_fetcher {

static const char *const TAG = "lyrics_fetcher";
static constexpr size_t URL_BUF_SIZE = 512;
static constexpr size_t BODY_MAX_BYTES = 64 * 1024;
static constexpr int FETCH_TIMEOUT_MS = 10000;

void LyricsFetcher::setup() {
  this->req_queue_ = xQueueCreate(2, URL_BUF_SIZE);
  this->result_mutex_ = xSemaphoreCreateMutex();
  if (this->req_queue_ == nullptr || this->result_mutex_ == nullptr) {
    ESP_LOGE(TAG, "queue/mutex alloc failed");
    this->mark_failed();
    return;
  }
  // Low priority: lyrics are never worth preempting audio/display tasks.
  if (xTaskCreate(LyricsFetcher::task_trampoline_, "lyrics_fetch", 8192, this, 1,
                  &this->task_) != pdPASS) {
    ESP_LOGE(TAG, "task create failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "Lyrics fetcher task started");
}

void LyricsFetcher::request(const std::string &url) {
  if (this->req_queue_ == nullptr)
    return;
  if (url.size() >= URL_BUF_SIZE) {
    ESP_LOGW(TAG, "url too long (%u), dropped", static_cast<unsigned>(url.size()));
    return;
  }
  char buf[URL_BUF_SIZE];
  std::strncpy(buf, url.c_str(), URL_BUF_SIZE - 1);
  buf[URL_BUF_SIZE - 1] = '\0';
  // Drop any queued request; only the latest matters.
  xQueueReset(this->req_queue_);
  if (xQueueSend(this->req_queue_, buf, 0) != pdTRUE) {
    ESP_LOGW(TAG, "request queue full, dropped");
  }
}

void LyricsFetcher::task_trampoline_(void *arg) {
  static_cast<LyricsFetcher *>(arg)->task_loop_();
}

void LyricsFetcher::task_loop_() {
  char url[URL_BUF_SIZE];
  for (;;) {
    if (xQueueReceive(this->req_queue_, url, portMAX_DELAY) != pdTRUE)
      continue;

    ESP_LOGD(TAG, "fetch: %s", url);

    esp_http_client_config_t cfg = {};
    cfg.url = url;
    cfg.timeout_ms = FETCH_TIMEOUT_MS;
    cfg.buffer_size = 4096;
    cfg.keep_alive_enable = false;

    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (client == nullptr) {
      this->set_error_("init failed");
      continue;
    }

    std::string body;
    bool ok = false;
    std::string err;

    if (esp_http_client_open(client, 0) == ESP_OK) {
      esp_http_client_fetch_headers(client);
      int status = esp_http_client_get_status_code(client);
      if (status == 200) {
        char chunk[1024];
        int n;
        while ((n = esp_http_client_read(client, chunk, sizeof(chunk))) > 0) {
          if (body.size() + n > BODY_MAX_BYTES) {
            err = "body too large";
            break;
          }
          body.append(chunk, static_cast<size_t>(n));
        }
        if (err.empty() && !body.empty()) {
          ok = true;
        } else if (err.empty()) {
          err = "empty body";
        }
      } else {
        err = "http status " + std::to_string(status);
      }
      esp_http_client_close(client);
    } else {
      err = "connect failed";
      esp_http_client_cleanup(client);
    }

    if (ok) {
      this->set_result_(body);
    } else {
      this->set_error_(err);
    }
  }
}

void LyricsFetcher::set_result_(const std::string &body) {
  if (xSemaphoreTake(this->result_mutex_, portMAX_DELAY) != pdTRUE)
    return;
  this->pending_body_ = body;
  this->result_ready_ = true;
  xSemaphoreGive(this->result_mutex_);
}

void LyricsFetcher::set_error_(const std::string &msg) {
  if (xSemaphoreTake(this->result_mutex_, portMAX_DELAY) != pdTRUE)
    return;
  this->pending_error_ = msg;
  this->error_ready_ = true;
  xSemaphoreGive(this->result_mutex_);
}

void LyricsFetcher::loop() {
  if (this->result_mutex_ == nullptr)
    return;
  if (xSemaphoreTake(this->result_mutex_, 0) != pdTRUE)
    return;
  std::string body, err;
  bool has_body = false, has_err = false;
  if (this->error_ready_) {
    err = this->pending_error_;
    this->pending_error_.clear();
    this->error_ready_ = false;
    has_err = true;
  }
  if (this->result_ready_) {
    body = this->pending_body_;
    this->pending_body_.clear();
    this->result_ready_ = false;
    has_body = true;
  }
  xSemaphoreGive(this->result_mutex_);

  if (has_err && this->error_trigger_ != nullptr) {
    this->error_trigger_->trigger(err);
  }
  if (has_body && this->result_trigger_ != nullptr) {
    this->result_trigger_->trigger(body);
  }
}

void LyricsFetcher::dump_config() { ESP_LOGCONFIG(TAG, "Lyrics Fetcher: task-based"); }

}  // namespace lyrics_fetcher
}  // namespace esphome

#endif  // USE_ESP32
