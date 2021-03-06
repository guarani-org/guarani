#pragma once
#include <cstdint>
#include <memory>
#include <queue>
#include <string.h>
#include <sys/time.h>

namespace gni {
static constexpr auto max_payload_sz = 1024;
namespace pkt_type {
enum { gps = 0, baro, imu };
}

struct header_t {
  uint64_t packet_sync = 4268435520;
  uint8_t type;
  uint64_t seconds;
  uint64_t nanos;
  uint32_t size;
}__attribute__((aligned(1),packed));

struct packet_t {
  header_t header;
  uint8_t payload[max_payload_sz];
}__attribute__((aligned(1),packed));

static constexpr auto packet_header_size = sizeof(header_t);
inline bool create_packet(packet_t &pkt, uint8_t type, void *payload,
                          uint32_t size) noexcept {
  if (size <= max_payload_sz) {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    pkt.header.type = type;
    pkt.header.seconds = ts.tv_sec;
    pkt.header.nanos = ts.tv_nsec;
    pkt.header.size = size;
    memcpy(pkt.payload, payload, size);
    return true;
  }
  return false;
}

inline bool create_packet(packet_t &pkt, void *payload,
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
