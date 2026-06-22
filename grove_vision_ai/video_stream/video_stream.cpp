#include "video_stream.h"
#include "esphome/core/log.h"
#include "esphome/components/web_server/web_server.h"

#ifdef USE_GROVE_VISION_AI_VIDEO_STREAM

#include <base64.h>

namespace esphome {
namespace grove_vision_ai {

static const char *const TAG = "grove_vision_ai.video_stream";

GroveVisionAIVideoStream::GroveVisionAIVideoStream() : Component() {}

GroveVisionAIVideoStream::~GroveVisionAIVideoStream() {
  if (stream_server_) {
    delete stream_server_;
  }
  if (stream_client_) {
    delete stream_client_;
  }
}

void GroveVisionAIVideoStream::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Grove Vision AI video stream");
  ESP_LOGCONFIG(TAG, "  Stream port: %u", port_);
  
  // Start streaming server
  if (!start_stream_server()) {
    ESP_LOGE(TAG, "Failed to start streaming server");
    this->mark_failed();
    return;
  }
  
  // Register HTTP handler with web_server component
  auto *web_server = web_server::global_web_server;
  if (web_server) {
    web_server->serve_http(
      [this](AsyncWebServerRequest *request) {
        this->handle_http_request(request);
      }
    );
  }
  
  ESP_LOGI(TAG, "Video stream ready. Access at: http://%s:%d/stream", 
           WiFi.localIP().toString().c_str(), port_);
}

void GroveVisionAIVideoStream::loop() {
  uint32_t now = millis();
  
  // Process streaming client
  if (stream_client_ && stream_client_->connected()) {
    streaming_active_ = true;
    handle_stream_client();
  } else {
    streaming_active_ = false;
    if (stream_client_) {
      delete stream_client_;
      stream_client_ = nullptr;
    }
  }
  
  // Accept new connections
  if (stream_server_ && stream_server_->hasClient()) {
    if (stream_client_ && stream_client_->connected()) {
      stream_server_->available().stop();
      ESP_LOGD(TAG, "Stream client rejected - already connected");
    } else {
      if (stream_client_) {
        delete stream_client_;
      }
      stream_client_ = new WiFiClient(stream_server_->available());
      ESP_LOGI(TAG, "Stream client connected from %s", 
               stream_client_->remoteIP().toString().c_str());
    }
  }
}

void GroveVisionAIVideoStream::dump_config() {
  ESP_LOGCONFIG(TAG, "Grove Vision AI Video Stream:");
  ESP_LOGCONFIG(TAG, "  Port: %u", port_);
  ESP_LOGCONFIG(TAG, "  Streaming: %s", streaming_active_ ? "Yes" : "No");
}

bool GroveVisionAIVideoStream::start_stream_server() {
  stream_server_ = new WiFiServer(port_);
  stream_server_->begin();
  ESP_LOGI(TAG, "Streaming server started on port %u", port_);
  return true;
}

void GroveVisionAIVideoStream::handle_http_request(AsyncWebServerRequest *request) {
  String path = request->url();
  
  if (path == "/stream" || path == "/stream/frame") {
    ESP_LOGD(TAG, "Stream request from %s", request->client()->remoteIP().toString().c_str());
    
    // Send MJPEG stream headers
    AsyncWebServerResponse *response = request->beginChunkedResponse(
      "multipart/x-mixed-replace; boundary=frame",
      [this](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
        // This is a simplified implementation
        // In production, you'd need proper MJPEG streaming
        return 0;
      }
    );
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Connection", "close");
    request->send(response);
  } else if (path == "/stream/result") {
    // Send detection results as JSON
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", 
      "{\"status\":\"ok\"}");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  } else {
    request->send(404);
  }
}

void GroveVisionAIVideoStream::handle_stream_client() {
  uint32_t now = millis();
  
  // Send frame at controlled rate
  if (now - last_frame_time_ >= FRAME_INTERVAL_MS) {
    last_frame_time_ = now;
    send_mjpeg_frame();
  }
}

void GroveVisionAIVideoStream::send_mjpeg_frame() {
  if (!stream_client_ || !stream_client_->connected()) {
    return;
  }
  
  // Get frame from parent component
  uint8_t *frame_buffer = nullptr;
  size_t frame_len = 0;
  
  if (parent_ && get_frame(&frame_buffer, &frame_len)) {
    // Send MJPEG boundary
    String boundary = "--frame\r\n";
    String header = "Content-Type: image/jpeg\r\nContent-Length: " + String(frame_len) + "\r\n\r\n";
    
    stream_client_->print(boundary);
    stream_client_->print(header);
    stream_client_->write(frame_buffer, frame_len);
    stream_client_->print("\r\n");
    
    ESP_LOGV(TAG, "Sent frame: %zu bytes", frame_len);
  }
}

bool GroveVisionAIVideoStream::get_frame(uint8_t **buffer, size_t *len) {
  // This method should be called by parent component when a new frame is available
  *buffer = stream_buffer_;
  *len = stream_len_;
  return stream_len_ > 0;
}

bool GroveVisionAIVideoStream::process_image_data(const char *base64_data) {
  if (!base64_data || strlen(base64_data) == 0) {
    return false;
  }
  
  // Decode base64 to JPEG
  size_t decoded_len = STREAM_BUFFER_SIZE;
  if (decode_base64_to_jpeg(base64_data, stream_buffer_, &decoded_len)) {
    stream_len_ = decoded_len;
    ESP_LOGV(TAG, "Processed image: %zu bytes", decoded_len);
    return true;
  }
  
  return false;
}

bool GroveVisionAIVideoStream::decode_base64_to_jpeg(const char *base64, uint8_t *out, size_t *out_len) {
  // Use ESP32's base64 decode
  size_t input_len = strlen(base64);
  size_t max_output_len = *out_len;
  
  // Simple base64 decode (ESP32 has built-in functions)
  int decoded = base64_decode_chars(base64, input_len, (char *)out);
  
  if (decoded > 0 && (size_t)decoded <= max_output_len) {
    *out_len = decoded;
    return true;
  }
  
  return false;
}

}  // namespace grove_vision_ai
}  // namespace esphome

#endif