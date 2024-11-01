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

void ethernet_init(struct PCI_driver *pci){
    struct ethernet_driver *driver;
    if(
        pci->device->vendorID == 0x8086 &&
        pci->device->deviceID == 0x100E
    ){
        print_serial("[ETHERNET] Init E1000 Driver\n");
        driver = e1000_init(pci);
    }

    if(!ethernet_assign_driver(driver)){
        print_serial("[ETHERNET] Unable to add ethernet device!\n");
        return;
    }

    print_serial("[ETHERNET] MAC: %2x:%2x:%2x:%2x:%2x:%2x\n", driver->mac[0], driver->mac[1], driver->mac[2], driver->mac[3], driver->mac[4], driver->mac[5]);
}