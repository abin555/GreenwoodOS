#include <stdio.h>
#include <sys/io.h>
#include <stdint.h>

struct netstat {
    uint8_t ip[4];
    uint8_t subnet[4];
    uint8_t gateway[4];
    uint8_t dns[4];
};

int main(int argc, char **argv){
    struct netstat nstat;
    int netfd = open("/-/net/conn", O_READ);
    if(netfd == -1){
        printf("Unable to access net sys file\n");
        return 1;
    }
    read(netfd, &nstat, sizeof(nstat));
    close(netfd);
    printf("     IP: %d.%d.%d.%d\n", nstat.ip[0], nstat.ip[1], nstat.ip[2], nstat.ip[3]);
    printf(" SUBNET: %d.%d.%d.%d\n", nstat.subnet[0], nstat.subnet[1], nstat.subnet[2], nstat.subnet[3]);
    printf("GATEWAY: %d.%d.%d.%d\n", nstat.gateway[0], nstat.gateway[1], nstat.gateway[2], nstat.gateway[3]);
    printf("    DNS: %d.%d.%d.%d\n", nstat.dns[0], nstat.dns[1], nstat.dns[2], nstat.dns[3]);
    return 0;
}