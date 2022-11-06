#ifndef PCI_H
#define PCI_H

#include "io.h"
#include "stdint.h"
#include "memory.h"
#include "console.h"
#include "ahci.h"
#include "paging.h"

struct pci_device{
    uint16_t vendor;
    uint16_t device;
    uint16_t func;
    uint16_t class;
    uint16_t progIF;

    uint32_t bus;
    uint32_t slot;
    uint32_t dev;
    struct pci_driver* driver;
};

struct pci_driver{
    char *name;
    int driverID;
    struct pci_device* device;
    uint32_t BAR[6];
    uint32_t CIS_P;
    uint8_t interrupt;
    void (*init_driver)(int);
    void (*exit_driver)(void);
};

struct pci_device **pci_devices;
struct pci_driver **pci_drivers;

uint32_t pci_device_num;
uint32_t pci_driver_num;

void init_pci();
void pci_probe();
void pci_add_device(struct pci_device* pdev);

uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);
uint32_t pci_read_dword(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceClass(uint16_t bus, uint16_t device, uint16_t function);
uint8_t getDeviceProgIF(uint16_t bus, uint16_t device, uint16_t function);
uint32_t getDeviceBar(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);
uint8_t getDeviceInterrupt(uint16_t bus, uint16_t device, uint16_t function);
void pci_load_BAR(struct pci_device *pdev, struct pci_driver *driver);

#endif