#include <i2c_aquisition.h>
#include <iostream>
namespace gni {
i2c_aquisition_t::i2c_aquisition_t(packet_queue_t &pkt_queue,
                                   const char *i2c_bus)
    : _pkt_queue(pkt_queue), _i2c_bus(0), _bus_name(i2c_bus) {}

void i2c_aquisition_t::run(std::atomic_bool &stop) noexcept {
  while (!stop) {

    ::ioctl(_i2c_bus, I2C_SLAVE, 0x77);
    _bmp.read();
    if (create_packet(_bmp_pkt, &_bmp._raw_data, sizeof(bmp280_raw_data_t))) {
      _pkt_queue.push(_bmp_pkt);
    }
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(35));
  }
}

bool i2c_aquisition_t::initialize(void) noexcept {
  if (!i2c_t::initialize(_bus_name.data())) {
    return false;
  }

  if (!_bmp.initialize(i2c_t::get_bus(), 0x77)) {
    return false;
  }
  _bmp_pkt.header.type = pkt_type::baro;

  return true;
}

} // namespace gni