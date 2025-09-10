#include "usb.h"
#include "ehci.h"
#include "stddef.h"

int USB_numControllers = 0;
struct USB_controller USB_controllers[USB_MAX_INTERFACES];

void USB_initialize(struct PCI_driver *usb){
    print_serial("[USB] Initializing Device - Type: ");
    USB_CONTROLLER_TYPE conn_type;
    switch(usb->device->progIF){
        case 0x00:
            print_serial("UHCI");
            conn_type = USB_UHCI;
            break;
        case 0x10:
            print_serial("OHCI");
            conn_type = USB_OHCI;
            break;
        case 0x20:
            print_serial("EHCI");
            conn_type = USB_EHCI;
            break;
        case 0x30:
            print_serial("XHCI");
            conn_type = USB_XHCI;
            break;
        default:
            print_serial("UNKN\n");
            return;
            break;
    }
    print_serial(" (%d) \n", conn_type);
    struct USB_controller *controller = &USB_controllers[USB_numControllers];
    controller->type = conn_type;
    controller->busid = USB_numControllers;
    switch(conn_type){
        case USB_XHCI:

            break;
        case USB_EHCI:
            controller->interface = ehci_initialize(usb);
            break;
        default:
            break;
    }
    if(controller->interface != NULL){
        USB_numControllers++;
    }
}