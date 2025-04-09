#include "icmp.h"
#include "ip.h"
#include "tcp.h"
#include "udp.h"
#include "network_utils.h"
#include "serial.h"
#include "checksum.h"
#include "netproc.h"

#define ICMP_TYPE_ECHO_REPLY            0
#define ICMP_TYPE_DEST_UNREACHABLE      3
#define ICMP_TYPE_SOUCE_QUENCH          4
#define ICMP_TYPE_REDIRECT_MSG          5
#define ICMP_TYPE_ECHO_REQUEST          8
#define ICMP_TYPE_ROUTER_ADVERTISEMENT  9
#define ICMP_TYPE_ROUTER_SOLICITATION   10
#define ICMP_TYPE_TIME_EXCEEDED         11
#define ICMP_TYPE_BAD_PARAM             12
#define ICMP_TYPE_TIMESTAMP             13
#define ICMP_TYPE_TIMESTAMP_REPLY       14
#define ICMP_TYPE_INFO_REQUEST          15
#define ICMP_TYPE_INFO_REPLY            16
#define ICMP_TYPE_ADDR_MASK_REQUEST     17
#define ICMP_TYPE_ADDR_MASK_REPLY       18
#define ICMP_TYPE_TRACEROUTE            30

void icmp_debugPrint(struct icmp_packet *packet){
    print_serial("[ICMP] Type %d Code %d Checksum %x\n",
        packet->type,
        packet->code,
        packet->checksum
    );
}

void icmp_echoRequest(struct ethernet_driver *driver, uint8_t dst[4], uint16_t id, uint16_t sequence, uint8_t *data, uint8_t *end){
    uint32_t data_len = end - data;

    struct icmp_packet *icmp = malloc(sizeof(struct icmp_packet) + data_len);
    icmp->type = ICMP_TYPE_ECHO_REQUEST;
    icmp->code = 0;
    icmp->checksum = 0;
    icmp->hbytes[0] = (id >> 8) & 0xff;
    icmp->hbytes[1] = (id) & 0xff;
    icmp->hbytes[2] = (sequence >> 8) & 0xff;
    icmp->hbytes[3] = (sequence) & 0xff;
    memcpy(
        ((void *) icmp)+sizeof(struct icmp_packet),
        data,
        data_len
    );
    uint32_t checksum = NetChecksum((uint8_t *) icmp, ((uint8_t *) icmp)+sizeof(struct icmp_packet)+data_len);
    icmp->checksum = htons(checksum);

    icmp_debugPrint(icmp);

    ipv4_send_packet(
        driver,
        driver->ipv4.ip,
        dst,
        IP_PROTOCOL_ICMP,
        icmp,
        sizeof(struct icmp_packet), 
        data, 
        data_len
    );
    //icmp->checksum = htons(NetChecksum())
}

void icmp_echoReply(struct ethernet_driver *driver, uint8_t dst[4], uint16_t id, uint16_t sequence, uint8_t *data, uint8_t *end){
    uint32_t data_len = end - data;

    struct icmp_packet *icmp = malloc(sizeof(struct icmp_packet) + data_len);
    icmp->type = ICMP_TYPE_ECHO_REPLY;
    icmp->code = 0;
    icmp->checksum = 0;
    icmp->hbytes[0] = (id >> 8) & 0xff;
    icmp->hbytes[1] = (id) & 0xff;
    icmp->hbytes[2] = (sequence >> 8) & 0xff;
    icmp->hbytes[3] = (sequence) & 0xff;
    memcpy(
        ((void *) icmp)+sizeof(struct icmp_packet),
        data,
        data_len
    );
    uint32_t checksum = NetChecksum((uint8_t *) icmp, ((uint8_t *) icmp)+sizeof(struct icmp_packet)+data_len);
    icmp->checksum = htons(checksum);

    //icmp_debugPrint(icmp);

    ipv4_send_packet(
        driver,
        driver->ipv4.ip,
        dst,
        IP_PROTOCOL_ICMP,
        icmp,
        sizeof(struct icmp_packet), 
        data, 
        data_len
    );
}

void icmp_recieve_packet(struct ethernet_driver *driver, struct ipv4_packet *ipv4_packet, struct icmp_packet *packet, void *data){
    //icmp_debugPrint(packet);

    uint16_t id = htons(
        packet->hbytes[0] << 8 | packet->hbytes[1]
    );
    uint16_t sequence = htons(
        packet->hbytes[1] << 8 | packet->hbytes[2]
    );

    if(packet->type == ICMP_TYPE_ECHO_REQUEST){
        print_serial("[ICMP] Echo Request from %d.%d.%d.%d\n",
            ipv4_packet->source_ip[0],
            ipv4_packet->source_ip[1],
            ipv4_packet->source_ip[2],
            ipv4_packet->source_ip[3]
        );
        icmp_echoReply(
            driver,
            ipv4_packet->source_ip,
            id,
            sequence,
            data,
            ((uint8_t *) data) + ipv4_packet->total_length - sizeof(struct ipv4_packet) - sizeof(struct icmp_packet)
        );
    }
    else if(packet->type == ICMP_TYPE_ECHO_REPLY){
        print_serial(
            "[ICMP] Echo Reply %d.%d.%d.%d\n",
            ipv4_packet->source_ip[0],
            ipv4_packet->source_ip[1],
            ipv4_packet->source_ip[2],
            ipv4_packet->source_ip[3]
        );
        netproc_checkPending_icmp_reply(ipv4_packet->source_ip, ipv4_packet->total_length - sizeof(struct ipv4_packet) - sizeof(struct icmp_packet), packet);
    }
}
