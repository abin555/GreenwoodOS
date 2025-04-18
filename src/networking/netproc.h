#ifndef NETPROC_H
#define NETPROC_H

#include "stdint.h"
#include "ethernet.h"
#include "ip.h"
#include "tcp.h"
#include "udp.h"
#include "http.h"
#include "arp.h"
#include "dhcp.h"
#include "icmp.h"

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

int netproc_addToQueue(int caller_pid, struct netproc_request request);
void netproc_init();
int netprocess(int argc, char **argv);

int netproc_checkPending_icmp_reply(uint8_t source_ip[4], unsigned int packet_size, struct icmp_packet *packet);

#endif