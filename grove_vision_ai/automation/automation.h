#pragma once

#include "esphome/core/automation.h"
#include "../grove_vision_ai.h"

namespace esphome {
namespace grove_vision_ai {

class GroveVisionAIOnDetectionTrigger : public Trigger<int, int, int, int, int, float> {
 public:
  explicit GroveVisionAIOnDetectionTrigger(GroveVisionAI *parent) {
    parent->set_detection_callback(
        [this](int target_id, int x, int y, int w, int h, float score) {
          this->trigger(target_id, x, y, w, h, score);
        }
    );
  }
};

}  // namespace grove_vision_ai
}  // namespace esphome