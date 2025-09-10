#ifndef USB_H
#define USB_H

#include "pci.h"
#include "allocator.h"
#include "memory.h"
#include "serial.h"
#include "stdint.h"

typedef enum {
    USB_OHCI,
    USB_UHCI,
    USB_EHCI,
    USB_XHCI
} USB_CONTROLLER_TYPE;

struct USB_controller {
    USB_CONTROLLER_TYPE type;
    void *interface;
    int busid;
};

#define USB_MAX_INTERFACES 4

extern int USB_numControllers;
extern struct USB_controller USB_controllers[USB_MAX_INTERFACES];

void USB_initialize(struct PCI_driver *usb);

#endif