
#include <cstdio>
#include <gps.h>
#include <i2c_aquisition.h>
#include <memory>

#include <packetbuffer.h>
#include <recorder.h>

int main(int ac, char **av) {

  gni::packetbuffer_t pktbuff;

  auto rec = std::make_unique<gni::recorder_t>(pktbuff, 1024 * 1024);
  auto gps = std::make_unique<gni::gps_t>(
      "/dev/ttyS0", B9600, O_RDWR | O_NOCTTY, pktbuff, "gps.hex");

  auto i2c = std::make_unique<gni::i2c_aquisition_t>(pktbuff, "/dev/i2c-1");

  if (!rec->initialize()) {
    return -1;
  }

  if (!gps->initialize()) {
    return -1;
  }

  if (!i2c->initialize()) {
    return -1;
  }

  rec->start();
  gps->start();
  i2c->start();

  i2c->join();
  gps->join();
  rec->join();

  return 0;
}
