#pragma once
#include <packetbuffer.h>
#include <serial.h>
#include <thread.h>

namespace gni {
class gps_t : public thread_t {
public:
  gps_t(const char * serial_port, uint32_t baudrate, uint32_t mode,
        packetbuffer_t &packets, const char * gps_hex_file);
  ~gps_t(void) = default;

  bool set_baudrate(uint32_t baudrate) noexcept;
  bool initialize(void) noexcept;

protected:
  void run(void) noexcept override;

private:
  serial_t _serial;
  uint32_t _serial_mode;
  uint8_t _buffer[max_payload_sz];
  uint8_t _bytes_rcv;
  uint16_t _buffer_pos;
  packetbuffer_t &_packets;
  packet_t _pkt;
  std::string _gps_hex_file;
  uint32_t _baudrate;
};
} // namespace gni