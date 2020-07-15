#pragma once

#include <packet.h>
#include <ringbuffer.h>
namespace gni {
using packetbuffer_t = ringbuffer_t<packet_t, 2048>;
}