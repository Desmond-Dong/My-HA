#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../grove_vision_ai.h"

namespace esphome {
namespace grove_vision_ai {

class GroveVisionAIBinarySensor : public binary_sensor.BinarySensor, public Component {
 public:
  void set_parent(GroveVisionAI *parent) { parent_ = parent; }
  void set_target_id(int target_id) { target_id_ = target_id; }
  void set_min_confidence(float confidence) { min_confidence_ = confidence; }
  
  void setup() override;
  void loop() override;
  
  void on_detection(int target_id, int x, int y, int w, int h, float score);

 protected:
  GroveVisionAI *parent_;
  int target_id_{-1};  // -1 means any target
  float min_confidence_{0.5f};
  uint32_t last_detection_time_{0};
  static const uint32_t DETECTION_TIMEOUT = 3000;  // 3 seconds
};

}  // namespace grove_vision_ai
}  // namespace esphome