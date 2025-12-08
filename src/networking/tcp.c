#include "tcp.h"
#include "timer.h"
#include "netfs.h"

static uint32_t syn_c = 0;
static tcp_listener *tcp_listeners = NULL;

static const size_t tcp_syn_cache_size = 16;

typedef struct tcp_syn_cache_entry {
    uint16_t port;
    bool syn;
    uint32_t ack;
} tcp_syn_cache_entry;

static tcp_syn_cache_entry *tcp_syn_cache;
static size_t curr_tcp_syn = 0;

void tcp_init(void) {
    tcp_syn_cache = malloc(tcp_syn_cache_size * sizeof(tcp_syn_cache_entry));
    tcp_listeners = malloc(65536 * sizeof(tcp_listener));
}

uint16_t tcp_calculate_checksum(struct tcp_packet *packet, uint8_t source_ip[4], uint8_t destination_ip[4], void *data, size_t data_size) {
    struct ipv4_pseudo_header ipv4_header;
    struct tcp_packet pseudo_header;
    memcpy(ipv4_header.source_ip, source_ip, 4);
    memcpy(ipv4_header.destination_ip, destination_ip, 4);
    ipv4_header.zero = 0;
    ipv4_header.protocol = IP_PROTOCOL_TCP;
    ipv4_header.length = htons(data_size + sizeof(struct tcp_packet));
    memcpy(&pseudo_header, packet, sizeof(struct tcp_packet));
    pseudo_header.checksum = 0;

    uint32_t checksum = 0;

    uint16_t *data16 = (uint16_t *)&ipv4_header;
    for (size_t i = 0; i < sizeof(struct ipv4_pseudo_header) / 2; i++) {
        checksum += data16[i];
    }

    data16 = (uint16_t *)&pseudo_header;
    for (size_t i = 0; i < sizeof(struct tcp_packet) / 2; i++) {
        checksum += data16[i];
    }

    data16 = (uint16_t *)data;
    for (size_t i = 0; i < data_size / 2; i++) {
        checksum += data16[i];
    }

    if (data_size % 2) {
        checksum += ((uint8_t *)data)[data_size - 1];
    }

    while (checksum >> 16) {
        checksum = (checksum & 0xFFFF) + (checksum >> 16);
    }

    return ~checksum;
}

void tcp_send_packet(struct ethernet_driver *driver, uint8_t source_ip[4], uint16_t source_port, uint8_t destination_ip[4], uint16_t destination_port, uint32_t seq, uint32_t ack, bool is_syn, bool is_ack, bool is_fin, bool is_data, void *data, size_t data_size){
    print_serial("tcp_send_packet\n");
    struct tcp_packet packet;
    memset(&packet, 0, sizeof(struct tcp_packet));
    packet.source_port = htons(source_port);
    packet.destination_port = htons(destination_port);
    packet.sequence_number = htonl(seq == 0 ? syn_c++ : seq);
    packet.acknowledgement_number = htonl(ack);
    packet.data_offset = sizeof(struct tcp_packet) / sizeof(uint32_t);
    packet.reserved = 0;
    packet.flags.syn = is_syn;
    packet.flags.ack = is_ack;
    packet.flags.fin = is_fin;
    packet.flags.psh = is_data;
    packet.window_size = htons(32768);
    packet.checksum = tcp_calculate_checksum(&packet, source_ip, destination_ip, data, data_size);
    packet.urgent_pointer = htons(0);

    print_serial("tcp_send_packet seq_nb=%d, ack_nb=%d\n", ntohl(packet.sequence_number), ntohl(packet.acknowledgement_number));

    ipv4_send_packet(driver, source_ip, destination_ip, IP_PROTOCOL_TCP, &packet, sizeof(struct tcp_packet), data, data_size);
}

bool tcp_syn(struct ethernet_driver *driver, uint8_t destination_ip[4], uint16_t destination_port, uint16_t source_port, int timeout, uint32_t *ack){
    tcp_syn_cache[curr_tcp_syn].port = source_port;
    tcp_syn_cache[curr_tcp_syn].syn = false;
    tcp_syn_cache[curr_tcp_syn].ack = 0;
    curr_tcp_syn++;

    tcp_send_packet(driver, driver->ipv4.ip, source_port, destination_ip, destination_port, 0, 0, true, false, false, false, NULL, 0);

    for (int t = 0; t < timeout; t += 10) {
        wait(10);

        for (size_t i = 0; i < curr_tcp_syn; i++) {
            if (tcp_syn_cache[i].port == source_port) {
                if (tcp_syn_cache[i].syn) {
                    print_serial("TCP connection established in %d ms\n", t + 10);
                    *ack = tcp_syn_cache[i].ack;
                    return true;
                }
            }
        }
    }

    print_serial("TCP connection timed out\n");

    return false;
}

