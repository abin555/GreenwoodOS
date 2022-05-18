#ifndef PCI_H
#define PCI_H

#include "gcc_stdint.h"
#include "io.h"
#include "memory.h"
#include "string.h"
#include "frame_buffer.h"
#include "drivers.h"

struct __pci_driver;
struct __pci_device_id;
struct __pci_device;
struct __pci_header0;

typedef struct __pci_device{
	uint16_t vendor;
	uint16_t device;
	uint16_t func;
	uint16_t Class;
	uint16_t progIF;
	struct __pci_driver *driver;
	struct __pci_device_id *device_id;
} pci_device;

typedef struct __pci_header0{
	uint32_t BAR[5];
	uint32_t CIS_P;
} pci_header0;

typedef struct __pci_device_id{
	uint32_t bus;
	uint32_t slot;
	uint32_t func;
} pci_device_id;

typedef struct __pci_driver {
	char *name;
	int driverID;
	pci_device *init_one;
	pci_header0 *header;
	void (*init_driver)(int, int);
	void (*exit_driver)(void);
} pci_driver;

pci_device **pci_devices;
pci_driver **pci_drivers;
uint32_t devs;
uint32_t drivs;

void pci_load_header0(pci_driver *pdrive, pci_header0 *header);
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