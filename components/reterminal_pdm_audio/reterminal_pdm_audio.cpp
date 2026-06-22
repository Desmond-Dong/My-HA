#include "reterminal_pdm_audio.h"

#include "esphome/core/log.h"

#include <Arduino.h>
#include <cmath>

namespace esphome {
namespace reterminal_pdm_audio {

static const char *const TAG = "reterminal_pdm_audio";
static constexpr size_t DMA_BUF_COUNT = 8;
static constexpr size_t DMA_BUF_LEN = 512;
static constexpr size_t BITS_PER_SAMPLE = 16;
static constexpr size_t READ_BUF_BYTES = DMA_BUF_LEN * (BITS_PER_SAMPLE / 8);

void ReTerminalPDMAudio::setup() {
  this->initialized_ = this->init_mic_();
  if (!this->initialized_) {
    ESP_LOGE(TAG, "Microphone init failed");
    this->mark_failed();
  }
}

void ReTerminalPDMAudio::update() {
  if (!this->initialized_)
    return;

  float rms_db = NAN;
  float peak_db = NAN;
  if (!this->read_samples_(rms_db, peak_db)) {
    ESP_LOGW(TAG, "Failed to read microphone samples");
    return;
  }

  if (this->rms_sensor_ != nullptr)
    this->rms_sensor_->publish_state(rms_db);
  if (this->peak_sensor_ != nullptr)
    this->peak_sensor_->publish_state(peak_db);
}

void ReTerminalPDMAudio::dump_config() {
  ESP_LOGCONFIG(TAG, "reTerminal E1001 PDM Audio");
  ESP_LOGCONFIG(TAG, "  Mic power pin: GPIO%d", this->mic_power_pin_);
  ESP_LOGCONFIG(TAG, "  Mic clock pin: GPIO%d", this->mic_clock_pin_);
  ESP_LOGCONFIG(TAG, "  Mic data pin: GPIO%d", this->mic_data_pin_);
  ESP_LOGCONFIG(TAG, "  Sample rate: %u", this->sample_rate_);
  ESP_LOGCONFIG(TAG, "  Initialized: %s", this->initialized_ ? "yes" : "no");
}

float ReTerminalPDMAudio::get_setup_priority() const { return setup_priority::DATA; }

bool ReTerminalPDMAudio::init_mic_() {
  pinMode(this->mic_power_pin_, OUTPUT);
  digitalWrite(this->mic_power_pin_, HIGH);
  delay(50);

  i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
  chan_cfg.dma_desc_num = DMA_BUF_COUNT;
  chan_cfg.dma_frame_num = DMA_BUF_LEN;
  chan_cfg.auto_clear = true;

  esp_err_t err = i2s_new_channel(&chan_cfg, nullptr, &this->rx_handle_);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "i2s_new_channel failed: 0x%x", err);
    return false;
  }

  i2s_pdm_rx_config_t pdm_cfg = {};
  pdm_cfg.clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(this->sample_rate_);
  pdm_cfg.slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO);
  pdm_cfg.gpio_cfg.clk = static_cast<gpio_num_t>(this->mic_clock_pin_);
  pdm_cfg.gpio_cfg.din = static_cast<gpio_num_t>(this->mic_data_pin_);
  pdm_cfg.gpio_cfg.invert_flags.clk_inv = false;

  err = i2s_channel_init_pdm_rx_mode(this->rx_handle_, &pdm_cfg);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "i2s_channel_init_pdm_rx_mode failed: 0x%x", err);
    i2s_del_channel(this->rx_handle_);
    this->rx_handle_ = nullptr;
    return false;
  }

  err = i2s_channel_enable(this->rx_handle_);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "i2s_channel_enable failed: 0x%x", err);
    i2s_del_channel(this->rx_handle_);
    this->rx_handle_ = nullptr;
    return false;
  }

  uint8_t warmup[READ_BUF_BYTES];
  size_t bytes_read = 0;
  for (int i = 0; i < 3; i++) {
    i2s_channel_read(this->rx_handle_, warmup, sizeof(warmup), &bytes_read, pdMS_TO_TICKS(500));
  }

  ESP_LOGI(TAG, "PDM microphone initialized via official PDM-RX path");
  return true;
}

bool ReTerminalPDMAudio::read_samples_(float &rms_db, float &peak_db) {
  if (this->rx_handle_ == nullptr)
    return false;

  uint8_t buffer[READ_BUF_BYTES];
  size_t bytes_read = 0;
  esp_err_t err = i2s_channel_read(this->rx_handle_, buffer, sizeof(buffer), &bytes_read, pdMS_TO_TICKS(200));
  if (err != ESP_OK || bytes_read == 0) {
    return false;
  }

  auto *samples = reinterpret_cast<int16_t *>(buffer);
  const size_t sample_count = bytes_read / sizeof(int16_t);
  if (sample_count == 0) {
    return false;
  }

  double sum_sq = 0.0;
  int16_t peak = 0;
  for (size_t i = 0; i < sample_count; i++) {
    const int16_t sample = samples[i];
    sum_sq += static_cast<double>(sample) * static_cast<double>(sample);
    const int16_t abs_sample = sample == INT16_MIN ? INT16_MAX : std::abs(sample);
    if (abs_sample > peak) {
      peak = abs_sample;
    }
  }

  const double rms = std::sqrt(sum_sq / sample_count);
  const double peak_ratio = std::max(peak / 32767.0, 1e-9);
  const double rms_ratio = std::max(rms / 32767.0, 1e-9);
  peak_db = static_cast<float>(20.0 * std::log10(peak_ratio));
  rms_db = static_cast<float>(20.0 * std::log10(rms_ratio));
  return true;
}

}  // namespace reterminal_pdm_audio
}  // namespace esphome
