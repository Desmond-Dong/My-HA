#include "grove_vision_ai.h"
#include "esphome/core/log.h"

// ArduinoJson for JSON parsing
#include <ArduinoJson.h>

#ifdef USE_GROVE_VISION_AI_VIDEO_STREAM
#include "video_stream/video_stream.h"
#endif

namespace esphome {
namespace grove_vision_ai {

static const char *const TAG = "grove_vision_ai";

void GroveVisionAI::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Grove Vision AI V2...");

  if (!init_sscma()) {
    ESP_LOGE(TAG, "Failed to initialize SSCMA library");
    this->mark_failed();
    return;
  }

  ESP_LOGI(TAG, "Grove Vision AI V2 initialized successfully");
}

void GroveVisionAI::loop() {
  uint32_t now = millis();
  
  // Check if it's time for a new inference
  if (now - last_update_ >= update_interval_) {
    last_update_ = now;
    invoke_inference();
  }

  // Process any incoming data
  while (this->available()) {
    if (rx_pos_ < RX_BUFFER_SIZE - 1) {
      rx_buffer_[rx_pos_++] = this->read();
    } else {
      // Buffer overflow, reset
      ESP_LOGW(TAG, "RX buffer overflow, resetting");
      rx_pos_ = 0;
      this->read();  // Clear the byte
    }
  }

  // Check if we have a complete response
  if (rx_pos_ > 0) {
    // Look for response suffix "}\n"
    if (rx_pos_ >= 2 && rx_buffer_[rx_pos_ - 2] == '}' && rx_buffer_[rx_pos_ - 1] == '\n') {
      // Find prefix
      const char *prefix = "\r{";
      bool found_prefix = false;
      size_t start_pos = 0;
      
      for (size_t i = 0; i < rx_pos_ - 1; i++) {
        if (rx_buffer_[i] == '\r' && rx_buffer_[i + 1] == '{') {
          start_pos = i;
          found_prefix = true;
          break;
        }
      }

      if (found_prefix) {
        rx_buffer_[rx_pos_] = '\0';  // Null terminate
        parse_response((const char *)&rx_buffer_[start_pos], rx_pos_ - start_pos);
      }
      
      rx_pos_ = 0;  // Reset buffer
    }
  }
}

void GroveVisionAI::invoke_inference() {
  int result = sscma_invoke();
  if (result != 0) {
    ESP_LOGW(TAG, "Inference failed with code: %d", result);
  }
}

bool GroveVisionAI::init_sscma() {
  // Send AT command to check connection
  const char *cmd = "AT+ID\r\n";
  this->write_array((const uint8_t *)cmd, strlen(cmd));
  
  // Wait for response
  delay(100);
  
  // Simple check - in production, we'd parse the full response
  ESP_LOGI(TAG, "SSCMA initialization command sent");
  return true;
}

int GroveVisionAI::sscma_invoke() {
  // AT+INVOKE=1,0,1\r\n - 1 invocation, no filter, with filter
  const char *cmd = "AT+INVOKE=1,0,1\r\n";
  this->write_array((const uint8_t *)cmd, strlen(cmd));
  ESP_LOGV(TAG, "Inference command sent");
  return 0;
}

void GroveVisionAI::parse_response(const char *data, size_t len) {
  ESP_LOGV(TAG, "Received response: %s", data);

  // Parse JSON response using ArduinoJson
  // Document size depends on expected response - 2048 should be enough for most cases
  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, data);

  if (error) {
    ESP_LOGW(TAG, "JSON parse error: %s", error.c_str());
    return;
  }

  // Check if this is an INVOKE event
  if (doc["type"] == 1 && doc["name"] == "INVOKE") {
    JsonObject data_obj = doc["data"];
    
    // Parse boxes (detections)
    if (data_obj.containsKey("boxes")) {
      JsonArray boxes = data_obj["boxes"];
      int detection_count = boxes.size();
      ESP_LOGD(TAG, "Found %d detection(s)", detection_count);

      // Process each detection
      for (JsonObject box : boxes) {
        int x = box[0];
        int y = box[1];
        int w = box[2];
        int h = box[3];
        float score = box[4];
        int target = box[5];

        ESP_LOGD(TAG, "Detection: target=%d, score=%.2f, bbox=(%d,%d,%d,%d)", 
                 target, score, x, y, w, h);

        // Trigger callback if set
        if (detection_callback_) {
          detection_callback_(target, x, y, w, h, score);
        }
      }
    }

    // Parse performance metrics
    if (data_obj.containsKey("perf")) {
      JsonArray perf = data_obj["perf"];
      uint16_t preprocess = perf[0];
      uint16_t inference = perf[1];
      uint16_t postprocess = perf[2];
      uint16_t total = preprocess + inference + postprocess;
      
      ESP_LOGD(TAG, "Performance: pre=%d, inf=%d, post=%d, total=%d ms", 
               preprocess, inference, postprocess, total);
    }

    // Parse classes (classification results)
    if (data_obj.containsKey("classes")) {
      JsonArray classes = data_obj["classes"];
      ESP_LOGD(TAG, "Found %d classification(s)", classes.size());
      
      for (JsonObject cls : classes) {
        float score = cls[0];
        int target = cls[1];
        ESP_LOGD(TAG, "Classification: target=%d, score=%.2f", target, score);
      }
    }

    // Parse keypoints (pose estimation)
    if (data_obj.containsKey("keypoints")) {
      JsonArray keypoints = data_obj["keypoints"];
      ESP_LOGD(TAG, "Found %d keypoint set(s)", keypoints.size());
      
      for (JsonObject kp : keypoints) {
        JsonArray box = kp[0];
        JsonArray points = kp[1];
        ESP_LOGD(TAG, "Keypoints: bbox=(%d,%d,%d,%d,%d,%d), points=%d", 
                 box[0], box[1], box[2], box[3], box[4], box[5], points.size());
      }
    }

    // Parse points (landmarks)
    if (data_obj.containsKey("points")) {
      JsonArray points = data_obj["points"];
      ESP_LOGD(TAG, "Found %d point(s)", points.size());
      
      for (JsonObject point : points) {
        int x = point[0];
        int y = point[1];
        float score = point[2];
        int target = point[3];
        ESP_LOGD(TAG, "Point: target=%d, score=%.2f, pos=(%d,%d)", target, score, x, y);
      }
    }

    // Parse image data (base64 encoded JPEG)
    if (data_obj.containsKey("image")) {
      const char *image_data = data_obj["image"];
      if (image_data && strlen(image_data) > 0) {
        process_image_data(image_data);
      }
    }
  }
}

void GroveVisionAI::process_image_data(const char *base64_data) {
#ifdef USE_GROVE_VISION_AI_VIDEO_STREAM
  if (video_stream_) {
    video_stream_->process_image_data(base64_data);
  }
#endif
  ESP_LOGV(TAG, "Image data received: %zu chars", strlen(base64_data));
}

void GroveVisionAI::dump_config() {
  ESP_LOGCONFIG(TAG, "Grove Vision AI V2:");
  ESP_LOGCONFIG(TAG, "  Update interval: %u ms", update_interval_);
  ESP_LOGCONFIG(TAG, "  RX buffer size: %u bytes", RX_BUFFER_SIZE);
}

}  // namespace grove_vision_ai
}  // namespace esphome