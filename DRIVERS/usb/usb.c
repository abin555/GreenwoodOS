#include "usb.h"

char usb_driverName[] = "Universal Serial Bus Driver: \0";

void usb_init_driver(int driverID){
    //usb_host_device_list = (uint32_t **) malloc(32);
    printk(usb_driverName);
    switch(pci_drivers[driverID]->init_one->progIF){
        case 0x0:
            printk("UHCI\0");
            init_uhci(driverID);
            break;
        case 0x10:
            printk("OHCI\0");
            break;
        case 0x20:
            printk("EHCI\0");
            break;
        case 0x30:
            printk("XHCI\0");
            break;
        case 0x80:
            printk("UNKN\0");
            break;
        case 0xFE:
            printk("USBD\0");
            break;
        default:
            break;
    }
    printk(" | %2h\n  USB BAR:\n\0", driverID);
    for(int i = 0; i < 6; i++){
        printk("%1h %8h\n\0", i, pci_drivers[driverID]->BAR[i]);
    }  
}

void usb_exit_driver(){
    
}