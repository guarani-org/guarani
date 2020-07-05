#include <config_ublox.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

namespace fs = std::filesystem;
namespace gni {

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

bool ublox_config::configure(serial_t &serial,
                             std::string_view hex_file) noexcept {

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

  std::string line;
  while (std::getline(gps_cfg, line)) {
    auto bytes = hex2byte(line);
    for (auto &byte : bytes) {
      serial.write(&byte, 1);
    }
    std::cout << "GPS Configuration written: " << line << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  serial.write((uint8_t*)baud_rate_cmd,sizeof(baud_rate_cmd));
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  return true;
} // namespace gni

} // namespace gni