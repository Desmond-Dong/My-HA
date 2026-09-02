"""Lyrics fetcher: HTTPS GET on a dedicated FreeRTOS task.

The stock http_request component performs requests on the main event loop
(see http_request.h: "The read helpers block the main event loop"), so a
lyrics lookup to lrclib.net froze LVGL, the api connection, the camera
loop and audio for seconds on every track change. This component moves
the blocking I/O into its own task and delivers the response body (or an
error) back on the main loop via on_result/on_error triggers.
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.const import CONF_ID, CONF_TRIGGER_ID
from esphome.components import esp32

DEPENDENCIES = ["network"]

CONF_ON_RESULT = "on_result"
CONF_ON_ERROR = "on_error"
CONF_URL = "url"

lyrics_fetcher_ns = cg.esphome_ns.namespace("lyrics_fetcher")
LyricsFetcher = lyrics_fetcher_ns.class_("LyricsFetcher", cg.Component)
RequestAction = lyrics_fetcher_ns.class_("RequestAction", automation.Action)
ResultTrigger = lyrics_fetcher_ns.class_("ResultTrigger", automation.Trigger.template(cg.std_string))
ErrorTrigger = lyrics_fetcher_ns.class_("ErrorTrigger", automation.Trigger.template(cg.std_string))


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(LyricsFetcher),
        cv.Optional(CONF_ON_RESULT): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ResultTrigger),
            }
        ),
        cv.Optional(CONF_ON_ERROR): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ErrorTrigger),
            }
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for conf in config.get(CONF_ON_RESULT, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
        await automation.build_automation(trigger, [(cg.std_string, "x")], conf)
        cg.add(var.set_result_trigger(trigger))

    for conf in config.get(CONF_ON_ERROR, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
        await automation.build_automation(trigger, [(cg.std_string, "x")], conf)
        cg.add(var.set_error_trigger(trigger))

    # esp_http_client is excluded from the ESPHome IDF build by default.
    esp32.include_builtin_idf_component("esp_http_client")


LYRICS_FETCHER_REQUEST_ACTION_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(LyricsFetcher),
        cv.Required(CONF_URL): cv.templatable(cv.string_strict),
    }
)


@automation.register_action(
    "lyrics_fetcher.request", RequestAction, LYRICS_FETCHER_REQUEST_ACTION_SCHEMA
)
async def lyrics_fetcher_request_to_code(config, action_id, template_arg, args):
    parent = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, parent)
    url = await cg.templatable(config[CONF_URL], args, cg.std_string)
    cg.add(var.set_url(url))
    return var
