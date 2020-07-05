#pragma once

#include <serial.h>
#include <vector>

namespace gni {
class ublox_config {
private:
  std::vector<uint8_t> hex2byte(std::string_view hex) noexcept;


public:
  ublox_config() = default;
  ~ublox_config() {}

  bool configure(serial_t &serial, std::string_view hex_file) noexcept;
};
} // namespace gni