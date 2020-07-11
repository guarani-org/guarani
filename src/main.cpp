
#include <gps.h>
#include <i2c_aquisition.h>
#include <recorder.h>

int main(int ac, char **av) {

  gni::packet_queue_t packets;

  gni::recorder_t rec(packets, 1024 * 1024);
  gni::gps_t gps("/dev/ttyS0", B9600, O_RDWR | O_NOCTTY, packets, "gps.hex");
  gni::i2c_aquisition_t i2c(packets, "/dev/i2c-1");

  if (!rec.initialize()) {
    return -1;
  }

  if (!gps.initialize()) {
    return -1;
  }

  if (!i2c.initialize()) {
    return -1;
  }

  i2c.start();
  gps.start();
  rec.start();

  i2c.join();
  gps.join();
  rec.join();

  return 0;
}
