
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

void delay_ms(uint32_t period) noexcept {
  std::this_thread::sleep_for(std::chrono::milliseconds(period));
}

int main(int ac, char **av) {

  //open the bus
  auto bus = open("/dev/i2c-1", O_RDWR);
  if (bus < 0) {
    return -1;
  }

  // set the device to manipulate;
  ioctl(bus, I2C_SLAVE, 0x77);

  //set the address to read/write
  int addr = 0xd0;
  auto n = write(bus, &addr, 1);

  // read the address
  uint8_t chip_id = 0;
  n = read(bus, &chip_id, 1);

  int8_t rslt;
  struct bmp280_dev bmp;

  /* Map the delay function pointer with the function responsible for
   * implementing the delay */
  bmp.delay_ms = delay_ms;

  /* Assign device I2C address based on the status of SDO pin (GND for
   * PRIMARY(0x76) & VDD for SECONDARY(0x77)) */
  bmp.dev_id = BMP280_I2C_ADDR_SEC;

  /* Select the interface mode as I2C */
  bmp.intf = BMP280_I2C_INTF;

  /* Map the I2C read & write function pointer with the functions responsible
   * for I2C bus transfer */
  // bmp.read = rd_i2c;
  // bmp.write = wr_i2c;

  /* To enable SPI interface: comment the above 4 lines and uncomment the below
   * 4 lines */

  /*
   * bmp.dev_id = 0;
   * bmp.read = spi_reg_read;
   * bmp.write = spi_reg_write;
   * bmp.intf = BMP280_SPI_INTF;
   */
  rslt = bmp280_init(&bmp);

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
