#include <i2c_aquisition.h>
#include <iostream>
#include <lsm9ds1_t.h>
namespace gni {
i2c_aquisition_t::i2c_aquisition_t(packetbuffer_t &pkt_queue,
                                   const char *i2c_bus)
    : _i2c_bus(0), _bus_name(i2c_bus), _pkt_queue(pkt_queue) {}

void i2c_aquisition_t::run(void) noexcept {
  while (!_stop) {

    _bmp.read();
    _imu.read();

    if (_bmp._raw_data.validity) {
      if (create_packet(_bmp_pkt, &_bmp._raw_data, sizeof(bmp280_raw_data_t))) {
        _pkt_queue.put(_bmp_pkt);
      }
    }

    if (_imu.raw.validity > 0) {
      if (create_packet(_imu_pkt, &_imu.raw, sizeof(imu_raw_t))) {
        _pkt_queue.put(_imu_pkt);
      }
    }

    std::this_thread::yield();
  }
}

bool i2c_aquisition_t::initialize(void) noexcept {
  if (!i2c_t::initialize(_bus_name.data())) {
    return false;
  }

  if (!_bmp.initialize(i2c_t::get_bus(), 0x77)) {
    return false;
  }

  if (!_imu.initialize(i2c_t::get_bus(), i2c_device::MAG, i2c_device::IMU)) {
    return false;
  }
  _bmp_pkt.header.type = pkt_type::baro;
  _imu_pkt.header.type = pkt_type::imu;

  return true;
}

} // namespace gni