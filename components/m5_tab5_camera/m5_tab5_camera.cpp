#include "m5_tab5_camera.h"

#ifdef USE_ESP32

#include "esphome/core/log.h"

#include <cstring>
#include <algorithm>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <esp_heap_caps.h>
#include <driver/jpeg_encode.h>
#include <driver/ledc.h>

// IDF private header for getting existing I2C bus handle (IDF v5.4+)
#include <esp_private/i2c_platform.h>
// MIPI-CSI+V4L2 video init (pulled by BSP espr::m5stack_tab5)
#include <esp_video_init.h>

// Fallback for V4L2 controls if not in headers
#ifndef V4L2_CID_HFLIP
#define V4L2_CID_HFLIP             (0x00980914)
#endif
#ifndef V4L2_CID_VFLIP
#define V4L2_CID_VFLIP             (0x00980915)
#endif

namespace esphome::m5_tab5_camera {

static const char *const TAG = "m5_tab5_camera";

static uint32_t esphome_quality_to_hw_quality(uint8_t quality) {
  // ESPHome camera quality uses 6(best)..63(worst).
  // ESP32-P4 hardware JPEG uses 1..100 with larger = better quality.
  uint32_t clamped = std::min<uint32_t>(63, std::max<uint32_t>(6, quality));
  return 95U - ((clamped - 6U) * 65U / 57U);
}

// ---------------------------------------------------------------------------
// M5Tab5CameraImage
// ---------------------------------------------------------------------------

M5Tab5CameraImage::M5Tab5CameraImage(uint8_t *data, size_t length, uint8_t requesters)
    : data_(data), length_(length), requesters_(requesters) {}

M5Tab5CameraImage::~M5Tab5CameraImage() {
  if (data_ != nullptr) {
    free(data_);
    data_ = nullptr;
  }
}

uint8_t *M5Tab5CameraImage::get_data_buffer() { return data_; }

size_t M5Tab5CameraImage::get_data_length() { return length_; }

bool M5Tab5CameraImage::was_requested_by(camera::CameraRequester requester) const {
  return (requesters_ & (1U << requester)) != 0;
}

// ---------------------------------------------------------------------------
// M5Tab5CameraImageReader
// ---------------------------------------------------------------------------

void M5Tab5CameraImageReader::set_image(std::shared_ptr<camera::CameraImage> image) {
  this->image_ = std::static_pointer_cast<M5Tab5CameraImage>(std::move(image));
  this->offset_ = 0;
}

size_t M5Tab5CameraImageReader::available() const {
  if (!this->image_) return 0;
  return this->image_->get_data_length() - this->offset_;
}

uint8_t *M5Tab5CameraImageReader::peek_data_buffer() {
  if (!this->image_) return nullptr;
  return this->image_->get_data_buffer() + this->offset_;
}

void M5Tab5CameraImageReader::consume_data(size_t consumed) { this->offset_ += consumed; }

void M5Tab5CameraImageReader::return_image() { this->image_.reset(); }

// ---------------------------------------------------------------------------
// M5Tab5Camera
// ---------------------------------------------------------------------------

M5Tab5Camera::M5Tab5Camera() : camera::Camera() {}

M5Tab5Camera::~M5Tab5Camera() = default;

void M5Tab5Camera::setup() {
  ESP_LOGCONFIG(TAG, "Setting up M5Stack Tab5 camera (SC2356 via MIPI-CSI)...");
  if (!init_camera_sensor_()) {
    ESP_LOGE(TAG, "Camera sensor init failed");
    this->mark_failed();
    return;
  }
  ESP_LOGCONFIG(TAG, "Camera initialised");
}

void M5Tab5Camera::loop() {
  if (!camera_started_) {
    return;
  }

  // Throttle to configured framerate.
  uint32_t now = millis();
  uint32_t min_interval = 1000U / std::max<uint32_t>(1, framerate_);
  if (now - last_update_ < min_interval) return;

  // Only capture when something is actually asking for a frame.
  uint8_t active = single_requesters_.load() | stream_requesters_.load();
  if (active == 0) return;

  size_t jpeg_len = 0;
  uint8_t *jpeg = capture_jpeg_frame_(&jpeg_len);
  if (jpeg == nullptr || jpeg_len == 0) {
    // Avoid flooding HA logs while the capture/encode path is stabilised.
    uint32_t now_ms = millis();
    if (now_ms - last_capture_error_log_ > 5000U) {
      ESP_LOGW(TAG, "Frame capture failed (no frame dequeued or JPEG encode failed)");
      last_capture_error_log_ = now_ms;
    }
    return;
  }

  last_update_ = now;
  publish_frame_(jpeg, jpeg_len);
}

void M5Tab5Camera::dump_config() {
  ESP_LOGCONFIG(TAG, "M5Stack Tab5 Camera:");
  ESP_LOGCONFIG(TAG, "  Frame size enum: %u", static_cast<uint32_t>(frame_size_));
  ESP_LOGCONFIG(TAG, "  JPEG quality: %u", jpeg_quality_);
  ESP_LOGCONFIG(TAG, "  Framerate: %u fps", framerate_);
}

camera::CameraImageReader *M5Tab5Camera::create_image_reader() {
  return new M5Tab5CameraImageReader();
}

void M5Tab5Camera::request_image(camera::CameraRequester requester) {
  single_requesters_.fetch_or(1U << requester);
}

void M5Tab5Camera::start_stream(camera::CameraRequester requester) {
  stream_requesters_.fetch_or(1U << requester);
}

void M5Tab5Camera::stop_stream(camera::CameraRequester requester) {
  stream_requesters_.fetch_and(~(1U << requester));
}

// ---------------------------------------------------------------------------
// Resolution mapping helper
// ---------------------------------------------------------------------------

static void get_frame_dimensions(M5Tab5CameraFrameSize fs, int *w, int *h) {
  switch (fs) {
    case M5_TAB5_CAMERA_SIZE_400X296:    *w = 400;  *h = 296;  break;
    case M5_TAB5_CAMERA_SIZE_640X480:    *w = 640;  *h = 480;  break;
    case M5_TAB5_CAMERA_SIZE_800X600:    *w = 800;  *h = 600;  break;
    case M5_TAB5_CAMERA_SIZE_1024X768:   *w = 1024; *h = 768;  break;
    case M5_TAB5_CAMERA_SIZE_1280X720:   *w = 1280; *h = 720;  break;
    case M5_TAB5_CAMERA_SIZE_1280X960:   *w = 1280; *h = 960;  break;
    case M5_TAB5_CAMERA_SIZE_1600X1200:  *w = 1600; *h = 1200; break;
    default: *w = 800; *h = 600; break;
  }
}

// ---------------------------------------------------------------------------
// Camera sensor init
// ---------------------------------------------------------------------------
// Follows BSP camera init pattern but avoids creating a second I2C bus:
//   1. Enable camera power via IO expander (ESPHome GPIO pin on pi4ioe1 pin 6)
//   2. Wait 100 ms for sensor to stabilise
//   3. Get ESPHome's I2C handle via i2c_master_get_bus_handle (IDF v5.4+)
//   4. Call esp_video_init() directly — NO bsp_i2c_init, NO bsp_feature_enable
//   5. Open V4L2 device, configure format, request buffers, start streaming

bool M5Tab5Camera::init_camera_sensor_() {
  get_frame_dimensions(frame_size_, &frame_width_, &frame_height_);

  // Camera power is handled by camera_power_enable switch (ALWAYS_ON).
  // Wait for sensor to fully power up and stabilise.
  vTaskDelay(pdMS_TO_TICKS(300));

  // Provide 24 MHz master clock to SC202CS via LEDC on GPIO36.
  // Without this clock the sensor PLL may produce wrong pixel timing,
  // affecting ISP colour processing.
  {
    const ledc_timer_config_t timer_conf = {
      .speed_mode      = LEDC_LOW_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_1_BIT,
      .timer_num       = LEDC_TIMER_0,
      .freq_hz         = 24000000,
      .clk_cfg         = LEDC_AUTO_CLK,
      .deconfigure     = false,
    };
    ledc_timer_config(&timer_conf);
    const ledc_channel_config_t ch_conf = {
      .gpio_num   = 36,
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .channel    = LEDC_CHANNEL_0,
      .intr_type  = LEDC_INTR_DISABLE,
      .timer_sel  = LEDC_TIMER_0,
      .duty       = 1,
      .hpoint     = 0,
      .sleep_mode = LEDC_SLEEP_MODE_KEEP_ALIVE,
    };
    ledc_channel_config(&ch_conf);
    ESP_LOGI(TAG, "Camera 24 MHz clock started on GPIO36 via LEDC");
  }
  vTaskDelay(pdMS_TO_TICKS(50));  // Let PLL stabilise before SCCB access

  // 3. Get ESPHome's I2C bus handle (ESPHome assigns HP ports from I2C_NUM_0)
  i2c_master_bus_handle_t i2c_handle = NULL;
  esp_err_t ret = i2c_master_get_bus_handle(0, &i2c_handle);
  if (ret != ESP_OK) {
    ESP_LOGW(TAG, "I2C handle not on port 0, trying port 1...");
    ret = i2c_master_get_bus_handle(1, &i2c_handle);
  }
  if (ret != ESP_OK || i2c_handle == NULL) {
    ESP_LOGE(TAG, "Cannot obtain I2C bus handle — camera init aborted");
    return false;
  }
  ESP_LOGI(TAG, "Got shared I2C handle — SCCB will reuse ESPHome's bus");

  // 4. Initialise MIPI-CSI + V4L2 bridge via esp_video_init
  const esp_video_init_csi_config_t csi_cfg = {
    .sccb_config = {
      .init_sccb = false,
      .i2c_handle = i2c_handle,
      .freq       = 400000,
    },
    .reset_pin = GPIO_NUM_NC,
    .pwdn_pin  = GPIO_NUM_NC,
  };
  esp_video_init_config_t cam_cfg = {};
  cam_cfg.csi = &csi_cfg;

  ret = esp_video_init(&cam_cfg);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "esp_video_init failed: %s", esp_err_to_name(ret));
    return false;
  }
  ESP_LOGI(TAG, "esp_video_init succeeded (MIPI-CSI + V4L2 bridge)");
  // Suppress ISP verbose debug logs that flood the console at ~3ms intervals.
  esp_log_level_set("esp_ipa_ian", ESP_LOG_WARN);
  esp_log_level_set("esp_ipa_awb", ESP_LOG_WARN);
  esp_log_level_set("esp_ipa_agc", ESP_LOG_WARN);
  esp_log_level_set("esp_ipa_adn", ESP_LOG_WARN);
  esp_log_level_set("esp_ipa_acc", ESP_LOG_WARN);
  esp_log_level_set("isp_task", ESP_LOG_WARN);
  // The SC202CS auto color-correction can compute a CCM channel > +4.0, which
  // the P4 ISP rejects. The ISP keeps the previous CCM (harmless), but logs the
  // rejection every frame -> blocking UART spam. Silence those tags like the
  // official espp/m5stack-tab5 BSP does.
  esp_log_level_set("ISP_CCM", ESP_LOG_NONE);
  esp_log_level_set("ISP", ESP_LOG_NONE);
  esp_log_level_set("isp_video", ESP_LOG_NONE);
  esp_log_level_set("esp_video", ESP_LOG_NONE);

  // 5. V4L2 capture setup
  // BSP_CAMERA_DEVICE = ESP_VIDEO_MIPI_CSI_DEVICE_NAME = "/dev/video0"
  camera_fd_ = open("/dev/video0", O_RDWR);
  if (camera_fd_ < 0) {
    ESP_LOGE(TAG, "Cannot open /dev/video0: %s", strerror(errno));
    return false;
  }
  ESP_LOGI(TAG, "Opened /dev/video0 (fd=%d)", camera_fd_);

  // Query capabilities
  {
    v4l2_capability cap;
    memset(&cap, 0, sizeof(cap));
    if (ioctl(camera_fd_, VIDIOC_QUERYCAP, &cap) < 0) {
      ESP_LOGW(TAG, "VIDIOC_QUERYCAP failed: %s", strerror(errno));
    } else {
      ESP_LOGI(TAG, "V4L2 driver: %s card: %s", cap.driver, cap.card);
    }
  }

  // Explicitly request RGB565 via S_FMT — this forces data through ISP debayer.
  // Using G_FMT (driver default) may give raw unprocessed data.
  v4l2_format fmt;
  memset(&fmt, 0, sizeof(fmt));
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = frame_width_;
  fmt.fmt.pix.height = frame_height_;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;
  fmt.fmt.pix.field = V4L2_FIELD_NONE;
  if (ioctl(camera_fd_, VIDIOC_S_FMT, &fmt) < 0) {
    ESP_LOGW(TAG, "VIDIOC_S_FMT RGB565 %dx%d failed: %s", frame_width_, frame_height_, strerror(errno));
    // Try native sensor resolution (1280x720) as fallback.
    fmt.fmt.pix.width = 1280;
    fmt.fmt.pix.height = 720;
    if (ioctl(camera_fd_, VIDIOC_S_FMT, &fmt) < 0) {
      ESP_LOGW(TAG, "VIDIOC_S_FMT 1280x720 also failed, falling back to G_FMT: %s", strerror(errno));
      memset(&fmt, 0, sizeof(fmt));
      fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      if (ioctl(camera_fd_, VIDIOC_G_FMT, &fmt) < 0) {
        ESP_LOGE(TAG, "VIDIOC_G_FMT also failed: %s", strerror(errno));
        return false;
      }
    }
  }
  frame_width_  = (int)fmt.fmt.pix.width;
  frame_height_ = (int)fmt.fmt.pix.height;
  {
    uint32_t pf = fmt.fmt.pix.pixelformat;
    char fourcc[5] = {(char)(pf&0xFF),(char)((pf>>8)&0xFF),(char)((pf>>16)&0xFF),(char)((pf>>24)&0xFF),0};
    ESP_LOGI(TAG, "Driver format: %dx%d %s stride=%d", frame_width_, frame_height_, fourcc, fmt.fmt.pix.bytesperline);
  }

  // Try hardware-level flip to avoid software rotation (non-fatal if unsupported).
  {
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_HFLIP;
    ctrl.value = 1;
    if (ioctl(camera_fd_, VIDIOC_S_CTRL, &ctrl) == 0) {
      ctrl.id = V4L2_CID_VFLIP;
      ctrl.value = 1;
      if (ioctl(camera_fd_, VIDIOC_S_CTRL, &ctrl) == 0) {
        ESP_LOGI(TAG, "Sensor flip enabled via V4L2 - skipping software rotation");
        sensor_hw_flip_ = true;
      }
    }
    if (!sensor_hw_flip_) {
      ESP_LOGI(TAG, "V4L2 flip not supported, will rotate in software");
    }
  }

  // Request buffers
  v4l2_requestbuffers req;
  memset(&req, 0, sizeof(req));
  req.count  = 4;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  if (ioctl(camera_fd_, VIDIOC_REQBUFS, &req) < 0) {
    ESP_LOGE(TAG, "VIDIOC_REQBUFS failed: %s", strerror(errno));
    return false;
  }
  num_buffers_ = req.count;
  ESP_LOGI(TAG, "V4L2 buffers granted: %u", num_buffers_);

  v4l2_buffers_ = new V4L2Buffer[num_buffers_];
  for (uint32_t i = 0; i < num_buffers_; i++) {
    v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index  = i;

    if (ioctl(camera_fd_, VIDIOC_QUERYBUF, &buf) < 0) {
      ESP_LOGE(TAG, "VIDIOC_QUERYBUF[%u] failed: %s", i, strerror(errno));
      return false;
    }
    v4l2_buffers_[i].length = buf.length;
    v4l2_buffers_[i].start  = mmap(nullptr, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camera_fd_, buf.m.offset);
    if (v4l2_buffers_[i].start == MAP_FAILED) {
      ESP_LOGE(TAG, "mmap[%u] failed: %s", i, strerror(errno));
      return false;
    }

    if (ioctl(camera_fd_, VIDIOC_QBUF, &buf) < 0) {
      ESP_LOGE(TAG, "VIDIOC_QBUF[%u] failed: %s", i, strerror(errno));
      return false;
    }
  }

  v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(camera_fd_, VIDIOC_STREAMON, &type) < 0) {
    ESP_LOGE(TAG, "VIDIOC_STREAMON failed: %s", strerror(errno));
    return false;
  }
  ESP_LOGI(TAG, "V4L2 streaming started (%u buffers)", num_buffers_);

  // 6. Init hardware JPEG encoder
  {
    jpeg_encode_engine_cfg_t enc_cfg = {.intr_priority = 0, .timeout_ms = 40};
    ret = jpeg_new_encoder_engine(&enc_cfg, &jpeg_enc_);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "jpeg_new_encoder_engine failed: %s", esp_err_to_name(ret));
      return false;
    }
    // Allocate a PSRAM-backed output buffer close to raw frame size to avoid
    // truncation warnings from the hardware encoder (YUV422 is ~2/3 of raw).
    jpeg_out_buf_size_ = std::max<uint32_t>(262144, (uint32_t)(frame_width_ * frame_height_ * 2));
    size_t allocated_size = 0;
    jpeg_encode_memory_alloc_cfg_t mem_cfg = {
      .buffer_direction = JPEG_ENC_ALLOC_OUTPUT_BUFFER,
    };
    jpeg_out_buf_ = (uint8_t *)jpeg_alloc_encoder_mem(jpeg_out_buf_size_, &mem_cfg, &allocated_size);
    jpeg_out_buf_size_ = (uint32_t)allocated_size;
    if (jpeg_out_buf_ == nullptr) {
      ESP_LOGE(TAG, "malloc JPEG outbuf (%u bytes) failed", jpeg_out_buf_size_);
      return false;
    }
    if (!sensor_hw_flip_) {
      rgb565_convert_buf_ = (uint8_t *)heap_caps_malloc((size_t)frame_width_ * (size_t)frame_height_ * 2U,
                                                        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      if (rgb565_convert_buf_ == nullptr) {
        ESP_LOGE(TAG, "malloc RGB565 convert buf failed");
        return false;
      }
    }
    ESP_LOGI(TAG, "JPEG encoder ready, outbuf=%u bytes", jpeg_out_buf_size_);
  }

  camera_started_ = true;
  ESP_LOGI(TAG, "Camera sensor initialised and streaming at %dx%d", frame_width_, frame_height_);
  return true;
}

