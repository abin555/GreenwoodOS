#ifndef HCI_H
#define HCI_H

#include "drivers.h"
#include "console.h"
#include "pci.h"
#include "memory.h"

void init_uhci(int pciDriverID);
void init_ohci(int pciDriverID);
void init_ehci(int pciDriverID);
void init_xhci(int pciDriverID);

#endif