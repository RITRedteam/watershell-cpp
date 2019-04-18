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
#include <limits>
#include <fstream>
#include <regex>
#include <sstream>




class Watershell {
public:
  char iface[100];
  std::string gateway_mac;
  Watershell(int port);
  Watershell(int port, bool DEBUG);
  Watershell(int port, bool DEBUG, bool PROMISC);
  int RunOnce(void);
  void Init(void);


private:
  bool DEBUG, PROMISC;
  int port, sockfd;
  struct ifreq *sifreq;
  struct sock_fprog filter;

  void SetGatewayMAC(void);
  std::string GetMacFromIP(char *ip_addr);
  void GetInterfaceName(char iface[]);
  void SendReply(unsigned char *buf, const char *payload);
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
