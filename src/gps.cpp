#include <config_ublox.h>
#include <gps.h>
using namespace gni;

gps_t::gps_t(std::string_view serial_port, uint32_t baudrate, uint32_t mode,
             packet_queue_t &packets, std::string_view gps_hex_file)
    : _serial(serial_port, B9600), _packets(packets), _baudrate(baudrate),
      _serial_mode(mode), _gps_hex_file(gps_hex_file) {}

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

void gps_t::run(std::atomic_bool &stop_flag) noexcept {
  _buffer_pos = 0;
  while (!stop_flag) {
    _bytes_rcv = _serial.read(&_buffer[_buffer_pos], 250);
    if (_bytes_rcv > 0) {
      _buffer_pos += _bytes_rcv;
      if (_buffer_pos >= sizeof(_buffer)) {
        if (create_packet(_pkt, _buffer, sizeof(_buffer))) {
          _packets.push(_pkt);
        }
        _buffer_pos = 0;
      }
    }
  }
}