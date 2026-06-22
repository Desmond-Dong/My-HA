import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, ICON_VOLUME_HIGH, STATE_CLASS_MEASUREMENT, UNIT_DECIBEL

DEPENDENCIES = ["esp32"]

reterminal_pdm_audio_ns = cg.esphome_ns.namespace("reterminal_pdm_audio")
ReTerminalPDMAudio = reterminal_pdm_audio_ns.class_("ReTerminalPDMAudio", cg.PollingComponent)

CONF_RMS = "rms"
CONF_PEAK = "peak"
CONF_MIC_POWER_PIN = "mic_power_pin"
CONF_MIC_CLOCK_PIN = "mic_clock_pin"
CONF_MIC_DATA_PIN = "mic_data_pin"
CONF_SAMPLE_RATE = "sample_rate"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ReTerminalPDMAudio),
        cv.Optional(CONF_MIC_POWER_PIN, default=38): cv.int_,
        cv.Optional(CONF_MIC_CLOCK_PIN, default=42): cv.int_,
        cv.Optional(CONF_MIC_DATA_PIN, default=41): cv.int_,
        cv.Optional(CONF_SAMPLE_RATE, default=16000): cv.int_,
        cv.Required(CONF_RMS): sensor.sensor_schema(
            unit_of_measurement=UNIT_DECIBEL,
            icon=ICON_VOLUME_HIGH,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Required(CONF_PEAK): sensor.sensor_schema(
            unit_of_measurement=UNIT_DECIBEL,
            icon=ICON_VOLUME_HIGH,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
).extend(cv.polling_component_schema("1s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_mic_power_pin(config[CONF_MIC_POWER_PIN]))
    cg.add(var.set_mic_clock_pin(config[CONF_MIC_CLOCK_PIN]))
    cg.add(var.set_mic_data_pin(config[CONF_MIC_DATA_PIN]))
    cg.add(var.set_sample_rate(config[CONF_SAMPLE_RATE]))

    rms = await sensor.new_sensor(config[CONF_RMS])
    cg.add(var.set_rms_sensor(rms))

    peak = await sensor.new_sensor(config[CONF_PEAK])
    cg.add(var.set_peak_sensor(peak))
