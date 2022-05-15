#include "usb.h"

char usb_driverName[] = "Universal Serial Bus Driver";

void usb_init_driver(int driverID){
    fb_write_xy(usb_driverName, sizeof(usb_driverName), 0, 40, driverID);
    decodeHex(STR_edit, 
    
    getDeviceClass(
        pci_drivers[driverID]->init_one->device_id->bus,
        pci_drivers[driverID]->init_one->device_id->slot,
        pci_drivers[driverID]->init_one->device_id->func
    ),
    
    //pci_drivers[driverID]->init_one->vendor, 
    16, 
    0);
    fb_write_xy(STR_edit, 4, 1, 40+sizeof(usb_driverName)+1,driverID);
}
void usb_exit_driver(){

}