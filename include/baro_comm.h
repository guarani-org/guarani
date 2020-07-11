#pragma once
#include <cstdint>
#include <i2c.h>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>

namespace gni {

inline void delay_ms(uint32_t period_ms) noexcept {
  std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
}

inline int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr,
                            uint8_t *reg_data, uint16_t length) noexcept {
  auto bus = i2c_t::get_bus();

  if (::ioctl(bus, I2C_SLAVE, i2c_devices::BARO) == 0) {
    if (::write(bus, &reg_addr, 1) > 0) {
      if (::write(bus, reg_data, length) >= 0) {
        return 0;
      }
    }
  }
  return -1;
}
inline int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr,
                           uint8_t *reg_data, uint16_t length) noexcept {
  auto bus = i2c_t::get_bus();
  if (::ioctl(bus, I2C_SLAVE, i2c_devices::BARO) == 0) {
    if (::write(bus, &reg_addr, 1) > 0) {
      if (::read(bus, reg_data, length) >= 0) {
        return 0;
      }
    }
  }
  return -1;
}
} // namespace gni