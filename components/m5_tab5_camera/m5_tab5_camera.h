#pragma once

#ifdef USE_ESP32

#include <atomic>
#include <freertos/FreeRTOS.h>
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/camera/camera.h"
#include "esphome/core/helpers.h"

struct jpeg_encoder_t;
typedef struct jpeg_encoder_t *jpeg_encoder_handle_t;

namespace esphome::m5_tab5_camera {

class M5Tab5Camera;

enum M5Tab5CameraFrameSize {
  M5_TAB5_CAMERA_SIZE_400X296,
  M5_TAB5_CAMERA_SIZE_640X480,
  M5_TAB5_CAMERA_SIZE_800X600,
  M5_TAB5_CAMERA_SIZE_1024X768,
  M5_TAB5_CAMERA_SIZE_1280X720,
  M5_TAB5_CAMERA_SIZE_1280X960,
  M5_TAB5_CAMERA_SIZE_1600X1200,
};

/* ---------- V4L2 mapped buffer ---------- */
struct V4L2Buffer {
  void *start;
  size_t length;
};

/* ---------------- CameraImage class ---------------- */
class M5Tab5CameraImage : public camera::CameraImage {
 public:
  M5Tab5CameraImage(uint8_t *data, size_t length, uint8_t requester);
  ~M5Tab5CameraImage() override;

  uint8_t *get_data_buffer() override;
  size_t get_data_length() override;
  bool was_requested_by(camera::CameraRequester requester) const override;

 protected:
  uint8_t *data_{nullptr};
  size_t length_{0};
  uint8_t requesters_{0};
};

/* ---------------- CameraImageReader class ---------------- */
class M5Tab5CameraImageReader : public camera::CameraImageReader {
 public:
  void set_image(std::shared_ptr<camera::CameraImage> image) override;
  size_t available() const override;
  uint8_t *peek_data_buffer() override;
  void consume_data(size_t consumed) override;
  void return_image() override;

 protected:
  std::shared_ptr<M5Tab5CameraImage> image_;
  size_t offset_{0};
};

/* ---------------- M5Tab5Camera class ---------------- */
class M5Tab5Camera : public camera::Camera {
 public:
  M5Tab5Camera();
  ~M5Tab5Camera() override;

  /* setters */
  void set_frame_size(M5Tab5CameraFrameSize size) { frame_size_ = size; }
  void set_jpeg_quality(uint8_t quality) { jpeg_quality_ = quality; }
  void set_framerate(uint8_t fps) { framerate_ = fps; }

  /* public API (derived) */
  void setup() override;
  void loop() override;
  void dump_config() override;

  /* public API (specific) */
  void start_stream(camera::CameraRequester requester) override;
  void stop_stream(camera::CameraRequester requester) override;
  void request_image(camera::CameraRequester requester) override;

  void add_listener(camera::CameraListener *listener) override { listeners_.push_back(listener); }
  camera::CameraImageReader *create_image_reader() override;

 protected:
  /* internals */
  bool init_camera_sensor_();
  uint8_t *capture_jpeg_frame_(size_t *out_len);
  void publish_frame_(uint8_t *data, size_t length);

  /* configuration */
  M5Tab5CameraFrameSize frame_size_{M5_TAB5_CAMERA_SIZE_800X600};
  uint8_t jpeg_quality_{12};
  uint8_t framerate_{10};

  /* runtime state */
  std::shared_ptr<M5Tab5CameraImage> current_image_;
  std::atomic<uint8_t> single_requesters_{0};
  std::atomic<uint8_t> stream_requesters_{0};
  std::vector<camera::CameraListener *> listeners_;
   uint32_t last_update_{0};
   uint32_t last_capture_error_log_{0};
   uint32_t frames_published_{0};
   uint32_t capture_failures_{0};
   bool camera_started_{false};

  /* V4L2 capture state */
  int camera_fd_{-1};
  V4L2Buffer *v4l2_buffers_{nullptr};
  uint32_t num_buffers_{0};
  int frame_width_{800};
  int frame_height_{600};
  bool sensor_hw_flip_{false};

  /* JPEG encoder */
  jpeg_encoder_handle_t jpeg_enc_{nullptr};
  uint8_t *jpeg_out_buf_{nullptr};
  uint32_t jpeg_out_buf_size_{0};
  uint8_t *rgb565_convert_buf_{nullptr};
};

}  // namespace esphome::m5_tab5_camera

#endif
