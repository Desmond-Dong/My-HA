#include "binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace grove_vision_ai {

static const char *const TAG = "grove_vision_ai.binary_sensor";

void GroveVisionAIBinarySensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Grove Vision AI binary sensor");
  ESP_LOGCONFIG(TAG, "  Target ID: %d (any if -1)", target_id_);
  ESP_LOGCONFIG(TAG, "  Min confidence: %.2f", min_confidence_);
}

void GroveVisionAIBinarySensor::loop() {
  // Check for timeout
  if (this->state && (millis() - last_detection_time_ > DETECTION_TIMEOUT)) {
    this->publish_state(false);
    ESP_LOGD(TAG, "Detection timeout");
  }
}

void GroveVisionAIBinarySensor::on_detection(int target_id, int x, int y, int w, int h, float score) {
  // Check if this detection matches our criteria
  if (target_id_ >= 0 && target_id != target_id_) {
    return;  // Not the target we're looking for
  }
  
  if (score < min_confidence_) {
    return;  // Confidence too low
  }
  
  last_detection_time_ = millis();
  this->publish_state(true);
  ESP_LOGD(TAG, "Detection: target=%d, score=%.2f, bbox=(%d,%d,%d,%d)", 
           target_id, score, x, y, w, h);
}

}  // namespace grove_vision_ai
}  // namespace esphome