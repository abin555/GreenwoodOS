#ifndef RTL8139_H
#define RTL8139_H
#include "pci.h"
#include "stdint.h"
#include "serial.h"
#include "interrupts.h"
#include "memory.h"
#include "paging.h"

struct ethernet_driver *rtl8139_init(struct PCI_driver *driver);

#endif