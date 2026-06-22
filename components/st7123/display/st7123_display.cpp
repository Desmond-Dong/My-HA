#ifdef USE_ESP32_VARIANT_ESP32P4

#include <utility>

#include "esp_heap_caps.h"
#include "esphome/core/helpers.h"
#include "st7123_display.h"

namespace {
static constexpr size_t RGB565_LINE_BUF_ALIGN = 64;
static uint8_t *rgb565_line_swap_buf_{nullptr};
static size_t rgb565_line_swap_cap_{0};
static uint8_t *ensure_rgb565_line_buf(size_t line_bytes) {
  if (rgb565_line_swap_cap_ >= line_bytes && rgb565_line_swap_buf_ != nullptr) return rgb565_line_swap_buf_;
  if (rgb565_line_swap_buf_ != nullptr) {
    heap_caps_free(rgb565_line_swap_buf_);
    rgb565_line_swap_buf_ = nullptr;
    rgb565_line_swap_cap_ = 0;
  }
  rgb565_line_swap_buf_ = static_cast<uint8_t *>(heap_caps_aligned_alloc(RGB565_LINE_BUF_ALIGN, line_bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  if (rgb565_line_swap_buf_ != nullptr) {
    rgb565_line_swap_cap_ = line_bytes;
    return rgb565_line_swap_buf_;
  }
  rgb565_line_swap_buf_ = static_cast<uint8_t *>(heap_caps_aligned_alloc(RGB565_LINE_BUF_ALIGN, line_bytes, MALLOC_CAP_8BIT));
  if (rgb565_line_swap_buf_ != nullptr) rgb565_line_swap_cap_ = line_bytes;
  return rgb565_line_swap_buf_;
}

static bool notify_refresh_ready(esp_lcd_panel_handle_t panel, esp_lcd_dpi_panel_event_data_t *edata, void *user_ctx) {
  auto sem = static_cast<SemaphoreHandle_t>(user_ctx);
  BaseType_t need_yield = pdFALSE;
  xSemaphoreGiveFromISR(sem, &need_yield);
  return (need_yield == pdTRUE);
}
}  // namespace

namespace esphome {
namespace st7123 {

static const uint8_t CMD0_60[] = {0x71, 0x23, 0xA2};
static const uint8_t CMD1_60[] = {0x71, 0x23, 0xA3};
static const uint8_t CMD2_60[] = {0x71, 0x23, 0xA4};
static const uint8_t CMD_A4[] = {0x31};
static const uint8_t CMD_D7[] = {0x10, 0x0A, 0x10, 0x2A, 0x80, 0x80};
static const uint8_t CMD_90[] = {0x71, 0x23, 0x5A, 0x20, 0x24, 0x09, 0x09};
static const uint8_t CMD_A3[] = {0x80, 0x01, 0x88, 0x30, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x1E, 0x5C, 0x1E, 0x80, 0x00, 0x4F, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x1E, 0x5C, 0x1E, 0x80, 0x00, 0x6F, 0x58, 0x00, 0x00, 0x00, 0xFF};
static const uint8_t CMD_A6[] = {0x03, 0x00, 0x24, 0x55, 0x36, 0x00, 0x39, 0x00, 0x6E, 0x6E, 0x91, 0xFF, 0x00, 0x24, 0x55, 0x38, 0x00, 0x37, 0x00, 0x6E, 0x6E, 0x91, 0xFF, 0x00, 0x24, 0x11, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x6E, 0x91, 0xFF, 0x00, 0xEC, 0x11, 0x00, 0x03, 0x00, 0x03, 0x6E, 0x6E, 0xFF, 0xFF, 0x00, 0x08, 0x80, 0x08, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00};
static const uint8_t CMD_A7[] = {0x19, 0x19, 0x80, 0x64, 0x40, 0x07, 0x16, 0x40, 0x00, 0x44, 0x03, 0x6E, 0x6E, 0x91, 0xFF, 0x08, 0x80, 0x64, 0x40, 0x25, 0x34, 0x40, 0x00, 0x02, 0x01, 0x6E, 0x6E, 0x91, 0xFF, 0x08, 0x80, 0x64, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x6E, 0x6E, 0x91, 0xFF, 0x08, 0x80, 0x64, 0x40, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x6E, 0x6E, 0x84, 0xFF, 0x08, 0x80, 0x44};
static const uint8_t CMD_AC[] = {0x03, 0x19, 0x19, 0x18, 0x18, 0x06, 0x13, 0x13, 0x11, 0x11, 0x08, 0x08, 0x0A, 0x0A, 0x1C, 0x1C, 0x07, 0x07, 0x00, 0x00, 0x02, 0x02, 0x01, 0x19, 0x19, 0x18, 0x18, 0x06, 0x12, 0x12, 0x10, 0x10, 0x09, 0x09, 0x0B, 0x0B, 0x1C, 0x1C, 0x07, 0x07, 0x03, 0x03, 0x01, 0x01};
static const uint8_t CMD_AD[] = {0xF0, 0x00, 0x46, 0x00, 0x03, 0x50, 0x50, 0xFF, 0xFF, 0xF0, 0x40, 0x06, 0x01, 0x07, 0x42, 0x42, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
static const uint8_t CMD_AE[] = {0xFE, 0x3F, 0x3F, 0xFE, 0x3F, 0x3F, 0x00};
static const uint8_t CMD_B2[] = {0x15, 0x19, 0x05, 0x23, 0x49, 0xAF, 0x03, 0x2E, 0x5C, 0xD2, 0xFF, 0x10, 0x20, 0xFD, 0x20, 0xC0, 0x00};
static const uint8_t CMD_E8[] = {0x20, 0x6F, 0x04, 0x97, 0x97, 0x3E, 0x04, 0xDC, 0xDC, 0x3E, 0x06, 0xFA, 0x26, 0x3E};
static const uint8_t CMD_75[] = {0x03, 0x04};
static const uint8_t CMD_E7[] = {0x3B, 0x00, 0x00, 0x7C, 0xA1, 0x8C, 0x20, 0x1A, 0xF0, 0xB1, 0x50, 0x00, 0x50, 0xB1, 0x50, 0xB1, 0x50, 0xD8, 0x00, 0x55, 0x00, 0xB1, 0x00, 0x45, 0xC9, 0x6A, 0xFF, 0x5A, 0xD8, 0x18, 0x88, 0x15, 0xB1, 0x01, 0x01, 0x77};
static const uint8_t CMD_EA[] = {0x13, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x2C};
static const uint8_t CMD_B0[] = {0x22, 0x43, 0x11, 0x61, 0x25, 0x43, 0x43};
static const uint8_t CMD_B7[] = {0x00, 0x00, 0x73, 0x73};
static const uint8_t CMD_BF[] = {0xA6, 0xAA};
static const uint8_t CMD_A9[] = {0x00, 0x00, 0x73, 0xFF, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03};
static const uint8_t CMD_C8[] = {0x00, 0x00, 0x10, 0x1F, 0x36, 0x00, 0x5D, 0x04, 0x9D, 0x05, 0x10, 0xF2, 0x06, 0x60, 0x03, 0x11, 0xAD, 0x00, 0xEF, 0x01, 0x22, 0x2E, 0x0E, 0x74, 0x08, 0x32, 0xDC, 0x09, 0x33, 0x0F, 0xF3, 0x77, 0x0D, 0xB0, 0xDC, 0x03, 0xFF};
static const uint8_t CMD_C9[] = {0x00, 0x00, 0x10, 0x1F, 0x36, 0x00, 0x5D, 0x04, 0x9D, 0x05, 0x10, 0xF2, 0x06, 0x60, 0x03, 0x11, 0xAD, 0x00, 0xEF, 0x01, 0x22, 0x2E, 0x0E, 0x74, 0x08, 0x32, 0xDC, 0x09, 0x33, 0x0F, 0xF3, 0x77, 0x0D, 0xB0, 0xDC, 0x03, 0xFF};
static const uint8_t CMD_36[] = {0x03};

static const ST7123Display::InitCmd INIT_CMDS[] = {
    {0x60, CMD0_60, sizeof(CMD0_60), 0},
    {0x60, CMD1_60, sizeof(CMD1_60), 0},
    {0x60, CMD2_60, sizeof(CMD2_60), 0},
    {0xA4, CMD_A4, sizeof(CMD_A4), 0},
    {0xD7, CMD_D7, sizeof(CMD_D7), 0},
    {0x90, CMD_90, sizeof(CMD_90), 0},
    {0xA3, CMD_A3, sizeof(CMD_A3), 0},
    {0xA6, CMD_A6, sizeof(CMD_A6), 0},
    {0xA7, CMD_A7, sizeof(CMD_A7), 0},
    {0xAC, CMD_AC, sizeof(CMD_AC), 0},
    {0xAD, CMD_AD, sizeof(CMD_AD), 0},
    {0xAE, CMD_AE, sizeof(CMD_AE), 0},
    {0xB2, CMD_B2, sizeof(CMD_B2), 0},
    {0xE8, CMD_E8, sizeof(CMD_E8), 0},
    {0x75, CMD_75, sizeof(CMD_75), 0},
    {0xE7, CMD_E7, sizeof(CMD_E7), 0},
    {0xEA, CMD_EA, sizeof(CMD_EA), 0},
    {0xB0, CMD_B0, sizeof(CMD_B0), 0},
    {0xB7, CMD_B7, sizeof(CMD_B7), 0},
    {0xBF, CMD_BF, sizeof(CMD_BF), 0},
    {0xA9, CMD_A9, sizeof(CMD_A9), 0},
    {0xC8, CMD_C8, sizeof(CMD_C8), 0},
    {0xC9, CMD_C9, sizeof(CMD_C9), 0},
    {0x36, CMD_36, sizeof(CMD_36), 0},
    {0x11, nullptr, 0, 120},
    {0x29, nullptr, 0, 50},
};

void ST7123Display::smark_failed(const LogString *message, esp_err_t err) {
  ESP_LOGE(TAG, "%s: %s", LOG_STR_ARG(message), esp_err_to_name(err));
  this->mark_failed(message);
}

void ST7123Display::setup() {
  for (auto *pin : this->enable_pins_) {
    pin->setup();
    pin->digital_write(true);
  }
  delay(10);

  esp_lcd_dsi_bus_config_t bus_config = {.bus_id = 0, .num_data_lanes = this->lanes_, .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT, .lane_bit_rate_mbps = this->lane_bit_rate_};
  auto err = esp_lcd_new_dsi_bus(&bus_config, &this->bus_handle_);
  if (err != ESP_OK) return this->smark_failed(LOG_STR("lcd_new_dsi_bus failed"), err);

  esp_lcd_dbi_io_config_t dbi_config = {.virtual_channel = 0, .lcd_cmd_bits = 8, .lcd_param_bits = 8};
  err = esp_lcd_new_panel_io_dbi(this->bus_handle_, &dbi_config, &this->io_handle_);
  if (err != ESP_OK) return this->smark_failed(LOG_STR("new_panel_io_dbi failed"), err);

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
  auto color_format = this->color_depth_ == display::COLOR_BITNESS_888 ? LCD_COLOR_FMT_RGB888 : LCD_COLOR_FMT_RGB565;
  esp_lcd_dpi_panel_config_t dpi_config = {
      .virtual_channel = 0,
      .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,
      .dpi_clock_freq_mhz = this->pclk_frequency_,
      .in_color_format = color_format,
#else
  auto pixel_format = this->color_depth_ == display::COLOR_BITNESS_888 ? LCD_COLOR_PIXEL_FORMAT_RGB888 : LCD_COLOR_PIXEL_FORMAT_RGB565;
  esp_lcd_dpi_panel_config_t dpi_config = {
      .virtual_channel = 0,
      .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,
      .dpi_clock_freq_mhz = this->pclk_frequency_,
      .pixel_format = pixel_format,
#endif
      .num_fbs = 1,
      .video_timing = {.h_size = this->width_, .v_size = this->height_, .hsync_pulse_width = this->hsync_pulse_width_, .hsync_back_porch = this->hsync_back_porch_, .hsync_front_porch = this->hsync_front_porch_, .vsync_pulse_width = this->vsync_pulse_width_, .vsync_back_porch = this->vsync_back_porch_, .vsync_front_porch = this->vsync_front_porch_},
      .flags = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(6, 0, 0)
        .use_dma2d = true,
#endif
      }};

  err = esp_lcd_new_panel_dpi(this->bus_handle_, &dpi_config, &this->handle_);
  if (err != ESP_OK) return this->smark_failed(LOG_STR("esp_lcd_new_panel_dpi failed"), err);

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
  err = esp_lcd_dpi_panel_enable_dma2d(this->handle_);
  if (err != ESP_OK) return this->smark_failed(LOG_STR("esp_lcd_dpi_panel_enable_dma2d failed"), err);
#endif

  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->digital_write(true);
    delay(5);
    this->reset_pin_->digital_write(false);
    delay(10);
    this->reset_pin_->digital_write(true);
    delay(120);
  } else {
    err = esp_lcd_panel_io_tx_param(this->io_handle_, LCD_CMD_SWRESET, nullptr, 0);
    if (err != ESP_OK) return this->smark_failed(LOG_STR("lcd swreset failed"), err);
    delay(120);
  }

  for (const auto &cmd : INIT_CMDS) {
    err = esp_lcd_panel_io_tx_param(this->io_handle_, cmd.cmd, cmd.data, cmd.data_bytes);
    if (err != ESP_OK) return this->smark_failed(LOG_STR("lcd panel init cmd failed"), err);
    if (cmd.delay_ms > 0) delay(cmd.delay_ms);
    delay(5);
  }

  err = esp_lcd_panel_init(this->handle_);
  if (err != ESP_OK) return this->smark_failed(LOG_STR("esp_lcd_init failed"), err);
  err = esp_lcd_panel_invert_color(this->handle_, false);
  if (err != ESP_OK) return this->smark_failed(LOG_STR("esp_lcd_panel_invert_color failed"), err);
  err = esp_lcd_panel_mirror(this->handle_, false, false);
  if (err != ESP_OK) return this->smark_failed(LOG_STR("esp_lcd_panel_mirror failed"), err);

  this->io_lock_ = xSemaphoreCreateBinary();
  esp_lcd_dpi_panel_event_callbacks_t cbs = {.on_color_trans_done = notify_refresh_ready};
  err = esp_lcd_dpi_panel_register_event_callbacks(this->handle_, &cbs, this->io_lock_);
  if (err != ESP_OK) return this->smark_failed(LOG_STR("Failed to register callbacks"), err);
}

void ST7123Display::update() {
  if (this->auto_clear_enabled_) this->clear();
  if (this->show_test_card_) this->test_card();
  else if (this->page_ != nullptr) this->page_->get_writer()(*this);
  else if (this->writer_.has_value()) (*this->writer_)(*this);
  else this->stop_poller();
  if (this->buffer_ == nullptr || this->x_low_ > this->x_high_ || this->y_low_ > this->y_high_) return;
  int w = this->x_high_ - this->x_low_ + 1;
  int h = this->y_high_ - this->y_low_ + 1;
  this->write_to_display_(this->x_low_, this->y_low_, w, h, this->buffer_, this->x_low_, this->y_low_, this->width_ - w - this->x_low_);
  this->x_low_ = this->width_;
  this->y_low_ = this->height_;
  this->x_high_ = 0;
  this->y_high_ = 0;
}

void ST7123Display::draw_pixels_at(int x_start, int y_start, int w, int h, const uint8_t *ptr, display::ColorOrder order, display::ColorBitness bitness, bool big_endian, int x_offset, int y_offset, int x_pad) {
  if (w <= 0 || h <= 0) return;
  if (bitness != this->color_depth_) return display::Display::draw_pixels_at(x_start, y_start, w, h, ptr, order, bitness, big_endian, x_offset, y_offset, x_pad);
  this->write_to_display_(x_start, y_start, w, h, ptr, x_offset, y_offset, x_pad);
}

void ST7123Display::write_to_display_(int x_start, int y_start, int w, int h, const uint8_t *ptr, int x_offset, int y_offset, int x_pad) {
  esp_err_t err = ESP_OK;
  auto bytes_per_pixel = 3 - this->color_depth_;
  auto stride = (x_offset + w + x_pad) * bytes_per_pixel;
  ptr += y_offset * stride + x_offset * bytes_per_pixel;
  const uint8_t *const region_ptr = ptr;
  if (this->color_depth_ == display::COLOR_BITNESS_565) {
    const size_t line_bytes = static_cast<size_t>(w) * 2;
    uint8_t *const line_buf = ensure_rgb565_line_buf(line_bytes);
    if (line_buf != nullptr) {
      for (int y = 0; y != h; y++) {
        const uint8_t *src_row = region_ptr + y * stride;
        for (int col = 0; col < w; col++) {
          line_buf[col * 2 + 0] = src_row[col * 2 + 1];
          line_buf[col * 2 + 1] = src_row[col * 2 + 0];
        }
        err = esp_lcd_panel_draw_bitmap(this->handle_, x_start, y + y_start, x_start + w, y + y_start + 1, line_buf);
        if (err != ESP_OK) break;
        xSemaphoreTake(this->io_lock_, portMAX_DELAY);
      }
      if (err != ESP_OK) ESP_LOGE(TAG, "lcd_panel_draw_bitmap failed: %s", esp_err_to_name(err));
      return;
    }
  }
  ptr = region_ptr;
  if (x_offset == 0 && x_pad == 0) {
    err = esp_lcd_panel_draw_bitmap(this->handle_, x_start, y_start, x_start + w, y_start + h, ptr);
    xSemaphoreTake(this->io_lock_, portMAX_DELAY);
  } else {
    for (int y = 0; y != h; y++) {
      err = esp_lcd_panel_draw_bitmap(this->handle_, x_start, y + y_start, x_start + w, y + y_start + 1, ptr);
      if (err != ESP_OK) break;
      ptr += stride;
      xSemaphoreTake(this->io_lock_, portMAX_DELAY);
    }
  }
  if (err != ESP_OK) ESP_LOGE(TAG, "lcd_panel_draw_bitmap failed: %s", esp_err_to_name(err));
}

bool ST7123Display::check_buffer_() {
  if (this->is_failed()) return false;
  if (this->buffer_ != nullptr) return true;
  auto bytes_per_pixel = 3 - this->color_depth_;
  RAMAllocator<uint8_t> allocator;
  this->buffer_ = allocator.allocate(this->height_ * this->width_ * bytes_per_pixel);
  if (this->buffer_ == nullptr) {
    this->mark_failed(LOG_STR("Could not allocate buffer for display!"));
    return false;
  }
  return true;
}

void ST7123Display::draw_pixel_at(int x, int y, Color color) {
  if (!this->get_clipping().inside(x, y)) return;
  switch (this->rotation_) {
    case display::DISPLAY_ROTATION_90_DEGREES: std::swap(x, y); x = this->width_ - x - 1; break;
    case display::DISPLAY_ROTATION_180_DEGREES: x = this->width_ - x - 1; y = this->height_ - y - 1; break;
    case display::DISPLAY_ROTATION_270_DEGREES: std::swap(x, y); y = this->height_ - y - 1; break;
    default: break;
  }
  if (x >= this->get_width_internal() || x < 0 || y >= this->get_height_internal() || y < 0) return;
  if (!this->check_buffer_()) return;
  size_t pos = (y * this->width_) + x;
  if (this->color_depth_ == display::COLOR_BITNESS_565) {
    auto *ptr_16 = reinterpret_cast<uint16_t *>(this->buffer_);
    uint8_t hi = static_cast<uint8_t>(color.r & 0xF8) | (color.g >> 5);
    uint8_t lo = static_cast<uint8_t>((color.g & 0x1C) << 3) | (color.b >> 3);
    uint16_t new_color = lo | (hi << 8);
    if (ptr_16[pos] == new_color) return;
    ptr_16[pos] = new_color;
  }
  if (x < this->x_low_) this->x_low_ = x;
  if (y < this->y_low_) this->y_low_ = y;
  if (x > this->x_high_) this->x_high_ = x;
  if (y > this->y_high_) this->y_high_ = y;
}

void ST7123Display::fill(Color color) {
  if (!this->check_buffer_()) return;
  if (this->get_clipping().is_set()) return Display::fill(color);
  if (this->color_depth_ == display::COLOR_BITNESS_565) {
    auto *ptr_16 = reinterpret_cast<uint16_t *>(this->buffer_);
    uint8_t hi = static_cast<uint8_t>(color.r & 0xF8) | (color.g >> 5);
    uint8_t lo = static_cast<uint8_t>((color.g & 0x1C) << 3) | (color.b >> 3);
    uint16_t new_color = lo | (hi << 8);
    std::fill_n(ptr_16, this->width_ * this->height_, new_color);
  }
}

int ST7123Display::get_width() { return (this->rotation_ == display::DISPLAY_ROTATION_90_DEGREES || this->rotation_ == display::DISPLAY_ROTATION_270_DEGREES) ? this->get_height_internal() : this->get_width_internal(); }
int ST7123Display::get_height() { return (this->rotation_ == display::DISPLAY_ROTATION_90_DEGREES || this->rotation_ == display::DISPLAY_ROTATION_270_DEGREES) ? this->get_width_internal() : this->get_height_internal(); }

void ST7123Display::dump_config() {
  ESP_LOGCONFIG(TAG, "ST7123 Dedicated LCD\n  Width: %u\n  Height: %u\n  Rotation: %d degrees", this->width_, this->height_, this->rotation_);
  LOG_PIN("  Reset Pin ", this->reset_pin_);
}

}  // namespace st7123
}  // namespace esphome

#endif
