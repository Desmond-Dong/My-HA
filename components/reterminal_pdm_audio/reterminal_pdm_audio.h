#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#include <driver/i2s_common.h>
#include <driver/i2s_pdm.h>

namespace esphome {
namespace reterminal_pdm_audio {

class ReTerminalPDMAudio : public PollingComponent {
 public:
  void set_mic_power_pin(int pin) { this->mic_power_pin_ = pin; }
  void set_mic_clock_pin(int pin) { this->mic_clock_pin_ = pin; }
  void set_mic_data_pin(int pin) { this->mic_data_pin_ = pin; }
  void set_sample_rate(uint32_t sample_rate) { this->sample_rate_ = sample_rate; }
  void set_rms_sensor(sensor::Sensor *sensor) { this->rms_sensor_ = sensor; }
  void set_peak_sensor(sensor::Sensor *sensor) { this->peak_sensor_ = sensor; }

  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override;

 protected:
  bool init_mic_();
  bool read_samples_(float &rms_db, float &peak_db);

  int mic_power_pin_{38};
  int mic_clock_pin_{42};
  int mic_data_pin_{41};
  uint32_t sample_rate_{16000};
  sensor::Sensor *rms_sensor_{nullptr};
  sensor::Sensor *peak_sensor_{nullptr};
  i2s_chan_handle_t rx_handle_{nullptr};
  bool initialized_{false};
};

}  // namespace reterminal_pdm_audio
}  // namespace esphome
