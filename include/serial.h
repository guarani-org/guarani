#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

namespace gni {
class serial_t {
public:
  serial_t(void) = default;
  serial_t(std::string_view device, int baud_rate);
  ~serial_t(void) = default;

  bool open(int32_t mode) noexcept;
  bool close(void) noexcept;
  int write(uint8_t *buffer, uint32_t size) noexcept;
  int read(uint8_t &buffer, uint32_t bytes2read) noexcept;
  bool set_baudrate(int baudrate) noexcept;

private:
  int _fd;
  std::string _device;
  int _baud_rate;
  termios _oldtio;
  termios _newtio;
};
} // namespace gni
