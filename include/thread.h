#pragma once
#include <atomic>
#include <thread>
namespace gni {

class thread_t {
public:
  thread_t(void) : _stop(false) {}
  virtual ~thread_t(void) = default;

  void start(void) noexcept {
    _thread = std::thread([this]{this->run(this->_stop);});
  }
  void stop(void) noexcept { _stop = true; }
  void join(void) noexcept {
    if (_thread.joinable()) {
      _thread.join();
    }
  }

protected:
  virtual void run(std::atomic_bool &stop_flag) noexcept = 0;

private:
  std::thread _thread;
  std::atomic_bool _stop;
};
} // namespace gni