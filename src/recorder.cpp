#include <recorder.h>
#include <sstream>
#include <sys/time.h>

namespace gni {

recorder_t::recorder_t(std::queue<packet_t> &packets, uint32_t max_file_size,
                       std::mutex & mtx)
    : _packets(packets), _file_segment_counter(0), _file_size_counter(0),
      _max_file_size(max_file_size), _file(nullptr), _mtx(mtx) {}

void recorder_t::run(std::atomic_bool &stop_flag) noexcept {
  stop_flag = false;
  while (!stop_flag) {

    if (!_packets.empty()) {
      std::unique_lock<std::mutex> lock(_mtx);
      auto pkt = _packets.front();
      _packets.pop();

      fwrite(&pkt, sizeof(gni::header_t) + pkt.header.size, 1, _file);

      _file_size_counter += (sizeof(gni::header_t) + pkt.header.size);

      if (_file_size_counter >= _max_file_size) {
        _file_size_counter = 0;
        open_new_file();
      }
    }
    std::this_thread::yield();
  }
}

bool recorder_t::open_new_file(void) noexcept {
  fflush_unlocked(_file);
  fclose(_file);
  std::ostringstream os;
  os << _base_file_name << _file_segment_counter++ << ".gni";
  _file = fopen(os.str().data(), "wb+");
  if (_file != nullptr && _file != NULL) {
    setbuf(_file, NULL); // no buffering
    printf("New file created: %s\n", os.str().data());
    return true;
  }
  return false;
}

bool recorder_t::initialize(void) noexcept {
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  std::ostringstream os;
  os << ts.tv_sec << "_";
  _base_file_name = os.str();
  os << _file_segment_counter++ << ".gni";

  _file = fopen(os.str().data(), "wb+");
  if (_file != nullptr && _file != NULL) {
    setbuf(_file, NULL); // no buffering
    return true;
  }

  return false;
}
} // namespace gni