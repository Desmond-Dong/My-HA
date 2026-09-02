#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

#ifdef USE_ESP32

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <cstdint>
#include <string>

namespace esphome {
namespace lyrics_fetcher {

/// Fetches a URL with esp_http_client on its own FreeRTOS task so the
/// main event loop (LVGL, api, camera, audio) is never blocked, then
/// fires on_result (HTTP 200 body) or on_error on the main loop.
class LyricsFetcher : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  /// Queue a fetch (non-blocking; replaces any queued request).
  void request(const std::string &url);

  void set_result_trigger(Trigger<std::string> *trig) { this->result_trigger_ = trig; }
  void set_error_trigger(Trigger<std::string> *trig) { this->error_trigger_ = trig; }

 protected:
  static void task_trampoline_(void *arg);
  void task_loop_();
  void set_result_(const std::string &body);
  void set_error_(const std::string &msg);

  QueueHandle_t req_queue_{nullptr};
  SemaphoreHandle_t result_mutex_{nullptr};
  TaskHandle_t task_{nullptr};

  // written by task under mutex, consumed on main loop
  std::string pending_body_;
  std::string pending_error_;
  bool result_ready_{false};
  bool error_ready_{false};

  Trigger<std::string> *result_trigger_{nullptr};
  Trigger<std::string> *error_trigger_{nullptr};
};

template<typename... Ts> class RequestAction : public Action<Ts...> {
 public:
  explicit RequestAction(LyricsFetcher *parent) : parent_(parent) {}
  TEMPLATABLE_VALUE(std::string, url)
  void play() override { this->parent_->request(this->url_.value()); }

 protected:
  LyricsFetcher *parent_;
};

class ResultTrigger : public Trigger<std::string> {};
class ErrorTrigger : public Trigger<std::string> {};

}  // namespace lyrics_fetcher
}  // namespace esphome

#endif  // USE_ESP32
