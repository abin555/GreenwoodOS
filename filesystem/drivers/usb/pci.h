#ifndef PCI_H
#define PCI_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

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

	uint32_t BAR[6];

	uint8_t configuration_offset;
	struct PCI_driver *driver;
};

struct PCI_driver{
	char *name;
	int driverID;
	struct PCI_device *device;
	uint8_t interrupt;
	void *driver_data;
	void (*init_driver)(struct PCI_driver *);
	void (*exit_driver)(struct PCI_driver *);
};

FILE *pci_open_devices();
struct PCI_device *pci_getDev(FILE *pci, uint16_t class, uint16_t progIF);


#endif