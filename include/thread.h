#pragma once
#include <atomic>
#include <thread>
namespace gni {

class thread_t {
public:
  thread_t(void) : _stop(false) {}
  virtual ~thread_t(void) = default;

  inline void start(void) noexcept {
    _thread = std::thread(&thread_t::run, this);
  }
  void stop(void) noexcept { _stop = true; }
  void join(void) noexcept {
    if (_thread.joinable()) {
      _thread.join();
    }
  }

protected:
  virtual void run(void) noexcept = 0;
  std::atomic_bool _stop;
  std::thread _thread;

private:
};
} // namespace gni