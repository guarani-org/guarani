
#include <serial.h>
#include <thread>

namespace gni {



serial_t::serial_t(std::string_view device, int baud_rate)
    : _fd(-1), _device(device), _baud_rate(baud_rate) {}

bool serial_t::open(int32_t mode) noexcept {
  _fd = ::open(_device.data(), mode);

  if (_fd < 0) {
    std::cerr << "Unable to open serial port: " << _device << "\n";
    return false;
  }
  if (::tcgetattr(_fd, &_oldtio) < 0) {
    std::cerr << "Unable to get actual serial configuration from: " << _device
              << "\n";
    return false;
  }

  memset(&_newtio, 0, sizeof(_newtio));

  //_newtio.c_cflag = _baud_rate | CRTSCTS | CS8 | CLOCAL | CREAD;
  _newtio.c_cflag = _baud_rate | CS8 | CLOCAL | CREAD;
  _newtio.c_iflag = IGNPAR;
  _newtio.c_oflag = 0;

  /* set input mode (non-canonical, no echo,...) */
  _newtio.c_lflag = 0;

  _newtio.c_cc[VTIME] = 0; /* inter-character timer TIME*.1 */
  _newtio.c_cc[VMIN] = serial_char_rcv_count;

  if (::tcflush(_fd, TCIFLUSH) >= 0 &&
      ::tcsetattr(_fd, TCSANOW, &_newtio) >= 0) {
    return true;
  }
  std::cerr << "Unable to set  serial configuration to: " << _device << "\n";
  return false;
}

bool serial_t::set_baudrate(int baud_rate) noexcept {
  _baud_rate = baud_rate;
  //_newtio.c_cflag = _baud_rate | CRTSCTS | CS8 | CLOCAL | CREAD;
  _newtio.c_cflag = _baud_rate | CS8 | CLOCAL | CREAD;

  if (tcflush(_fd, TCIFLUSH) >= 0 && tcsetattr(_fd, TCSANOW, &_newtio) >= 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return true;
  }

  std::cerr << "Unable to set  serial configuration to: " << _device << "\n";
  return false;
}

bool serial_t::close(void) noexcept {
  if (_fd > 0) {
    return ::close(_fd) >= 0;
  }
  return true;
}

int serial_t::write(uint8_t *buffer, uint32_t size) noexcept {
  return ::write(_fd, buffer, size);
}

int serial_t::read(uint8_t *buffer, uint32_t bytes2read) noexcept {
  return ::read(_fd, buffer, bytes2read);
}

} // namespace gni