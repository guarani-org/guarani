
#include <lsm9ds1_t.h>



namespace gni {


bool lsm9ds1_t::initialize(int fd, uint8_t mag_addres,
                           uint8_t imu_address) noexcept {
  _fd = fd;
  /* Initialize inertial sensors (IMU) driver interface */
  _dev_ctx_imu.write_reg = write_imu;
  _dev_ctx_imu.read_reg = read_imu;
  _dev_ctx_imu.handle = (void *)&_fd;

  /* Initialize magnetic sensors driver interface */
  _dev_ctx_mag.write_reg = write_mag;
  _dev_ctx_mag.read_reg = read_mag;
  _dev_ctx_mag.handle = (void *)&fd;

  delay_mag_imu(20);

  /* Check device ID */
  lsm9ds1_dev_id_get(&_dev_ctx_mag, &_dev_ctx_imu, &_whoamI);
  if (_whoamI.imu != LSM9DS1_IMU_ID || _whoamI.mag != LSM9DS1_MAG_ID) {

    return false;
  }

  /* Restore default configuration */
  lsm9ds1_dev_reset_set(&_dev_ctx_mag, &_dev_ctx_imu, PROPERTY_ENABLE);
  do {
    lsm9ds1_dev_reset_get(&_dev_ctx_mag, &_dev_ctx_imu, &_rst);
  } while (_rst);

  /* Enable Block Data Update */
  lsm9ds1_block_data_update_set(&_dev_ctx_mag, &_dev_ctx_imu, PROPERTY_ENABLE);

  /* Set full scale */
  lsm9ds1_xl_full_scale_set(&_dev_ctx_imu, LSM9DS1_4g);
  lsm9ds1_gy_full_scale_set(&_dev_ctx_imu, LSM9DS1_2000dps);
  lsm9ds1_mag_full_scale_set(&_dev_ctx_mag, LSM9DS1_16Ga);

  /* Configure filtering chain - See datasheet for filtering chain details */
  /* Accelerometer filtering chain */
  lsm9ds1_xl_filter_aalias_bandwidth_set(&_dev_ctx_imu, LSM9DS1_AUTO);
  lsm9ds1_xl_filter_lp_bandwidth_set(&_dev_ctx_imu, LSM9DS1_LP_ODR_DIV_400);
  lsm9ds1_xl_filter_out_path_set(&_dev_ctx_imu, LSM9DS1_LP_OUT);

  /* Gyroscope filtering chain */
  lsm9ds1_gy_filter_lp_bandwidth_set(&_dev_ctx_imu, LSM9DS1_LP_STRONG);
  lsm9ds1_gy_filter_hp_bandwidth_set(&_dev_ctx_imu, LSM9DS1_HP_LIGHT);
  lsm9ds1_gy_filter_out_path_set(&_dev_ctx_imu, LSM9DS1_LPF1_HPF_LPF2_OUT);

  /* Set Output Data Rate / Power mode */
  lsm9ds1_imu_data_rate_set(&_dev_ctx_imu, LSM9DS1_IMU_952Hz);
  lsm9ds1_mag_data_rate_set(&_dev_ctx_mag, LSM9DS1_MAG_UHP_155Hz);
  return true;
}
void lsm9ds1_t::read(void) noexcept {
  raw.validity = 0;
  
  lsm9ds1_dev_status_get(&_dev_ctx_mag, &_dev_ctx_imu, &_reg);

  if (_reg.status_imu.xlda && _reg.status_imu.gda) {
    memset(&raw.accel.u8bit, 0x00, 3 * sizeof(int16_t));
    memset(&raw.gyro.u8bit, 0x00, 3 * sizeof(int16_t));
    lsm9ds1_acceleration_raw_get(&_dev_ctx_imu, raw.accel.u8bit);
    lsm9ds1_angular_rate_raw_get(&_dev_ctx_imu, raw.gyro.u8bit);
    raw.validity |= 1 << 0;
  }
  if (_reg.status_mag.zyxda) {
    memset(&raw.mag.u8bit, 0x00, 3 * sizeof(int16_t));
    lsm9ds1_magnetic_raw_get(&_dev_ctx_mag, raw.mag.u8bit);
    raw.validity |= 1 << 1;
  }
}
} // namespace gni