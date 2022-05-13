#include "usb.h"

char usb_driverName[] = "Universal Serial Bus Driver";

void usb_init_driver(struct __pci_device *device){
    printChar(20,20,'S');
    fb_write_xy(usb_driverName, sizeof(usb_driverName), 0, 8, 8);
    decodeHex(STR_edit, getDeviceClass(device_id->bus, device_id->slot, device_id->func), 16, 0);
    fb_write_xy(STR_edit, 4, -1, 8,9);
}
void usb_exit_driver(){

}