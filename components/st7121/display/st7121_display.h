#pragma once

#ifdef USE_ESP32_VARIANT_ESP32P4

#include "esphome/components/display/display.h"
#include "esphome/core/application.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/log.h"

#include "esp_lcd_mipi_dsi.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

namespace esphome {
namespace st7121 {

constexpr static const char *const TAG = "display.st7121";

class ST7121Display : public display::Display {
 public:
  struct InitCmd {
    int cmd;
    const uint8_t *data;
    size_t data_bytes;
    unsigned int delay_ms;
  };

  ST7121Display(size_t width, size_t height, display::ColorBitness color_depth, uint8_t pixel_mode)
      : width_(width), height_(height), color_depth_(color_depth), pixel_mode_(pixel_mode) {}

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
  void set_invert_colors(bool invert_colors) { this->invert_colors_ = invert_colors; }
  void set_color_mode(display::ColorOrder color_mode) { this->color_mode_ = color_mode; }
  void set_reset_pin(GPIOPin *reset_pin) { this->reset_pin_ = reset_pin; }
  void set_enable_pins(std::vector<GPIOPin *> enable_pins) { this->enable_pins_ = std::move(enable_pins); }
  void set_pclk_frequency(float pclk_frequency) { this->pclk_frequency_ = pclk_frequency; }
  int get_width_internal() override { return this->width_; }
  int get_height_internal() override { return this->height_; }
  void set_hsync_back_porch(uint16_t v) { this->hsync_back_porch_ = v; }
  void set_hsync_front_porch(uint16_t v) { this->hsync_front_porch_ = v; }
  void set_hsync_pulse_width(uint16_t v) { this->hsync_pulse_width_ = v; }
  void set_vsync_pulse_width(uint16_t v) { this->vsync_pulse_width_ = v; }
  void set_vsync_back_porch(uint16_t v) { this->vsync_back_porch_ = v; }
  void set_vsync_front_porch(uint16_t v) { this->vsync_front_porch_ = v; }
  void set_lane_bit_rate(float v) { this->lane_bit_rate_ = v; }
  void set_lanes(uint8_t v) { this->lanes_ = v; }

  void update() override;
  void setup() override;
  void draw_pixels_at(int x_start, int y_start, int w, int h, const uint8_t *ptr, display::ColorOrder order,
                      display::ColorBitness bitness, bool big_endian, int x_offset, int y_offset, int x_pad) override;
  void draw_pixel_at(int x, int y, Color color) override;
  void fill(Color color) override;
  int get_width() override;
  int get_height() override;
  void dump_config() override;

 protected:
  void smark_failed(const LogString *message, esp_err_t err);
  void write_to_display_(int x_start, int y_start, int w, int h, const uint8_t *ptr, int x_offset, int y_offset,
                         int x_pad);
  bool check_buffer_();
  size_t get_bytes_per_pixel_() const;
  uint16_t pack_color_565_(uint8_t r, uint8_t g, uint8_t b) const;
  void pack_color_888_(uint8_t r, uint8_t g, uint8_t b, uint8_t *dst) const;
  static uint16_t swap_rgb565_rb_(uint16_t value);

  GPIOPin *reset_pin_{nullptr};
  std::vector<GPIOPin *> enable_pins_{};
  size_t width_{};
  size_t height_{};
  uint16_t hsync_pulse_width_ = 2;
  uint16_t hsync_back_porch_ = 40;
  uint16_t hsync_front_porch_ = 40;
  uint16_t vsync_pulse_width_ = 2;
  uint16_t vsync_back_porch_ = 4;
  uint16_t vsync_front_porch_ = 320;
  float pclk_frequency_ = 78;
  float lane_bit_rate_{1300};
  uint8_t lanes_{2};
  bool invert_colors_{};
  display::ColorOrder color_mode_{display::COLOR_ORDER_RGB};
  display::ColorBitness color_depth_;
  uint8_t pixel_mode_{};

  esp_lcd_panel_handle_t handle_{};
  esp_lcd_dsi_bus_handle_t bus_handle_{};
  esp_lcd_panel_io_handle_t io_handle_{};
  SemaphoreHandle_t io_lock_{};
  uint8_t *buffer_{nullptr};
  uint16_t x_low_{1};
  uint16_t y_low_{1};
  uint16_t x_high_{0};
  uint16_t y_high_{0};
};

}  // namespace st7121
}  // namespace esphome

#endif
