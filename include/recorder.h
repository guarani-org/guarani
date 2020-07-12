#pragma once
#include <packet.h>
#include <queue>
#include <string>
#include <thread.h>
#include <mutex>

/*
  file name:
    timestamp_segment.gni
*/
namespace gni {
class recorder_t : public thread_t {
public:
  recorder_t(packet_queue_t &packets, uint32_t max_file_size, std::mutex &mtx);
  ~recorder_t(void) = default;

  bool initialize(void) noexcept;

protected:
  void run(std::atomic_bool &stop_flag) noexcept override;

private:
  packet_queue_t &_packets;
  FILE *_file;
  uint32_t _max_file_size;
  uint32_t _file_segment_counter;
  std::string _base_file_name;
  uint32_t _file_size_counter;
  std::mutex &_mtx;
  bool open_new_file(void) noexcept;
};

} // namespace gni