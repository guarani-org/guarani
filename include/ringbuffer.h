#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string.h>

template <class T, uint32_t N = 1> class ringbuffer_t {
public:
  ringbuffer_t(void) : _begin(0), _end(0), _max(N), _full(false) {
    _buffer = std::make_unique<T[]>(N);
  }
  ~ringbuffer_t(void) = default;

  void put(T &data) noexcept {
    std::unique_lock<std::mutex> l(_mtx);
    _buffer[_begin] = data;

    if (_full) {
      _end = (_end + 1) % _max;
    }
    _begin = (_begin + 1) % _max;
    _full = (_begin == _end);
  }

  int get(T &data) noexcept {
    std::unique_lock<std::mutex> l(_mtx);
    if (empty()) {
      return -1;
    }
    data = _buffer[_end];
    _full = false;
    _end = (_end + 1) % _max;
    return 0;
  }

private:
  std::unique_ptr<T[]> _buffer;
  uint32_t _begin;
  uint32_t _end;
  const uint32_t _max;
  bool _full;
  std::mutex _mtx;

  inline bool empty(void) noexcept { return (!_full && (_begin == _end)); }
};
