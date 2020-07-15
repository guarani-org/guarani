#pragma once
#include <mutex>
#include <packet.h>
#include <packetbuffer.h>
#include <queue>
#include <string>
#include <thread.h>

/*
  file name:
    timestamp_segment.gni
*/
namespace gni {
class recorder_t : public thread_t {
public:
  recorder_t(packetbuffer_t &packets, uint32_t max_file_size);
  ~recorder_t(void) = default;

  bool initialize(void) noexcept;

protected:
  void run(void) noexcept override;

private:
  FILE *_file;
  uint32_t _max_file_size;
  uint32_t _file_segment_counter;
  std::string _base_file_name;
  uint32_t _file_size_counter;
  packetbuffer_t &_pktbuff;
  bool open_new_file(void) noexcept;
};

} // namespace gni