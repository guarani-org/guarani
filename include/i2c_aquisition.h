#pragma once
#include <bmp280_t.h>
#include <i2c_fd.h>
#include <packet.h>
#include <thread.h>

namespace gni {

class i2c_aquisition_t : public thread_t {
public:
  i2c_aquisition_t(packet_queue_t &pkt_queue, const char *i2c_bus);
  ~i2c_aquisition_t(void) = default;
  bool initialize(void) noexcept;

protected:
  void run(std::atomic_bool &stop) noexcept override;

private:
  int _i2c_bus;
  std::string _bus_name;
  packet_queue_t &_pkt_queue;
  Bmp280 _bmp;
  packet_t _bmp_pkt;
};
} // namespace gni