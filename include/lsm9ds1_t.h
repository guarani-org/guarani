#pragma once

#include <lsm9ds1_reg.h>
#include <i2c/smbus.h>
#include <i2c_fd.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <thread>

namespace gni {

  inline int32_t write_imu(void *handle, uint8_t reg, uint8_t *bufp,
                         uint16_t len) noexcept {
  int fd = *(int *)handle;
  ::ioctl(fd, I2C_SLAVE, gni::i2c_device::IMU);
  if (i2c_smbus_write_i2c_block_data(fd, reg, len, bufp) >= 0) {
    return 0;
  }
  return -1;
}
inline int32_t read_imu(void *handle, uint8_t reg, uint8_t *bufp,
                        uint16_t len) noexcept {
  int fd = *(int *)handle;
  ::ioctl(fd, I2C_SLAVE, gni::i2c_device::IMU);
  if (i2c_smbus_read_i2c_block_data(fd, reg, len, bufp) >= 0) {
    return 0;
  }
  return -1;
}

inline int32_t write_mag(void *handle, uint8_t reg, uint8_t *bufp,
                         uint16_t len) noexcept {
  int fd = *(int *)handle;
  ::ioctl(fd, I2C_SLAVE, gni::i2c_device::MAG);
  if (i2c_smbus_write_i2c_block_data(fd, reg, len, bufp) >= 0) {
    return 0;
  }
  return -1;
}
inline int32_t read_mag(void *handle, uint8_t reg, uint8_t *bufp,
                        uint16_t len) noexcept {
  int fd = *(int *)handle;
  ::ioctl(fd, I2C_SLAVE, gni::i2c_device::MAG);
  if (i2c_smbus_read_i2c_block_data(fd, reg, len, bufp) >= 0) {
    return 0;
  }
  return -1;
}

inline void delay_mag_imu(uint32_t ms) noexcept {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#pragma pack(1)
union axis3bit16_t {
  int16_t i16bit[3];
  uint8_t u8bit[6];
};

struct imu_raw_t {
  uint8_t validity; // 0 = all invalid; 1 = accel & gyro valid; 2 = mag valid;
                    // 3= mag & gyro & accel valid
  axis3bit16_t accel;
  axis3bit16_t gyro;
  axis3bit16_t mag;
};
#pragma pop()

class lsm9ds1_t {
public:
  lsm9ds1_t(void) = default;
  ~lsm9ds1_t(void) = default;
  bool initialize(int fd, uint8_t mag_address, uint8_t imu_address) noexcept;
  void read(void) noexcept;
  bool self_test(void) noexcept;

  imu_raw_t raw;

private:
  int _fd;
  lsm9ds1_id_t _whoamI;
  lsm9ds1_status_t _reg;
  uint8_t _rst;
  stmdev_ctx_t _dev_ctx_imu;
  stmdev_ctx_t _dev_ctx_mag;
};
} // namespace gni