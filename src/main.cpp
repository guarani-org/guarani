
#include <config_ublox.h>
#include <fstream>
#include <mutex>
#include <recorder.h>
#include <serial.h>
#include <thread>
#include <gps.h>

int main(int ac, char **av) {
  gni::serial_t serial("/dev/ttyS0", B9600);
  if (!serial.open(O_RDWR | O_NOCTTY)) {

    return -1;
  }

  gni::ublox_config ubcfg;
  ubcfg.configure(serial, "gps.hex");

  serial.set_baudrate(B115200);

  std::mutex pkt_mtx;
  gni::packet_queue_t packets;

  gni::recorder_t rec(packets, 1024 * 10);
  rec.initialize();
  rec.start();

  // auto thread_gps = std::thread([&serial, &packets, &pkt_mtx]() {
  //   uint8_t buffer[gni::max_payload_sz];
  //   uint16_t buff_pos = 0;
  //   uint8_t temp = 0;
  //   uint16_t bytes_received = 0;
  //   gni::packet_t pkt;

  //   while (1) {
  //     bytes_received = serial.read(temp, 1);
  //     if (bytes_received > 0) {
  //       buffer[buff_pos++] = temp;
  //       if (buff_pos >= sizeof(buffer)) {
  //         if (gni::create_packet(pkt, gni::pkt_type::gps, buffer,
  //                                buff_pos - 1)) {
  //           std::unique_lock<std::mutex> lock(pkt_mtx);
  //           packets.push(pkt);
  //         }
  //         buff_pos = 0;
  //       }
  //     } else {
  //       if (buff_pos > 0) {
  //         if (gni::create_packet(pkt, gni::pkt_type::gps, buffer,
  //                                buff_pos - 1)) {
  //           std::unique_lock<std::mutex> lock(pkt_mtx);
  //           packets.push(pkt);
  //         }
  //         buff_pos = 0;
  //       }
  //     }
  //     std::this_thread::yield();
  //   }
  // });

  rec.join();
  //thread_gps.join();
  return 0;
}
