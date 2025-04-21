#include "ethernet.h"
#include "arp.h"
#include "ip.h"

struct ethernet_driver *ethernet_drivers[2] = {NULL, NULL};


bool ethernet_assign_driver(struct ethernet_driver *driver) {
    if(driver == NULL) return false;
    for (int i = 0; i < 2; i++) {
        if (!ethernet_drivers[i]) {
            ethernet_drivers[i] = driver;
            print_serial("[ETHERNET] Assigned ethernet driver to interface %d\n", i);
            return true;
        }
    }

    return false;
}

void ethernet_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    //print_serial("[ETHERNET] interrupt\n");
    //pic_acknowledge(INT_currentInterrupt);

    //print_serial("[ETHERNET] ethernet_handler: %d\n", INT_currentInterrupt-32);
    for (int i = 0; i < 2; i++) {
        if (!ethernet_drivers[i]) {
            continue;
        }

        if (ethernet_drivers[i]->int_number == INT_currentInterrupt-32 && ethernet_drivers[i]->int_handler) {
            ethernet_drivers[i]->int_handler(ethernet_drivers[i]);
        }
    }
    //print_serial("[ETHERNET] interrupt complete\n");
    return;
}

void ethernet_init(struct PCI_driver *pci){
    struct ethernet_driver *driver = NULL;
    if(
        pci->device->vendorID == 0x8086 &&
        pci->device->deviceID == 0x100E
    ){
        print_serial("[ETHERNET] Init E1000 Driver\n");
        driver = e1000_init(pci);
    }
    else if(
        pci->device->vendorID == 0x10EC &&
        pci->device->deviceID == 0x8139
    ){
        print_serial("[ETHERNET] Init Realtek RTL8139 Driver\n");
        driver = rtl8139_init(pci);
    }

    if(!ethernet_assign_driver(driver)){
        print_serial("[ETHERNET] Unable to add ethernet device!\n");
        return;
    }

    print_serial("[ETHERNET] MAC: %2x:%2x:%2x:%2x:%2x:%2x\n", driver->mac[0], driver->mac[1], driver->mac[2], driver->mac[3], driver->mac[4], driver->mac[5]);
    print_serial("[ETHERNET] Interrupt Line: %d\n", pci->interrupt);

    
    IRQ_clear_mask(pci->interrupt);
    interrupt_add_handle(32 + pci->interrupt, ethernet_handler);
    if(driver->int_enable){
        driver->int_enable(driver);
    }
    
    print_serial("[ETHERNET] Init Complete\n");
}

int ethernet_send_packet(struct ethernet_driver *ether, uint8_t *dst_mac_addr, uint8_t *data, int len, uint16_t protocol){
    //print_serial("[ETHERNET] Sending Packet of size %d\n", len);
    struct ethernet_header *packet = malloc(sizeof(struct ethernet_header) + len);
    void *frame_data = (void*)packet + sizeof(struct ethernet_header);

    memcpy(packet->source_mac, ether->mac, 6);
    memcpy(packet->destination_mac, dst_mac_addr, 6);

    memcpy(frame_data, data, len);

    packet->ethertype = htons(protocol);

    ether->write(ether, (void *) packet, sizeof(struct ethernet_header) + len);

    return len;
}

void ethernet_demo(){
    struct ethernet_driver *ether = ethernet_drivers[0];
    if(ether == NULL) return;

    char message[] = "HELLO WORLD!\n";
    uint8_t destination[6] = {
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0
    };
    ethernet_send_packet(ether, destination, (uint8_t *) message, sizeof(message), ETHERNET_TYPE_IP);

    uint8_t check_mac[6];
    arp_get_mac(ether, (uint8_t [4]){10, 0, 1, 2}, check_mac, 100);
}

void ethernet_handle_packet(struct ethernet_header *packet, int len){
    //print_serial("[ETHERNET] Handling Packet!\n");
    //void *data = (void *) packet + sizeof(struct ethernet_header);
    int data_len = len - sizeof(struct ethernet_header);
    //print_serial("[ETHERNET] Data (@ 0x%x) Size: %d\n", data, data_len);

    if(ntohs(packet->ethertype) == ETHERNET_TYPE_ARP){
        print_serial("[ETHERNET] Arp Packet Identified\n");
        arp_receive_packet(ethernet_drivers[0], (struct arp_packet *) packet->data);
    }
    if(ntohs(packet->ethertype) == ETHERNET_TYPE_IP){
        print_serial("[ETHERNET] IP Packet Identified\n");
        ipv4_receive_packet(ethernet_drivers[0], (struct ipv4_packet *) packet->data, packet->data + sizeof(struct ipv4_packet), data_len - sizeof(struct ethernet_header));
    }
}

struct ethernet_driver *ethernet_getDriver(){
    return ethernet_drivers[0];
}