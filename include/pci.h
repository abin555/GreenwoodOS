#ifndef PCI_H
#define PCI_H

#include "io.h"
#include "stdint.h"
#include "memory.h"
#include "console.h"
#include "ahci.h"
#include "paging.h"
#include "intel_hda.h"
#include "USB.h"

#define BIT(x) (1<<x)
#define PCI_CMD_IO                           BIT(0)
#define PCI_CMD_MMIO                         BIT(1)
#define PCI_CMD_BUSMASTER                    BIT(2)
#define PCI_CMD_SPECIALCYCLES                BIT(3)
#define PCI_CMD_MEMORYWRITEINVALIDATEENABLE  BIT(4)
#define PCI_CMD_VGAPALETTESNOOP              BIT(5)
#define PCI_CMD_PARITYERRORRESPONSE          BIT(6)
#define PCI_CMD_SERRENABLE                   BIT(8)
#define PCI_CMD_FASTBACKTOBACKENABLE         BIT(9)
#define PCI_CMD_INTERRUPTDISABLE             BIT(10)

struct pci_device{
    uint16_t vendor;
    uint16_t device;
    uint16_t func;
    uint16_t class;
    uint16_t progIF;

    uint32_t bus;
    uint32_t slot;
    uint32_t dev;
    uint8_t configuration_offset;
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

uint8_t pci_read_byte(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset);
uint16_t pci_read_word(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset);
uint32_t pci_read_dword(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset);
void pci_write_byte(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset, uint8_t value);
void pci_write_word(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset, uint16_t value);
void pci_write_dword(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset, uint32_t value);

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceClass(uint16_t bus, uint16_t device, uint16_t function);
uint8_t getDeviceProgIF(uint16_t bus, uint16_t device, uint16_t function);
uint32_t getDeviceBar(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);
uint8_t getDeviceInterrupt(uint16_t bus, uint16_t device, uint16_t function);
void pci_load_BAR(struct pci_device *pdev, struct pci_driver *driver);

#endif