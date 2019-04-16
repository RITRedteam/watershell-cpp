#ifndef WATERSHELL_H_
#define WATERSHELL_H_

// Networking
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/filter.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>
#include <ifaddrs.h>

// General
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <string>
#include <cstring>


class Watershell {
public:
  Watershell(int port);
  Watershell(int port, bool DEBUG);
  Watershell(int port, bool DEBUG, bool PROMISC);
  int RunOnce(void);


private:
  char iface[100];
  bool DEBUG, PROMISC;
  int port, sockfd;
  struct ifreq *sifreq;
  struct sock_fprog filter;

  void GetInterfaceName(char iface[]);
  void SendReply(unsigned char *buf, unsigned char *payload);
  // void Sigint(int signum);
  void CalcIPChecksum(struct iphdr *ip);
};


/* its a datagram inside a packet inside a frame!
 * gotta be packed though!
 */
struct __attribute__((__packed__)) udpframe {
    struct ethhdr ehdr;
    struct iphdr ip;
    struct udphdr udp;
    unsigned char data[ETH_DATA_LEN - sizeof(struct udphdr) - sizeof(struct iphdr)];
};
#endif // WATERSHELL_H_
