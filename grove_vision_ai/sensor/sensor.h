#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../grove_vision_ai.h"

namespace esphome {
namespace grove_vision_ai {

class GroveVisionAISensor : public sensor::Sensor, public Component {
 public:
  void set_parent(GroveVisionAI *parent) { parent_ = parent; }
  
  void setup() override;
  void loop() override;
  
  void update_detection_count(int count);
  void update_performance_time(float time);

 protected:
  GroveVisionAI *parent_;
  int detection_count_{0};
  float performance_time_{0.0f};
};

}  // namespace grove_vision_ai
}  // namespace esphome