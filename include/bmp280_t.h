#pragma once

#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#pragma pack(1)
struct bmp280_calib_t {
  uint16_t t1;
  int16_t t2;
  int16_t t3;
  uint16_t p1;
  int16_t p2;
  int16_t p3;
  int16_t p4;
  int16_t p5;
  int16_t p6;
  int16_t p7;
  int16_t p8;
  int16_t p9;
};

struct bmp280_raw_data_t {
  bmp280_calib_t calib_data;
  uint8_t data[8];
};
#pragma pop()

class Bmp280 {

public:
  Bmp280(void) = default;
  ~Bmp280(void) = default;
  bool initialize(int fd, uint8_t address) noexcept;
  uint8_t get_address(void) noexcept;
  void read() noexcept;
  double temperature() const;
  double pressure() const;

  bmp280_raw_data_t _raw_data;

protected:
  int fd_;

  unsigned adcTemp_;
  unsigned adcPres_;
  uint8_t _i2cAddress;
};
