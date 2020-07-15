#include <config_ublox.h>
#include <gps.h>
#include <serial.h>
using namespace gni;

gps_t::gps_t(const char *serial_port, uint32_t baudrate, uint32_t mode,
             packetbuffer_t &packets, const char *gps_hex_file)
    : _serial(serial_port, B9600), _serial_mode(mode), _bytes_rcv(0),
      _buffer_pos(0), _packets(packets), _pkt({0}), _gps_hex_file(gps_hex_file),
      _baudrate(baudrate) {}

bool gps_t::set_baudrate(uint32_t baudrate) noexcept {
  _baudrate = _baudrate;
  return _serial.set_baudrate(_baudrate);
}

bool gps_t::initialize(void) noexcept {
  if (!_serial.close()) {
    return false;
  }
  if (!_serial.open(_serial_mode)) {
    return false;
  }

  ublox_config ubcfg;
  if (!ubcfg.configure(_serial, _gps_hex_file)) {
    return false;
  }

  if (!_serial.set_baudrate(B115200)) {
    return false;
  }
  _pkt.header.type = pkt_type::gps;
  return true;
}

void gps_t::run(void) noexcept {
  _buffer_pos = 0;
  while (!_stop) {

    _bytes_rcv = _serial.read(&_buffer[_buffer_pos], serial_char_rcv_count);
    if (_bytes_rcv > 0) {
      _buffer_pos += _bytes_rcv;
      if (_buffer_pos >= sizeof(_buffer) - 1) {
        if (create_packet(_pkt, _buffer, sizeof(_buffer))) {
          _packets.put(_pkt);
        }
        _buffer_pos = 0;
      }
    }
    std::this_thread::yield();
  }
}