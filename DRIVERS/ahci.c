#include "ahci.h"

void initialize_AHCI(int driverID){
    //printk("AHCI DRIVER INIT\n");
    printk("AHCI Driver %2h BAR: %8h\n\0", pci_drivers[driverID]->init_one->progIF, pci_drivers[driverID]->BAR[5]);
}