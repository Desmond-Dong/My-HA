#include "sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace grove_vision_ai {

static const char *const TAG = "grove_vision_ai.sensor";

void GroveVisionAISensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Grove Vision AI sensor");
}

void GroveVisionAISensor::loop() {
  // Update sensor state
  this->publish_state(detection_count_);
}

void GroveVisionAISensor::update_detection_count(int count) {
  detection_count_ = count;
  this->publish_state(count);
}

void GroveVisionAISensor::update_performance_time(float time) {
  performance_time_ = time;
}

}  // namespace grove_vision_ai
}  // namespace esphome