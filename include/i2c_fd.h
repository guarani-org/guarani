#pragma once

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

namespace gni {
struct i2c_device {
  enum { BARO = 0x77, IMU = 0x6a, MAG = 0x1c };
};

class i2c_t {

public:
  i2c_t(const i2c_t &) = delete;
  i2c_t(i2c_t &&) = delete;

  static i2c_t &get(void) noexcept;
  static bool select(int device) noexcept;
  static int get_bus(void) noexcept;
  static int initialize(const char *bus_node) noexcept;

private:
  i2c_t(void) : _bus(-1) {}
  ~i2c_t(void) = default;

  int _bus;
};
} // namespace gni