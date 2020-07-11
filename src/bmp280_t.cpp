
#include <bmp280_t.h>
#include <i2c/smbus.h>

Bmp280::Bmp280(int fd, uint8_t i2cAddress) {
  fd_ = fd;
  _i2cAddress = i2cAddress;

  ioctl(fd_, I2C_SLAVE, _i2cAddress);
  i2c_smbus_write_byte_data(fd_, 0xF4, 0xB7);
  i2c_smbus_write_byte_data(fd_, 0xF5, 0x10);

  // i2c_smbus_write_byte_data(fd_, 0xF4, 0x27);
  // i2c_smbus_write_byte_data(fd_, 0xF5, 0xA0);
}

void Bmp280::read() {

  ioctl(fd_, I2C_SLAVE, _i2cAddress);

  i2c_smbus_read_i2c_block_data(
      fd_, 0x88, sizeof(_raw_data.calib_data),
      reinterpret_cast<uint8_t *>(&_raw_data.calib_data));

  i2c_smbus_read_i2c_block_data(fd_, 0xF7, sizeof(_raw_data.data),
                                _raw_data.data);

    //Do not process dataa to save cpu
  
  // adcPres_ = ((static_cast<unsigned>(_raw_data.data[0]) << 16) +
  //             (static_cast<unsigned>(_raw_data.data[1]) << 8) +
  //             (_raw_data.data[2] & 0xF0)) >>
  //            4;
  // adcTemp_ = ((static_cast<unsigned>(_raw_data.data[3]) << 16) +
  //             (static_cast<unsigned>(_raw_data.data[4]) << 8) +
  //             (_raw_data.data[5] & 0xF0)) >>
  //            4;
}

double Bmp280::temperature() const {
  double var1((adcTemp_ / 16384.0L - _raw_data.calib_data.t1 / 1024.0L) *
              _raw_data.calib_data.t2);
  double var2(((adcTemp_ / 131072.0L - _raw_data.calib_data.t1 / 8192.0L) *
               (adcTemp_ / 131072.0L - _raw_data.calib_data.t1 / 8192.0L)) *
              _raw_data.calib_data.t3);

  return (var1 + var2) / 5120.0L;
}

double Bmp280::pressure() const {
  double var1((adcTemp_ / 16384.0L - _raw_data.calib_data.t1 / 1024.0L) *
              _raw_data.calib_data.t2);
  double var2(((adcTemp_ / 131072.0L - _raw_data.calib_data.t1 / 8192.0L) *
               (adcTemp_ / 131072.0L - _raw_data.calib_data.t1 / 8192.0L)) *
              _raw_data.calib_data.t3);

  var1 = ((var1 + var2) / 2.0L) - 64000.0L;
  var2 = var1 * var1 * _raw_data.calib_data.p6 / 32768.0L;
  var2 = var2 + var1 * _raw_data.calib_data.p5 * 2.0L;
  var2 = (var2 / 4.0L) + _raw_data.calib_data.p4 * 65536.0L;
  var1 = (_raw_data.calib_data.p3 * var1 * var1 / 524288.0L +
          _raw_data.calib_data.p2 * var1) /
         524288.0L;
  var1 = (1.0L + var1 / 32768.0L) * _raw_data.calib_data.p1;

  double p(1048576.0L - adcPres_);

  p = (p - (var2 / 4096.0)) * 6250.0 / var1;
  var1 = _raw_data.calib_data.p9 * p * p / 2147483648.0L;
  var2 = p * _raw_data.calib_data.p8 / 32768.0L;

  return (p + (var1 + var2 + _raw_data.calib_data.p7) / 16.0L) / 100.0L;
}
