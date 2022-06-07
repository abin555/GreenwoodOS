#include "hci.h"

void init_uhci(int pciDriverID){
    printk("UHCI Driver Init @ %2h\n", pciDriverID);
    usb_host_device_list[usb_host_device_num] = (unsigned int *) pci_drivers[pciDriverID]->BAR[4];
    uhci_struct *uhci = (uhci_struct *)usb_host_device_list[usb_host_device_num];
    printk("Addr: %8h USB HOST ID: %2h\n", usb_host_device_list[usb_host_device_num], usb_host_device_num);
    printk("Frame Number: %4h\n", uhci->FRNUM);


    usb_host_device_num++;
}