#pragma once

#include "esphome/core/component.h"
#include "esphome/components/web_server/web_server.h"
#include "../grove_vision_ai.h"

#ifdef USE_GROVE_VISION_AI_VIDEO_STREAM

#include <WiFi.h>
#include <esp_camera.h>

namespace esphome {
namespace grove_vision_ai {

class GroveVisionAIVideoStream : public Component {
 public:
  GroveVisionAIVideoStream();
  ~GroveVisionAIVideoStream();

  void setup() override;
  void loop() override;
  void dump_config() override;
  
  void set_parent(GroveVisionAI *parent) { parent_ = parent; }
  void set_port(uint16_t port) { port_ = port; }
  
  // Start streaming server
  bool start_stream_server();
  
  // Get current frame
  bool get_frame(uint8_t **buffer, size_t *len);

 protected:
  GroveVisionAI *parent_;
  uint16_t port_{8080};
  WiFiServer *stream_server_{nullptr};
  WiFiClient *stream_client_{nullptr};
  
  // JPEG encoder settings
  static const int JPEG_QUALITY = 12;
  static const int FRAME_INTERVAL_MS = 100;
  
  // Stream buffer
  static const size_t STREAM_BUFFER_SIZE = 64 * 1024;
  uint8_t stream_buffer_[STREAM_BUFFER_SIZE];
  size_t stream_len_{0};
  
  uint32_t last_frame_time_{0};
  bool streaming_active_{false};
  
  // HTTP handlers
  void handle_stream_client();
  void handle_http_request(AsyncWebServerRequest *request);
  void send_mjpeg_frame();
  
  // Image processing
  bool process_image_data(const char *base64_data);
  bool decode_base64_to_jpeg(const char *base64, uint8_t *out, size_t *out_len);
};

}  // namespace grove_vision_ai
}  // namespace esphome

#endif