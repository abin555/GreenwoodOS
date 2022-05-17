#include "usb.h"

char usb_driverName[] = "Universal Serial Bus Driver";

void usb_init_driver(int driverID){
    fb_write_xy(usb_driverName, sizeof(usb_driverName), 0, 30, fb_terminal_h-driverID-1);
    uint16_t progIF = getDeviceProgIF(
        pci_drivers[driverID]->init_one->device_id->bus,
        pci_drivers[driverID]->init_one->device_id->slot,
        pci_drivers[driverID]->init_one->device_id->func
    );
    switch(progIF){
        case 0x0:
            fb_write_xy("UHCI", 4, 0, 30+sizeof(usb_driverName)+1,fb_terminal_h-driverID-1);
            break;
        case 0x10:
            fb_write_xy("OHCI", 4, 0, 30+sizeof(usb_driverName)+1,fb_terminal_h-driverID-1);
            break;
        case 0x20:
            fb_write_xy("EHCI", 4, 0, 30+sizeof(usb_driverName)+1,fb_terminal_h-driverID-1);
            break;
        case 0x30:
            fb_write_xy("XHCI", 4, 0, 30+sizeof(usb_driverName)+1,fb_terminal_h-driverID-1);
            break;
        case 0x80:
            fb_write_xy("UNKN", 4, 0, 30+sizeof(usb_driverName)+1,fb_terminal_h-driverID-1);
            break;
        case 0xFE:
            fb_write_xy("USBD", 4, 0, 30+sizeof(usb_driverName)+1,fb_terminal_h-driverID-1);
            break;
    }

    uint32_t dataBar = getDeviceBar(
        pci_drivers[driverID]->init_one->device_id->bus,
        pci_drivers[driverID]->init_one->device_id->slot,
        pci_drivers[driverID]->init_one->device_id->func,
        4
    );
    decodeHex(STR_edit, dataBar, 32, 50);
    fb_write_xy(STR_edit, 8, 51, fb_terminal_w-9, fb_terminal_h-driverID-1);
}
void usb_exit_driver(){
    
}