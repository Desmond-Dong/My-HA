#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"

#ifdef USE_ESP32

#include <esp_http_server.h>
#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <cstdint>
#include <vector>

namespace esphome {
namespace rs485_bridge {

/// @brief RS-485 <-> Web passthrough bridge for the M5Stack Tab5.
///
/// Configures the ESP32-P4 UART in native RS485 half-duplex mode (the SIT3088
/// DE/RE pin is driven by the UART's RTS line) and exposes the bus through an
/// esp_http_server instance:
///   - WebSocket /rs485/ws (full-duplex raw bytes)
///   - POST /rs485/tx (raw bytes to the bus)
///   - GET  /rs485/rx (buffered bytes, drained by default)
///   - GET  /rs485/status (JSON config + counters)
///   - GET  /rs485/ (built-in WebSocket console page)
class Rs485Bridge : public Component {
 public:
  void set_tx_pin(int pin) { this->tx_pin_ = pin; }
  void set_rx_pin(int pin) { this->rx_pin_ = pin; }
  void set_dir_pin(int pin) { this->dir_pin_ = pin; }
  void set_uart_num(int num) { this->uart_num_ = static_cast<uart_port_t>(num); }
  void set_baud_rate(uint32_t baud) { this->baud_rate_ = baud; }
  void set_data_bits(uart_word_length_t bits) { this->data_bits_ = bits; }
  void set_stop_bits(uart_stop_bits_t bits) { this->stop_bits_ = bits; }
  void set_parity(uart_parity_t parity) { this->parity_ = parity; }
  void set_port(uint16_t port) { this->port_ = port; }
  void set_rx_buffer_size(size_t bytes) { this->rx_buffer_ = bytes; }
  void set_tx_buffer_size(size_t bytes) { this->tx_buffer_ = bytes; }
  void set_rx_hold_bytes(size_t bytes) { this->rx_hold_ = bytes; }

  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  void setup() override;
  void loop() override;
  void dump_config() override;

 protected:
  // --- UART ---
  int tx_pin_{20};
  int rx_pin_{21};
  int dir_pin_{34};
  uart_port_t uart_num_{UART_NUM_2};
  uint32_t baud_rate_{9600};
  uart_word_length_t data_bits_{UART_DATA_8_BITS};
  uart_stop_bits_t stop_bits_{UART_STOP_BITS_1};
  uart_parity_t parity_{UART_PARITY_DISABLE};
  size_t rx_buffer_{4096};
  size_t tx_buffer_{4096};

  // --- HTTP server ---
  uint16_t port_{8080};
  httpd_handle_t server_{nullptr};
  std::vector<int> ws_clients_{};
  SemaphoreHandle_t ws_mutex_{nullptr};

  // --- Buffered RX (REST GET /rs485/rx) ---
  std::vector<uint8_t> rx_ring_{};
  SemaphoreHandle_t rx_mutex_{nullptr};
  size_t rx_hold_{4096};

  // --- Counters ---
  uint32_t tx_total_{0};
  uint32_t rx_total_{0};
  uint32_t rx_waiting_{0};
  uint32_t last_ping_ms_{0};

  bool write_bytes_val(const uint8_t *data, size_t len);
  void push_rx(const uint8_t *data, size_t len);
  void ws_broadcast(const uint8_t *data, size_t len);
  void ws_add_client(int fd);
  void ws_remove_client(int fd);
  void ws_ping_all();

  // --- esp_http_server handlers (static, `this` via req->user_ctx) ---
  static esp_err_t handle_page(httpd_req_t *req);
  static esp_err_t handle_ws(httpd_req_t *req);
  static esp_err_t handle_status(httpd_req_t *req);
  static esp_err_t handle_tx(httpd_req_t *req);
  static esp_err_t handle_rx(httpd_req_t *req);

  void register_uri(const char *uri, httpd_method_t method,
                    esp_err_t (*handler)(httpd_req_t *), bool is_websocket);
};

}  // namespace rs485_bridge
}  // namespace esphome

#endif  // USE_ESP32