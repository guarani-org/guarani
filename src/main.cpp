
#include <config_ublox.h>
#include <fstream>
#include <gps.h>
#include <recorder.h>
#include <serial.h>
#include <thread>

int main(int ac, char **av) {
  // gni::serial_t serial("/dev/ttyS0", B9600);
  // if (!serial.open(O_RDWR | O_NOCTTY)) {

  //   return -1;
  // }

  // gni::ublox_config ubcfg;
  // ubcfg.configure(serial, "gps.hex");

  // serial.set_baudrate(B115200);

  // std::mutex pkt_mtx;
  gni::packet_queue_t packets;

  gni::recorder_t rec(packets, 1024 * 10);
  gni::gps_t gps("/dev/ttyS0", B9600, O_RDWR | O_NOCTTY, packets, "gps.hex");

  if(!rec.initialize()){
    return -1;
  }
 
  if(!gps.initialize()){
    return -1;
  }
  gps.start();
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
  // thread_gps.join();
  return 0;
}
