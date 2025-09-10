#ifndef USB_EHCI_H
#define USB_EHCI_H

#include "pci.h"
#include "allocator.h"
#include "memory.h"
#include "serial.h"
#include "stdint.h"

struct ehci_capregs {
    uint8_t caplen;
    uint8_t reserved;
    uint16_t hci_version;
    uint32_t hcs_params;
    uint32_t hcc_params;
    uint32_t hcsp_portroute;
};

void *ehci_initialize(struct PCI_driver *usb);

#endif