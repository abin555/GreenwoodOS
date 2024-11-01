#ifndef ETHERNET_H
#define ETHERNET_H

#include "pci.h"
#include "interrupts.h"
#include "serial.h"
#include "stdint.h"

#include "intel_e1000.h"

struct ethernet_header {
    uint8_t destination_mac[6];
    uint8_t source_mac[6];
    uint16_t ethertype;
} __attribute__((packed));

struct ethernet_packet{
    struct ethernet_header header;
    void *data;
};

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
    uint8_t tx_buffer_size;
    uint8_t tx_buffer_end;

    uint32_t (*write)(struct ethernet_driver *driver, struct ethernet_packet *packet, uint32_t size);
    int int_number;
    void (*int_handler)(struct ethernet_driver *driver);
    void (*int_enable)(struct ethernet_driver *driver);
    void (*int_disable)(struct ethernet_driver *driver);
};

void ethernet_init(struct PCI_driver *driver);

#endif