#include "hci.h"

void init_uhci(int pciDriverID){
    printk("UHCI Driver Init @ %2h\n", pciDriverID);
    usb_host_device_list[usb_host_device_num] = malloc(sizeof(uhci_struct));
    printk("%4h\n", usb_host_device_list[usb_host_device_num]);
}