#include "ahci.h"

void initialize_AHCI(int driverID){
    //printk("AHCI DRIVER INIT\n");
    fb_write_xy("AHCI Driver ", sizeof("AHCI Driver "), 0, 50, driverID+1);
    
    uint32_t dataBar = pci_drivers[driverID]->BAR[5]; 
    
    decodeHex(STR_edit, dataBar, 32, 0);
    fb_write_xy(STR_edit, 8, 1, 50+sizeof(usb_driverName)+9, driverID+1);

    decodeHex(STR_edit, pci_drivers[driverID]->init_one->progIF, 8, 0);
    fb_write_xy(STR_edit, 2, 1, 50+sizeof("AHCI Driver ")+1, driverID+1);
}