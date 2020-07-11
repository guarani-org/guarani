#pragma once

#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

class Bmp280 {

public:
  Bmp280(int fd, uint8_t i2cAddress);

  void read();
  double temperature() const;
  double pressure() const;

protected:
  int fd_;
  bmp280_raw_data_t _raw_data;
  unsigned adcTemp_;
  unsigned adcPres_;
  uint8_t _i2cAddress;
};
