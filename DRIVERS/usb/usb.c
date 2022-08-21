#include "usb.h"
struct usb_host_controller usb_host_controllers[4];
char usb_driverName[] = "Universal Serial Bus Driver: \n\0";
bool usb_organized = false;
void usb_init_driver(int driverID){
    printk(usb_driverName);
    switch(pci_drivers[driverID]->init_one->progIF){
        case 0x0:
            printk("UHCI \n");
            init_uhci(driverID);
            usb_host_device_num++;
            break;
        case 0x10:
            printk("OHCI \n");
            usb_host_device_num++;
            break;
        case 0x20:
            printk("EHCI \n");
            ehci_init(driverID);
            usb_host_device_num++;
            break;
        case 0x30:
            printk("XHCI \n");
            xhci_init(pci_drivers[driverID]->BAR[0],pci_drivers[driverID]->interrupt);
            usb_host_device_num++;
            break;
        case 0x80:
            printk("UNKN \0");
            break;
        case 0xFE:
            printk("USBD \0");
            break;
        default:
            break;
    }
    printk("%2h  USB BAR:\n\0", driverID);
    for(int i = 0; i < 6; i++){
        printk("%1h %8h\n\0", i, pci_drivers[driverID]->BAR[i]);
    }  
}

void usb_add_host_controller(int PCI_driverID){
    uint32_t BAR = 0;
    switch(pci_drivers[PCI_driverID]->init_one->progIF){
        case 0x20:
        case 0x30:
            BAR = pci_drivers[PCI_driverID]->BAR[0];
            break;
    }
    usb_host_controllers[usb_host_device_num].PCI_Driver_ID = PCI_driverID;
    usb_host_controllers[usb_host_device_num].Base_Address = BAR;
    usb_host_controllers[usb_host_device_num].controller_type = pci_drivers[PCI_driverID]->init_one->progIF;
}

void usb_exit_driver(){
    
}