#ifndef ETHERNET_H
#define ETHERNET_H

#include "pci.h"
#include "interrupts.h"
#include "serial.h"
#include "stdint.h"

#include "network_utils.h"
#include "intel_e1000.h"
#include "rtl8139.h"

#define ETHERNET_TYPE_ARP 0x0806
#define ETHERNET_TYPE_IP  0x0800

struct ethernet_header {
    uint8_t destination_mac[6];
    uint8_t source_mac[6];
    uint16_t ethertype;
    uint8_t data[];
} __attribute__((packed));

struct ipv4_config {
    uint8_t ip[4];
    uint8_t netmask[4];
    uint8_t gateway[4];
    uint8_t dns[4];
    int lease_time;
};

struct ethernet_driver;
struct ethernet_driver{
    struct PCI_driver *pci;

    bool bar_type;
    uint32_t mem_base;
    uint32_t io_base;
    uint8_t mac[6];
    bool eeprom_exists;

    struct ipv4_config ipv4;
    bool duplex;
    int speed;
    bool up;
    
    uint8_t *rx_buffer;
    uint32_t rx_buffer_size;
    uint32_t rx_buffer_end;

    uint8_t *tx_buffer;
    uint32_t tx_buffer_size;
    uint32_t tx_buffer_end;
    bool tx_lock;

    void *private_page_base;
    uint32_t private_page_offset;
    int num_private_pages;

    uint32_t (*write)(struct ethernet_driver *driver, void *packet, uint32_t size);
    unsigned int int_number;
    void (*int_handler)(struct ethernet_driver *driver);
    void (*int_enable)(struct ethernet_driver *driver);
    void (*int_disable)(struct ethernet_driver *driver);
};

void ethernet_init(struct PCI_driver *driver);
int ethernet_send_packet(struct ethernet_driver *ether, uint8_t *dst_mac_addr, uint8_t *data, int len, uint16_t protocol);
void ethernet_handle_packet(struct ethernet_header *packet, int len);
void ethernet_demo();
struct ethernet_driver *ethernet_getDriver();
#endif