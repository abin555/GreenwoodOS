#ifndef PCI_H
#define PCI_H

#include "io.h"
#include "stdint.h"
#include "allocator.h"
#include "serial.h"
#include "paging.h"

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

#define PCI_IO_CONFADD 0xcf8
#define PCI_IO_CONFDATA 0xcfc

#define PCI_IO_CONF_CONE 31
#define PCI_IO_CONF_BUSNUM 16
#define PCI_IO_CONF_DEVNUM 11
#define PCI_IO_CONF_FUNCNUM 8
#define PCI_IO_CONF_REGNUM 2

struct PCI_command_reg {
    bool io_space: 1;
    bool memory_space: 1;
    bool bus_master: 1;
    bool special_cycles: 1;
    bool memory_write_and_invalidate: 1;
    bool vga_palette_snoop: 1;
    bool parity_error_response: 1;
    bool reserved: 1;
    bool serr_enable: 1;
    bool fast_back_to_back_enable: 1;
    bool interrupt_disable: 1;
    uint8_t reserved2: 5;
};

struct PCI_device{
	uint16_t vendorID;
	uint16_t deviceID;
	uint16_t class_code;
	uint16_t progIF;
	/*
	Header Types:
	0x0 = PCI general device <_/
	0x1 = PCI to PCI bridge X
	0x2 = PCI to CardBus bridge X
	*/
	uint8_t header_type;

	uint32_t bus;
	uint32_t slot;
	uint32_t device;

	uint8_t configuration_offset;
	struct PCI_driver *driver;
};

struct PCI_driver{
	char *name;
	int driverID;
	struct PCI_device *device;
	uint32_t BAR[6];
	uint8_t interrupt;
	void *driver_data;
	void (*init_driver)(struct PCI_driver *);
	void (*exit_driver)(struct PCI_driver *);
};

extern struct PCI_device *PCI_devices[50];
extern struct PCI_driver *PCI_drivers[50];
extern int PCI_numDevices;
extern int PCI_numDrivers;

void PCI_init();
void PCI_probe();
void PCI_initDevice(struct PCI_device *pdev);

uint8_t PCI_read_byte(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset);
uint16_t PCI_read_word(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset);
uint32_t PCI_read_dword(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset);

void PCI_write_byte(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset, uint8_t value);
void PCI_write_word(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset, uint16_t value);
void PCI_write_dword(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset, uint32_t value);

uint16_t PCI_getVendorID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t PCI_getDeviceID(uint16_t bus, uint16_t device, uint16_t function);
uint16_t PCI_getDeviceClass(uint16_t bus, uint16_t device, uint16_t function);
uint8_t PCI_getDeviceProgIF(uint16_t bus, uint16_t device, uint16_t function);
uint8_t PCI_getDeviceInterrupt(uint16_t bus, uint16_t device, uint16_t function);
uint32_t PCI_getDeviceBar(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);

void PCI_load_BAR(struct PCI_driver *driver);
#endif