// ---------------------------------------------------------------------------
// Capture + JPEG encode
// ---------------------------------------------------------------------------

uint8_t *M5Tab5Camera::capture_jpeg_frame_(size_t *out_len) {
  *out_len = 0;
  if (!camera_started_ || camera_fd_ < 0) {
    return nullptr;
  }

  v4l2_buffer buf;
  memset(&buf, 0, sizeof(buf));
  buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;

  if (ioctl(camera_fd_, VIDIOC_DQBUF, &buf) < 0) {
    uint32_t now = millis();
    if (now - last_capture_error_log_ > 5000U) {
      ESP_LOGW(TAG, "VIDIOC_DQBUF failed: %s", strerror(errno));
      last_capture_error_log_ = now;
    }
    return nullptr;
  }

  uint8_t *raw = (uint8_t *)v4l2_buffers_[buf.index].start;
  uint32_t raw_len = buf.bytesused;
  uint32_t jpeg_len = 0;

  if (raw == nullptr || raw_len == 0) {
    uint32_t now = millis();
    if (now - last_capture_error_log_ > 5000U) {
      ESP_LOGW(TAG, "Dequeued empty frame: index=%u bytesused=%u", buf.index, raw_len);
      last_capture_error_log_ = now;
    }
    ioctl(camera_fd_, VIDIOC_QBUF, &buf);
    return nullptr;
  }

  // Rotate 180° to match UI orientation.
  uint8_t *enc_src;
  uint32_t enc_src_len;
  if (sensor_hw_flip_) {
    enc_src    = raw;
    enc_src_len = raw_len;
  } else {
    // Row-wise reverse copy — ~10x faster than pixel-by-pixel loop.
    uint16_t *src16 = (uint16_t *)raw;
    uint16_t *dst16 = (uint16_t *)rgb565_convert_buf_;
    for (int y = 0; y < frame_height_; y++) {
      uint16_t *row_src = src16 + y * frame_width_;
      uint16_t *row_dst = dst16 + (frame_height_ - 1 - y) * frame_width_;
      std::reverse_copy(row_src, row_src + frame_width_, row_dst);
    }
    enc_src    = rgb565_convert_buf_;
    enc_src_len = (uint32_t)frame_width_ * (uint32_t)frame_height_ * 2U;
  }

  jpeg_encode_cfg_t enc_cfg = {};
  enc_cfg.height        = (uint32_t)frame_height_;
  enc_cfg.width         = (uint32_t)frame_width_;
  enc_cfg.src_type      = JPEG_ENCODE_IN_FORMAT_RGB565;
  enc_cfg.sub_sample    = JPEG_DOWN_SAMPLING_YUV422;
  enc_cfg.image_quality = esphome_quality_to_hw_quality(jpeg_quality_);

  esp_err_t ret = jpeg_encoder_process(jpeg_enc_, &enc_cfg, enc_src, enc_src_len,
                                       jpeg_out_buf_, jpeg_out_buf_size_, &jpeg_len);

  // Re-queue immediately
  ioctl(camera_fd_, VIDIOC_QBUF, &buf);

  if (ret != ESP_OK) {
    uint32_t now = millis();
    if (now - last_capture_error_log_ > 5000U) {
      ESP_LOGW(TAG, "jpeg_encoder_process failed: ret=%s raw_len=%u outbuf=%u %dx%d",
               esp_err_to_name(ret), raw_len, jpeg_out_buf_size_, frame_width_, frame_height_);
      last_capture_error_log_ = now;
    }
    return nullptr;
  }

  if (jpeg_len == 0) {
    uint32_t now = millis();
    if (now - last_capture_error_log_ > 5000U) {
      ESP_LOGW(TAG, "jpeg_encoder_process returned zero-length JPEG: raw_len=%u %dx%d",
               raw_len, frame_width_, frame_height_);
      last_capture_error_log_ = now;
    }
    return nullptr;
  }

  uint8_t *out = (uint8_t *)heap_caps_malloc(jpeg_len, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (out == nullptr) {
    uint32_t now = millis();
    if (now - last_capture_error_log_ > 5000U) {
      ESP_LOGW(TAG, "heap_caps_malloc failed for output JPEG copy: len=%u", jpeg_len);
      last_capture_error_log_ = now;
    }
    return nullptr;
  }
  memcpy(out, jpeg_out_buf_, jpeg_len);
  *out_len = jpeg_len;
  return out;
}

void M5Tab5Camera::publish_frame_(uint8_t *data, size_t length) {
  uint8_t requesters = single_requesters_.load() | stream_requesters_.load();
  current_image_ = std::make_shared<M5Tab5CameraImage>(data, length, requesters);

  for (auto *listener : listeners_) {
    listener->on_camera_image(current_image_);
  }
  single_requesters_ = 0;
}

}  // namespace esphome::m5_tab5_camera

#endif  // USE_ESP32
