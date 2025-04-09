#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/task.h>

struct icmp_packet {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    union {
        uint32_t header;
        uint8_t hbytes[4];
    };
} __attribute__((packed));

typedef int (*icmp_echo_reply_callback)(unsigned int packet_size, uint8_t source_ip[4], struct icmp_packet *packet);

typedef enum {
    NETPROC_ICMP_ECHO_REQUEST
} netproc_request_type;

struct netproc_icmp_echo_request {
    uint8_t dst_ip[4];
    icmp_echo_reply_callback callback;
};

struct netproc_request {
    netproc_request_type type;
    union {
        struct netproc_icmp_echo_request icmp_echo_request;
    } request;
};

int has_reply;

int callback(unsigned int packet_size, uint8_t *source_ip, struct icmp_packet *packet) {
    printf("PING Reply: %d.%d.%d.%d\n",
        source_ip[0],
        source_ip[1],
        source_ip[2],
        source_ip[3]
    );
    has_reply = 1;
    return 0;
}

int main(int argc, char **argv){
    int icmp_fd = open("/-/net/icmp", O_WRITE);
    if(icmp_fd == -1){
        printf("Unable to open ICMP file\n");
        return 1;
    }

    struct netproc_request ping;
    ping.type = NETPROC_ICMP_ECHO_REQUEST;
    ping.request.icmp_echo_request.dst_ip[0] = 8;
    ping.request.icmp_echo_request.dst_ip[1] = 8;
    ping.request.icmp_echo_request.dst_ip[2] = 8;
    ping.request.icmp_echo_request.dst_ip[3] = 8;
    ping.request.icmp_echo_request.callback = callback;

    has_reply = 0;

    printf("Ping %d.%d.%d.%d\n",
        ping.request.icmp_echo_request.dst_ip[0],
        ping.request.icmp_echo_request.dst_ip[1],
        ping.request.icmp_echo_request.dst_ip[2],
        ping.request.icmp_echo_request.dst_ip[3]
    );

    write(icmp_fd, &ping, sizeof(ping));

    while(has_reply == 0){

    }
    has_reply = 0;

    close(icmp_fd);
}