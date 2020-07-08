#pragma once

#include <serial.h>
#include <vector>
#include <fstream>

namespace gni {
class ublox_config {
private:
  std::vector<uint8_t> hex2byte(std::string_view hex) noexcept;
  uint32_t count_lines(std::ifstream &file)noexcept;
  void compute_progress(uint32_t total_lines, uint32_t actual_line)noexcept;
public:
  ublox_config() = default;
  ~ublox_config() {}

  bool configure(serial_t &serial, std::string_view hex_file) noexcept;
};
} // namespace gni