#include <config_ublox.h>
#include <filesystem>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

namespace fs = std::filesystem;
namespace gni {

uint32_t ublox_config::count_lines(std::ifstream &file) noexcept {
  file.clear();
  file.seekg(0, std::ios::beg);
  file.unsetf(std::ios_base::skipws);
  uint32_t nlines = std::count(std::istream_iterator<char>(file),
                               std::istream_iterator<char>(), '\n');
  file.clear();
  file.seekg(0, std::ios::beg);
  return nlines;
}

const uint8_t baud_rate_cmd[] = {
    0xb5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xd0,
    0x08, 0x00, 0x00, 0x00, 0xc2, 0x01, 0x00, 0x07, 0x00, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xc4, 0x96, 0xb5, 0x62, 0x06, 0x00, 0x01};

std::vector<uint8_t> ublox_config::hex2byte(std::string_view hex) noexcept {
  std::vector<uint8_t> bytes;

  for (unsigned int i = 0; i < hex.length(); i += 2) {
    auto byteString = std::string(hex.substr(i, 2));
    uint8_t byte = (uint8_t)strtol(byteString.data(), NULL, 16);
    bytes.push_back(byte);
  }

  return std::move(bytes);
}

void ublox_config::compute_progress(uint32_t total_lines,
                                     uint32_t actual_line) noexcept {
  if (total_lines > 0) {
    float progress = ((float)actual_line / (float)total_lines) * 100.0f;
    printf("Progress: %.2f\r", progress);
    fflush(stdout);
  }
}

bool ublox_config::configure(serial_t &serial,
                             std::string_view hex_file) noexcept {

  printf("Flashing GPS: %s\n", hex_file.data());
  if (!fs::exists(hex_file)) {
    std::cerr << "Hex file: " << hex_file << " does not exist!\n";
    return false;
  }

  std::ifstream gps_cfg =
      std::ifstream(hex_file.data(), std::ios::in | std::ios::binary);

  if (!gps_cfg.good() || !gps_cfg.is_open()) {
    std::cerr << "Hex file: " << hex_file << " not oppend properly.\n";
    return false;
  }

  uint32_t total_lines = count_lines(gps_cfg);
  uint32_t actual_line = 0;
  std::string line;
  while (std::getline(gps_cfg, line)) {
    auto bytes = hex2byte(line);
    for (auto &byte : bytes) {
      serial.write(&byte, 1);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    compute_progress(total_lines,++actual_line);
  }

  serial.write((uint8_t *)baud_rate_cmd, sizeof(baud_rate_cmd));
  printf("GPS Baudrate set to 115200\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  printf("Gps Flashing complete!\n");
  return true;
}

} // namespace gni