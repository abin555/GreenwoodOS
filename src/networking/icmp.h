#ifndef ICMP_H
#define ICMP_H

#include "ethernet.h"
#include "ip.h"

struct icmp_packet {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    union {
        uint32_t header;
        uint8_t hbytes[4];
    };
} __attribute__((packed));

void icmp_echoRequest(struct ethernet_driver *driver, uint8_t dst[4], uint16_t id, uint16_t sequence, uint8_t *data, uint8_t *end);
void icmp_recieve_packet(struct ethernet_driver *driver, struct ipv4_packet *ipv4_packet, struct icmp_packet *packet, void *data);

#endif