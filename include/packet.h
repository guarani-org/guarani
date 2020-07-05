#pragma once
#include <cstdint>
#include <memory>
#include <string.h>
#include <sys/time.h>

namespace gni {

namespace pkt_type {
enum { gps = 0, baro, mag, gyro, accel };

}

#pragma pack(1)

struct header_t {
  uint64_t packet_sync = 4268435520;
  uint8_t type;
  uint64_t seconds;
  uint64_t nanos;
  uint32_t size;
};

struct packet_t {
  header_t header;
  uint8_t payload[1500];

  packet_t(uint8_t tp, uint32_t sz, uint8_t *pl) {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    header.seconds = ts.tv_sec;
    header.nanos = ts.tv_nsec;
    header.size = sz;
    memcpy(&payload, pl, sz);
  }
};
#pragma pop()

} // namespace gni