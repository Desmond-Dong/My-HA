"""RS485 web passthrough bridge for the M5Stack Tab5.

Hardware (per M5Stack Tab5 official docs):
  - SIT3088 RS-485 transceiver, half-duplex
  - ESP32-P4: TX = GPIO20, RX = GPIO21, DIR (DE/RE) = GPIO34
  - The ESP32-P4 UART supports native RS485 half-duplex mode where the RTS
    line drives the transceiver's DE/RE pin automatically (no GPIO toggling).

Endpoints served by ESPHome's web_server component (same port, no extra
HTTP server instance):
  - REST       POST /rs485/tx    send raw bytes to the bus (body = raw bytes)
  - REST       GET  /rs485/rx    read buffered bytes ({n, hex}, ?raw=1, ?clear=0)
  - REST       GET  /rs485/status  configuration + counters (JSON)
  - Web page   GET  /rs485/      polling console page
Requires the `web_server` component (auto-loaded).
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = []
DEPENDENCIES = ["web_server_base"]
AUTO_LOAD = ["web_server"]

CONF_TX_PIN = "tx_pin"
CONF_RX_PIN = "rx_pin"
CONF_DIR_PIN = "dir_pin"
CONF_UART_NUM = "uart_num"
CONF_BAUD_RATE = "baud_rate"
CONF_DATA_BITS = "data_bits"
CONF_STOP_BITS = "stop_bits"
CONF_PARITY = "parity"
CONF_RX_BUFFER_SIZE = "rx_buffer_size"
CONF_TX_BUFFER_SIZE = "tx_buffer_size"
CONF_RX_HOLD_BYTES = "rx_hold_bytes"

rs485_ns = cg.esphome_ns.namespace("rs485_bridge")
Rs485Bridge = rs485_ns.class_("Rs485Bridge", cg.Component)

_DATA_BITS = {5: "UART_DATA_5_BITS", 6: "UART_DATA_6_BITS", 7: "UART_DATA_7_BITS", 8: "UART_DATA_8_BITS"}
_STOP_BITS = {1.0: "UART_STOP_BITS_1", 1.5: "UART_STOP_BITS_1_5", 2.0: "UART_STOP_BITS_2"}
_PARITY = {"NONE": "UART_PARITY_DISABLE", "EVEN": "UART_PARITY_EVEN", "ODD": "UART_PARITY_ODD"}


def _pin(value):
    if isinstance(value, int):
        return cv.int_range(min=0, max=54)(value)
    if isinstance(value, str):
        s = value.strip().upper().replace("GPIO", "")
        try:
            return _pin(int(s))
        except ValueError as ex:
            raise cv.Invalid(f"Invalid GPIO pin: {value}") from ex
    raise cv.Invalid(f"Invalid GPIO pin: {value}")


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Rs485Bridge),
            cv.Optional(CONF_TX_PIN, default=20): _pin,
            cv.Optional(CONF_RX_PIN, default=21): _pin,
            cv.Optional(CONF_DIR_PIN, default=34): _pin,
            cv.Optional(CONF_UART_NUM, default=2): cv.int_range(min=0, max=4),
            cv.Optional(CONF_BAUD_RATE, default=9600): cv.int_range(min=1200, max=3000000),
            cv.Optional(CONF_DATA_BITS, default=8): cv.one_of(5, 6, 7, 8),
            cv.Optional(CONF_STOP_BITS, default=1): cv.one_of(1.0, 1.5, 2.0),
            cv.Optional(CONF_PARITY, default="NONE"): cv.one_of("NONE", "EVEN", "ODD"),
            cv.Optional(CONF_RX_BUFFER_SIZE, default=4096): cv.int_range(min=256, max=16384),
            cv.Optional(CONF_TX_BUFFER_SIZE, default=4096): cv.int_range(min=256, max=16384),
            cv.Optional(CONF_RX_HOLD_BYTES, default=4096): cv.int_range(min=1024, max=65536),
        }
    ),
    cv.only_on_esp32,
    cv.only_with_framework("esp-idf"),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_tx_pin(config[CONF_TX_PIN]))
    cg.add(var.set_rx_pin(config[CONF_RX_PIN]))
    cg.add(var.set_dir_pin(config[CONF_DIR_PIN]))
    cg.add(var.set_uart_num(config[CONF_UART_NUM]))
    cg.add(var.set_baud_rate(config[CONF_BAUD_RATE]))
    cg.add(var.set_data_bits(cg.RawExpression(_DATA_BITS[config[CONF_DATA_BITS]])))
    cg.add(var.set_stop_bits(cg.RawExpression(_STOP_BITS[float(config[CONF_STOP_BITS])])))
    cg.add(var.set_parity(cg.RawExpression(_PARITY[config[CONF_PARITY]])))
    cg.add(var.set_rx_buffer_size(config[CONF_RX_BUFFER_SIZE]))
    cg.add(var.set_tx_buffer_size(config[CONF_TX_BUFFER_SIZE]))
    cg.add(var.set_rx_hold_bytes(config[CONF_RX_HOLD_BYTES]))