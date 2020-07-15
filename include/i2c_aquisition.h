#pragma once
#include <bmp280_t.h>
#include <i2c_fd.h>
#include <lsm9ds1_t.h>
#include <packetbuffer.h>
#include <thread.h>
namespace gni {

class i2c_aquisition_t : public thread_t {
public:
  i2c_aquisition_t(packetbuffer_t &pkt_queue, const char *i2c_bus);
  ~i2c_aquisition_t(void) = default;
  bool initialize(void) noexcept;

protected:
  void run(void) noexcept override;

private:
  int _i2c_bus;
  std::string _bus_name;
  packetbuffer_t &_pkt_queue;
  Bmp280 _bmp;
  lsm9ds1_t _imu;

  packet_t _bmp_pkt;
  packet_t _imu_pkt;
};
} // namespace gni