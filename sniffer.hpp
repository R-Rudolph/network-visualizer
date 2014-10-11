#ifndef __SNIFFER_HPP_INCLUDED__
#define __SNIFFER_HPP_INCLUDED__

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcap.h>
#include "NetworkEntities.hpp"
#include "MovingCube.hpp"

#define SNIFFER_SNAP_LEN 20000
#define SIZE_ETHERNET 14

/* IP header */
struct ip_header
{
  uint8_t  ip_vhl; //4 bits version, 4 bits header length
  uint8_t  ip_tos; //type of service
  uint16_t ip_len; //total length
  uint16_t ip_id;  //identification
  uint16_t ip_off; //fragment offset field
  uint8_t  ip_ttl; //time to live
  uint8_t  ip_p;   //protocol
  uint16_t ip_sum; //checksum
  uint32_t ip_src; //src address
  uint32_t ip_dst; //dst address
};

struct tcp_header_t
{
  uint16_t src_port;
  uint16_t dst_port;
  uint32_t seq;
  uint32_t ack;
  uint8_t  data_offset;  // 4 bits offset, 4 reserved
  uint8_t  flags;        // 2 bit reserved, 
  uint16_t window_size;
  uint16_t checksum;
  uint16_t urgent_p;
};

void printTCP(u_char* content, bpf_u_int32 length);
void printIPPackage(u_char* content, bpf_u_int32 length);
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void* sniffer_start_thread(void* arg);
void sniffer_start();

#endif

