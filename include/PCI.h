#ifndef PCI_H
#define PCI_H

#include "gcc_stdint.h"
#include "io.h"
#include "memory.h"
#include "string.h"
#include "frame_buffer.h"
#include "drivers.h"

struct __pci_driver;
struct __pci_device;

typedef struct __pci_device{
	uint16_t vendor;
	uint16_t device;
	uint16_t func;
	uint16_t Class;
	uint16_t progIF;
	//PCI Location Data
	uint32_t bus;
	uint32_t slot;
	uint32_t dev;
	struct __pci_driver *driver;
} pci_device;

typedef struct __pci_driver {
	char *name;
	int driverID;
	pci_device *init_one;
	uint32_t BAR[6];
	uint32_t CIS_P;
	void (*init_driver)(int);
	void (*exit_driver)(void);
} pci_driver;

pci_device **pci_devices;
pci_driver **pci_drivers;
uint32_t devs;
uint32_t drivs;

void pci_load_BAR(pci_device *pdev, pci_driver *driver);
void add_pci_device();

uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);
uint32_t pci_read_dword(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceClass(uint16_t bus, uint16_t device, uint16_t function);
uint8_t getDeviceProgIF(uint16_t bus, uint16_t device, uint16_t function);
uint32_t getDeviceBar(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);

void pci_init();
void pci_probe();

#endif