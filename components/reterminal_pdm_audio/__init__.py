import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["esp32"]

reterminal_pdm_audio_ns = cg.esphome_ns.namespace("reterminal_pdm_audio")
ReTerminalPDMAudio = reterminal_pdm_audio_ns.class_("ReTerminalPDMAudio", cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ReTerminalPDMAudio),
    }
).extend(cv.polling_component_schema("1s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
