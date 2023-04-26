#include "USB.h"

void usb_init_driver(int driverID){
    //printk("[USB] Device Init! @ %x\n", driverID);
    struct pci_driver* driver = pci_drivers[driverID];
    struct pci_device* device = driver->device;
    switch(driver->device->progIF){
        case 0x0000:
            printk("[USB] UHCI host contoller | BAR: %x\n", driver->BAR[4]);
            break;
        case 0x0010:
            printk("[USB] OHCI host controller | BAR: %x\n", driver->BAR[0]);
            break;
        case 0x0020:
            printk("[USB] EHCI host controller | BAR: %x\n", driver->BAR[0]);
            break;
        case 0x0030:
            printk("[USB] XHCI host controller | BAR: %x\n", driver->BAR[0]);
            break;
        default:
            printk("[USB] Unknown host controller ¯\\_(:|)_/¯\n");
            return;
    }

    printk("[USB] IRQ: %2x ", driver->interrupt);
    printk("Revision #: %2x\n", pci_read_word(
        device->bus,
        device->slot,
        device->dev,
        0x60
    ) & 0xFF);
    

    switch(driver->device->progIF){
        case 0x0000:
            break;
        case 0x0010:
            break;
        case 0x0020:
            //init_EHCI(device, driver);
            break;
        case 0x0030:
            break;
    }
}