#include "usb.h"

char usb_driverName[] = "Universal Serial Bus Driver";

void usb_init_driver(int driverID, int reversedID){
    uint32_t dataBar = pci_drivers[reversedID]->BAR[4]; 

    decodeHex(STR_edit, driverID, 8, 0);
    fb_write_xy(STR_edit, 2, 1, 50+sizeof(usb_driverName)+6, driverID+1);

    decodeHex(STR_edit, dataBar, 32, 0);
    fb_write_xy(STR_edit, 8, 1, 50+sizeof(usb_driverName)+9, driverID+1);

    fb_write_xy(usb_driverName, sizeof(usb_driverName), 0, 50, driverID+1);
    uint16_t progIF = getDeviceProgIF(
        pci_drivers[driverID]->init_one->bus,
        pci_drivers[driverID]->init_one->slot,
        pci_drivers[driverID]->init_one->func
    );
    switch(progIF){
        case 0x0:
            fb_write_xy("UHCI", 4, 0, 50+sizeof(usb_driverName)+1,driverID+1);
            break;
        case 0x10:
            fb_write_xy("OHCI", 4, 0, 50+sizeof(usb_driverName)+1,driverID+1);
            break;
        case 0x20:
            fb_write_xy("EHCI", 4, 0, 50+sizeof(usb_driverName)+1,driverID+1);
            break;
        case 0x30:
            fb_write_xy("XHCI", 4, 0, 50+sizeof(usb_driverName)+1,driverID+1);
            break;
        case 0x80:
            fb_write_xy("UNKN", 4, 0, 50+sizeof(usb_driverName)+1,driverID+1);
            break;
        case 0xFE:
            fb_write_xy("USBD", 4, 0, 50+sizeof(usb_driverName)+1,driverID+1);
            break;
        default:
            decodeHex(STR_edit, progIF, 16, 0);
            fb_write_xy(STR_edit, 4, 1, 50+sizeof(usb_driverName)+1,driverID+1);
            break;
    }    
}

void usb_exit_driver(){
    
}