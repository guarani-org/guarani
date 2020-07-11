
#include <baro_comm.h>
#include <bmp280.h>
#include <config_ublox.h>
#include <fstream>
#include <gps.h>
#include <linux/i2c-dev.h>
#include <lsm9ds1_reg.h>
#include <recorder.h>
#include <serial.h>
#include <sys/ioctl.h>
#include <thread>

void print_rslt(const char api_name[], int8_t rslt) {
  if (rslt != BMP280_OK) {
    printf("%s\t", api_name);
    if (rslt == BMP280_E_NULL_PTR) {
      printf("Error [%d] : Null pointer error\r\n", rslt);
    } else if (rslt == BMP280_E_COMM_FAIL) {
      printf("Error [%d] : Bus communication failed\r\n", rslt);
    } else if (rslt == BMP280_E_IMPLAUS_TEMP) {
      printf("Error [%d] : Invalid Temperature\r\n", rslt);
    } else if (rslt == BMP280_E_DEV_NOT_FOUND) {
      printf("Error [%d] : Device not found\r\n", rslt);
    } else {
      /* For more error codes refer "*_defs.h" */
      printf("Error [%d] : Unknown error code\r\n", rslt);
    }
  }
}

int main(int ac, char **av) {

  if (gni::i2c_t::initialize("/dev/i2c-1") < 0) {
    return -1;
  }
  int8_t rslt;
  struct bmp280_dev bmp;
  struct bmp280_config conf;
  struct bmp280_uncomp_data ucomp_data;
  uint32_t pres32 = 0, pres64 = 0;
  double pres = 0.0;
  int32_t temp32 = 0;
  double temp = 0.0;

  /* Map the delay function pointer with the function responsible for
   * implementing the delay */
  bmp.delay_ms = gni::delay_ms;

  /* Assign device I2C address based on the status of SDO pin (GND for
   * PRIMARY(0x76) & VDD for SECONDARY(0x77)) */
  bmp.dev_id = BMP280_I2C_ADDR_SEC;

  /* Select the interface mode as I2C */
  bmp.intf = BMP280_I2C_INTF;

  /* Map the I2C read & write function pointer with the functions responsible
   * for I2C bus transfer */
  bmp.read = gni::i2c_reg_read;
  bmp.write = gni::i2c_reg_write;

  /* To enable SPI interface: comment the above 4 lines and uncomment the below
   * 4 lines */

  /*
   * bmp.dev_id = 0;
   * bmp.read = spi_reg_read;
   * bmp.write = spi_reg_write;
   * bmp.intf = BMP280_SPI_INTF;
   */
  rslt = bmp280_init(&bmp);

  rslt = bmp280_get_config(&conf, &bmp);
  print_rslt(" bmp280_get_config status", rslt);

  /* configuring the temperature oversampling, filter coefficient and output
   * data rate */
  /* Overwrite the desired settings */
  conf.filter = BMP280_FILTER_COEFF_2;

  /* Pressure oversampling set at 4x */
  conf.os_pres = BMP280_OS_4X;

  /* Setting the output data rate as 1HZ(1000ms) */
  conf.odr = BMP280_ODR_1000_MS;
  rslt = bmp280_set_config(&conf, &bmp);
  rslt = bmp280_get_config(&conf, &bmp);
  print_rslt(" bmp280_set_config status", rslt);

  /* Always set the power mode after setting the configuration */
  rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);
  print_rslt(" bmp280_set_power_mode status", rslt);
  while (1) {
    /* Reading the raw data from sensor */
    rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);
    printf("Press: %d    Temp: %d\n", ucomp_data.uncomp_press,
           ucomp_data.uncomp_temp);

    // /* Getting the compensated pressure using 32 bit precision */
    // rslt = bmp280_get_comp_pres_32bit(&pres32, ucomp_data.uncomp_press,
    // &bmp);

    // /* Getting the compensated pressure using 64 bit precision */
    // rslt = bmp280_get_comp_pres_64bit(&pres64, ucomp_data.uncomp_press,
    // &bmp);

    // /* Getting the compensated pressure as floating point value */
    // rslt = bmp280_get_comp_pres_double(&pres, ucomp_data.uncomp_press, &bmp);
    // printf("UP: %ld, P32: %ld, P64: %ld, P64N: %ld, P: %f\r\n",
    //        ucomp_data.uncomp_press, pres32, pres64, pres64 / 256, pres);

    bmp.delay_ms(1000);
  }

  return 0;

  gni::packet_queue_t packets;

  gni::recorder_t rec(packets, 1024 * 10);
  gni::gps_t gps("/dev/ttyS0", B9600, O_RDWR | O_NOCTTY, packets, "gps.hex");

  if (!rec.initialize()) {
    return -1;
  }

  if (!gps.initialize()) {
    return -1;
  }
  gps.start();
  rec.start();

  gps.join();
  rec.join();

  return 0;
}
