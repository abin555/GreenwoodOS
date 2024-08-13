#ifndef USB_H
#define USB_H

#include "pci.h"
#include "allocator.h"
#include "memory.h"
#include "serial.h"
#include "stdint.h"

void USB_initialize(struct PCI_driver *usb);

#endif