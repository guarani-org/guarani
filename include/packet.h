#pragma once
#include <cstdint>
#include <memory>
#include <queue>
#include <string.h>
#include <sys/time.h>

namespace gni {
static constexpr auto max_payload_sz = 1500;
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
  uint8_t payload[max_payload_sz];
};
#pragma pop()

static constexpr auto packet_header_size = sizeof(header_t);
inline bool create_packet(packet_t &pkt, uint8_t type, void *payload,
                   uint32_t size) noexcept {
  if (size <= max_payload_sz) {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    pkt.header.seconds = ts.tv_sec;
    pkt.header.nanos = ts.tv_nsec;
    pkt.header.size = size;
    memcpy(pkt.payload, payload, size);
    return true;
  }
  return false;
}

using packet_queue_t = std::queue<packet_t>;
} // namespace gni
