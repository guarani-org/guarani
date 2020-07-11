#include <i2c.h>

namespace gni {
i2c_t &i2c_t::get(void) noexcept {
  static i2c_t instance;
  return instance;
}
int i2c_t::initialize(const char *bus_node) noexcept {
  auto &bus = i2c_t::get()._bus;
  bus = open("/dev/i2c-1", O_RDWR);
  return bus;
}

int i2c_t::get_bus(void) noexcept { return i2c_t::get()._bus; }
bool i2c_t::select(int device) noexcept {
  return ioctl(i2c_t::get()._bus, I2C_SLAVE, device) > 0;
}
} // namespace gni