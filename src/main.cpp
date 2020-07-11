
#include <bmp280_t.h>
#include <i2c_fd.h>
#include <config_ublox.h>
#include <fstream>
#include <gps.h>
#include <lsm9ds1_reg.h>
#include <recorder.h>
#include <serial.h>
#include <thread>

int main(int ac, char **av) {

  if (gni::i2c_t::initialize("/dev/i2c-1") < 0) {
    return -1;
  }

  Bmp280 bmp(gni::i2c_t::get_bus(),0x77);

  while(1){
    bmp.read();
    printf("Temp: %.2f    Press: %.2f\n",bmp.temperature(),bmp.pressure());
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
