#include "usb.h"

char usb_driverName[] = "Universal Serial Bus Driver";

void usb_init_driver(int driverID){
    fb_write_xy(usb_driverName, sizeof(usb_driverName), 0, 40, driverID);
    uint16_t progIF = getDeviceProgIF(
        pci_drivers[driverID]->table->bus,
        pci_drivers[driverID]->table->slot,
        pci_drivers[driverID]->table->func
    );
    switch(progIF){
        case 0x0:
            fb_write_xy("UHCI", 4, 0, 40+sizeof(usb_driverName)+1,driverID);
            break;
        case 0x10:
            fb_write_xy("OHCI", 4, 0, 40+sizeof(usb_driverName)+1,driverID);
            break;
        case 0x20:
            fb_write_xy("EHCI", 4, 0, 40+sizeof(usb_driverName)+1,driverID);
            break;
        case 0x30:
            fb_write_xy("XHCI", 4, 0, 40+sizeof(usb_driverName)+1,driverID);
            break;
        case 0x80:
            fb_write_xy("UNKN", 4, 0, 40+sizeof(usb_driverName)+1,driverID);
            break;
        case 0xFE:
            fb_write_xy("USBD", 4, 0, 40+sizeof(usb_driverName)+1,driverID);
            break;
    }
}
void usb_exit_driver(){
    
}