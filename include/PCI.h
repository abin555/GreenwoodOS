#ifndef PCI_H
#define PCI_H

#include "gcc_stdint.h"
#include "io.h"
#include "memory.h"
#include "string.h"
#include "frame_buffer.h"
#include "drivers.h"

struct __pci_driver;

typedef struct {
	uint32_t vendor;
	uint32_t device;
	uint32_t func;
	uint16_t class;
	struct __pci_driver *driver;
} pci_device;

typedef struct {
	uint32_t vendor;
	uint32_t device;
	uint32_t func;
	uint16_t class;
} pci_device_id;

typedef struct __pci_driver {
	pci_device_id *table;
	char *name;
	pci_device *init_one;
	void (*init_driver)(void);
	void (*exit_driver)(void);
} pci_driver;

void add_pci_device();

uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);
uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceClass(uint16_t bus, uint16_t device, uint16_t function);

void pci_init();
void pci_probe();

#endif