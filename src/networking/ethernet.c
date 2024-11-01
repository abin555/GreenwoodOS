#include "ethernet.h"

struct ethernet_driver *ethernet_drivers[2];


bool ethernet_assign_driver(struct ethernet_driver *driver) {
    for (int i = 0; i < 2; i++) {
        if (!ethernet_drivers[i]) {
            ethernet_drivers[i] = driver;
            print_serial("[ETHERNET] Assigned ethernet driver to interface %d\n", i);
            return true;
        }
    }

    return false;
}

void ethernet_init(struct PCI_driver *driver){
    struct ethernet_driver *ether;
    if(
        driver->device->vendorID == 0x8086 &&
        driver->device->deviceID == 0x100E
    ){
        print_serial("[ETHERNET] Init E1000 Driver\n");
        ether = e1000_init(driver);
    }

    if(!ethernet_assign_driver(ether)){
        print_serial("[ETHERNET] Unable to add ethernet device!\n");
        return;
    }

    print_serial("[ETHERNET] MAC: %2x:%2x:%2x:%2x:%2x:%2x\n", ether->mac[0], ether->mac[1], ether->mac[2], ether->mac[3], ether->mac[4], ether->mac[5]);
}