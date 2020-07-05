
#include <config_ublox.h>
#include <fstream>
#include <mutex>
#include <packet.h>
#include <queue>
#include <serial.h>
#include <thread>

int main(int ac, char **av) {
  gni::serial_t serial("/dev/ttyS0", B9600);
  if (!serial.open(O_RDWR | O_NOCTTY)) {

    return -1;
  }

  gni::ublox_config ubcfg;
  ubcfg.configure(serial, "gps.hex");

  serial.set_baudrate(B115200);

  std::mutex pkt_mtx;
  std::queue<gni::packet_t> packets;

  auto thread_writter = std::thread([&packets, &pkt_mtx]() {
    FILE *out;
    out = fopen("data.gni", "wb");
    while (1) {
      if (packets.empty()) {
        std::this_thread::yield();
      } else {
        // std::unique_lock<std::mutex> lock(pkt_mtx);
        auto packet = std::move(packets.front());
        packets.pop();
        fwrite(&packet, sizeof(gni::header_t) + packet.header.size, 1, out);
      }
    }
  });

  auto thread_gps = std::thread([&serial, &packets, &pkt_mtx]() {
    uint8_t buffer[8192];
    uint16_t buff_pos = 0;
    uint8_t temp = 0;
    uint16_t bytes_received = 0;

    while (1) {
      bytes_received = serial.read(temp, 1);
      if (bytes_received > 0) {
        buffer[buff_pos++] = temp;
        if (buff_pos >= sizeof(buffer) - 1) {
          buffer[buff_pos] = '\0';
          // std::unique_lock<std::mutex> lock(pkt_mtx);
          packets.emplace(gni::pkt_type::gps, buff_pos, buffer);
          std::cout << buffer << "\n dataLen = " << buff_pos << "\n";
          ::memset(&buffer, 0, buff_pos);
          buff_pos = 0;
        } else {
          std::this_thread::yield();
        }
      } else {
        if (buff_pos > 0) {
          buffer[buff_pos] = '\0';
          // std::unique_lock<std::mutex> lock(pkt_mtx);
          packets.emplace(gni::pkt_type::gps, buff_pos, buffer);
          std::cout << buffer << "\n dataLen = " << buff_pos << " :timeout\n";
          ::memset(&buffer, 0, buff_pos);
          buff_pos = 0;
        } else {
          std::this_thread::yield();
        }
      }
      std::this_thread::yield();
    }
  });

  thread_gps.join();
  thread_writter.join();
  return 0;
}
