#ifndef USB_H
#define USB_H

#include "memory.h"
#include "console.h"
#include "pci.h"
#include "paging.h"
#include "utilities.h"
#include "EHCI.h"

typedef enum{
    OHCI = 0,
    UHCI = 1,
    EHCI = 2,
    XHCI = 3
} USB_controller_type;

struct USB_controller{
    USB_controller_type type;
    void *controller_def_enum;
};

struct USB_controller *USB_controllers[0xF];
uint8_t num_USB_controllers = 0;

void usb_init_driver(int driverID);
#endif