void tcp_close_connection(struct ethernet_driver *driver, uint8_t destination_ip[4], uint16_t destination_port, uint16_t source_port){
    tcp_send_packet(driver, driver->ipv4.ip, source_port, destination_ip, destination_port, 0, 0, false, true, true, false, NULL, 0);
}

bool tcp_install_listener(uint16_t port, tcp_listener listener){
    if (tcp_listeners[port]) {
        return false;
    }

    tcp_listeners[port] = listener;
    return true;
}

void tcp_uninstall_listener(uint16_t port){
    tcp_listeners[port] = NULL;
}

void tcp_receive_packet(struct ethernet_driver *driver, struct ipv4_packet *ipv4_packet, struct tcp_packet *packet, void *data) {
    if (!tcp_listeners) {
        return;
    }

    uint16_t dest_port = ntohs(packet->destination_port);
    uint16_t src_port = ntohs(packet->source_port);
    
    // Calculate lengths correctly
    size_t total_tcp_len = ntohs(ipv4_packet->total_length) - sizeof(struct ipv4_packet);
    size_t tcp_header_len = packet->data_offset * 4; // data_offset is in 32-bit words
    size_t data_len = (total_tcp_len > tcp_header_len) ? (total_tcp_len - tcp_header_len) : 0;
    
    print_serial("tcp_receive_packet port=%d seq=%d ack=%d data_len=%d flags=SYN:%d ACK:%d FIN:%d PSH:%d\n", 
                 dest_port, ntohl(packet->sequence_number), 
                 ntohl(packet->acknowledgement_number), data_len,
                 packet->flags.syn, packet->flags.ack, 
                 packet->flags.fin, packet->flags.psh);

    // Verify checksum
    uint16_t received_checksum = packet->checksum;
    packet->checksum = 0;
    uint16_t calculated_checksum = tcp_calculate_checksum(packet, ipv4_packet->source_ip, 
                                                         ipv4_packet->destination_ip, 
                                                         data, total_tcp_len - sizeof(struct tcp_packet));
    packet->checksum = received_checksum;
    
    if (received_checksum != calculated_checksum) {
        print_serial("tcp_receive_packet: checksum failed (got %x, expected %x)\n", 
                    received_checksum, calculated_checksum);
        return; // Drop packet on checksum failure
    }

    // Handle SYN-ACK response (connection establishment)
    if (packet->flags.syn && packet->flags.ack) {
        // Send ACK to complete 3-way handshake
        tcp_send_packet(driver, driver->ipv4.ip, dest_port, ipv4_packet->source_ip, 
                       src_port, ntohl(packet->acknowledgement_number), 
                       ntohl(packet->sequence_number) + 1, 
                       false, true, false, false, NULL, 0);
        
        // Update SYN cache
        for (size_t i = 0; i < curr_tcp_syn; i++) {
            if (tcp_syn_cache[i].port == dest_port) {
                tcp_syn_cache[i].syn = true;
                tcp_syn_cache[i].ack = ntohl(packet->acknowledgement_number);
                break;
            }
        }
        return;
    }

    // Handle incoming data or control packets
    if (tcp_listeners[dest_port]) {
        tcp_listener listener = tcp_listeners[dest_port];
        
        // Calculate pointer to actual data (skip TCP options if any)
        void *payload = (uint8_t*)packet + tcp_header_len;
        
        if (listener(driver, dest_port, payload, data_len)) {
            // Send ACK - acknowledge all received data
            uint32_t ack_num = ntohl(packet->sequence_number) + data_len;
            
            // If no data but SYN/FIN flags are set, still increment by 1
            if (data_len == 0 && (packet->flags.syn || packet->flags.fin)) {
                ack_num = ntohl(packet->sequence_number) + 1;
            }
            
            tcp_send_packet(driver, driver->ipv4.ip, dest_port, 
                          ipv4_packet->source_ip, src_port, 
                          ntohl(packet->acknowledgement_number), 
                          ack_num, 
                          false, true, false, false, NULL, 0);
        }
    }

    // Handle FIN
    if (packet->flags.fin) {
        print_serial("tcp_receive_packet: received FIN\n");
        
        // Send ACK for FIN
        tcp_send_packet(driver, driver->ipv4.ip, dest_port, 
                       ipv4_packet->source_ip, src_port, 
                       ntohl(packet->acknowledgement_number), 
                       ntohl(packet->sequence_number) + 1, 
                       false, true, false, false, NULL, 0);
        
        // Clean up connection state
        for (size_t i = 0; i < curr_tcp_syn; i++) {
            if (tcp_syn_cache[i].port == dest_port) {
                tcp_syn_cache[i].syn = false;
                tcp_syn_cache[i].ack = 0;
                break;
            }
        }

        // Clean up NetFS connections
        for(int i = 0; i < NETFS_MAXCONNECTIONS; i++){
            if(NetFS_Connections[i].port == dest_port){
                NetFS_Connections[i].pending = 0;
            }
        }
    }
}