#include "text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace grove_vision_ai {

static const char *const TAG = "grove_vision_ai.text_sensor";

void GroveVisionAITextSensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Grove Vision AI text sensor");
}

void GroveVisionAITextSensor::loop() {
  // Text sensor is updated via callback
}

void GroveVisionAITextSensor::update_results(const std::string &results) {
  results_ = results;
  this->publish_state(results);
  ESP_LOGD(TAG, "Detection results: %s", results.c_str());
}

}  // namespace grove_vision_ai
}  // namespace esphome