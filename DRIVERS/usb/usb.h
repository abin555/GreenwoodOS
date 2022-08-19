#ifndef USB_H
#define USB_H

#include "frame_buffer.h"
#include "pci.h"
#include "hci.h"
#include "drivers.h"
#include "xhci.h"
#include "ehci.h"

char usb_driverName[31];

enum usb_controller_types{
    UHCI = 0x0,
    OHCI = 0x10,
    EHCI = 0x20,
    XHCI = 0x30
};

struct usb_host_controller{
    uint32_t PCI_Driver_ID;
    uint32_t Base_Address;
    char controller_type;
};

extern struct usb_host_controller usb_host_controllers[4];
uint32_t usb_host_device_num;

void usb_add_host_controller(int PCI_driverID);
void usb_init_driver(int driverID);
void usb_exit_driver();

#endif