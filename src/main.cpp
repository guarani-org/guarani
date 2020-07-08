
#include <config_ublox.h>
#include <fstream>
#include <gps.h>
#include <recorder.h>
#include <serial.h>
#include <thread>

int main(int ac, char **av) {

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
