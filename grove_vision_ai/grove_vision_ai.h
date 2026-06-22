#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace grove_vision_ai {

// Forward declaration
class GroveVisionAIVideoStream;

class GroveVisionAI : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  // Methods to trigger AI inference
  void invoke_inference();
  void set_update_interval(uint32_t interval) { update_interval_ = interval; }

  // Callbacks for detection results
  void set_detection_callback(std::function<void(int, int, int, int, int, float)> callback) {
    detection_callback_ = callback;
  }

  // Video stream support
  void set_video_stream(GroveVisionAIVideoStream *stream) { video_stream_ = stream; }

 protected:
  uint32_t update_interval_{1000};  // Default 1 second
  uint32_t last_update_{0};
  
  // Detection callback: target_id, x, y, width, height, score
  std::function<void(int, int, int, int, int, float)> detection_callback_{nullptr};

  // Video stream component
  GroveVisionAIVideoStream *video_stream_{nullptr};

  // SSCMA library interface
  bool init_sscma();
  int sscma_invoke();
  
  // Buffer for receiving data
  static const size_t RX_BUFFER_SIZE = 32768;
  uint8_t rx_buffer_[RX_BUFFER_SIZE];
  size_t rx_pos_{0};

  // Parse received data
  void parse_response(const char *data, size_t len);
  
  // Image data processing
  void process_image_data(const char *base64_data);
};

}  // namespace grove_vision_ai
}  // namespace esphome