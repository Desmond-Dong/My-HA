#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "../grove_vision_ai.h"

namespace esphome {
namespace grove_vision_ai {

class GroveVisionAITextSensor : public text_sensor.TextSensor, public Component {
 public:
  void set_parent(GroveVisionAI *parent) { parent_ = parent; }
  
  void setup() override;
  void loop() override;
  
  void update_results(const std::string &results);

 protected:
  GroveVisionAI *parent_;
  std::string results_;
};

}  // namespace grove_vision_ai
}  // namespace esphome