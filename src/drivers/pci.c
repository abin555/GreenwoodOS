#include "pci.h"

void init_pci(){
    printk("[PCI] Initializing\n");
    pci_device_num = pci_driver_num = 0;
    pci_devices = (struct pci_device**) malloc(32 * sizeof(struct pci_device));
    pci_drivers = (struct pci_driver**) malloc(32 * sizeof(struct pci_driver));
    pci_probe();
}

void pci_load_BAR(struct pci_device *pdev, struct pci_driver *driver){
    for(int bar = 0; bar <= 6; bar++){
            uint32_t bar_data = getDeviceBar(
                pdev->bus,
                pdev->slot,
                pdev->dev,
                bar
            ); 
            driver->BAR[bar] = bar_data;
    }
}

void pci_probe(){
    printk("[PCI] Probing PCI Interface\n");
    for(uint32_t bus = 0; bus < 256; bus++){
        for(uint32_t slot = 0; slot < 32; slot++){
            for(uint32_t function = 0; function < 8; function++){
                uint16_t vendor = getVendorID(bus, slot, function);
                if(vendor == 0xFFFF) continue;

                uint16_t device = getDeviceID(bus, slot, function);
                uint16_t class = getDeviceClass(bus, slot, function);
                uint16_t progIF = getDeviceProgIF(bus,slot,function);

                printk("[PCI Device] Vendor: %4x Device: %4x Class: %4x ProgIf: %4x\n", vendor, device, class, progIF);

                struct pci_device *pdev = (struct pci_device*)malloc(sizeof(struct pci_device));
                pdev->bus = bus;
                pdev->slot = slot;
                pdev->dev = function;

                pdev->vendor = vendor;
                pdev->device = device;
                pdev->func = function;
                pdev->class = class;
                pdev->progIF = progIF;
                pdev->driver = 0;

                pci_add_device(pdev);
            }
        }
    }
}

void pci_add_device(struct pci_device *pdev)
{
	pci_devices[pci_device_num] = pdev;
    struct pci_driver *pdrive;
    switch(pdev->class){
        /*
        case 0x0C03://Universal Serial Bus Controller
            //printChar(0,0,'U');
            pdrive = (struct pci_driver *)malloc(sizeof(struct pci_driver));
            pdrive->name = usb_driverName;
            pdrive->init_driver = usb_init_driver;
            pdrive->exit_driver = usb_exit_driver;
            goto generic_install;
        break;
        case 0x0101:
            pdrive = (struct pci_driver *)malloc(sizeof(struct pci_driver));
            pdrive->name = ide_driverName;
            pdrive->init_driver = ide_driver_install;
            goto generic_install;
        break;
        */
        case 0x0106:
            pdrive = (struct pci_driver *)malloc(sizeof(struct pci_driver));
            pdrive->init_driver = initialize_AHCI;
            printk("[PCI Driver] Added AHCI to Driver List\n");
            goto generic_install;
        break;
    }
	pci_device_num++;
	return;

    generic_install:;
    pdrive->device = pdev;
    pdrive->driverID = pci_driver_num;
    pdrive->interrupt = getDeviceInterrupt(
        pdev->bus,
        pdev->slot,
        pdev->dev
    );
    pci_drivers[pci_driver_num] = pdrive;
    pci_load_BAR(pdev, pdrive);
    
    pci_device_num++;
    pci_driver_num++;
    return;    
}

uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset)
{
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outportl (0xCF8, address);
    tmp = (uint16_t)((inportl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

#define PCI_IO_CONFADD 0xcf8
#define PCI_IO_CONFDATA 0xcfc

#define PCI_IO_CONF_CONE 31
#define PCI_IO_CONF_BUSNUM 16
#define PCI_IO_CONF_DEVNUM 11
#define PCI_IO_CONF_FUNCNUM 8
#define PCI_IO_CONF_REGNUM 2

uint32_t pci_read_dword(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
    outdw(
        PCI_IO_CONFADD, 
        bus << PCI_IO_CONF_BUSNUM | 
        slot << PCI_IO_CONF_DEVNUM |
        func << PCI_IO_CONF_FUNCNUM | 
        (offset & 0xfc) |
        1 << PCI_IO_CONF_CONE
    );
	return indw(PCI_IO_CONFDATA);
}

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function)
{
    uint16_t r0 = pci_read_word(bus,device,function,0);
    return r0;
}

uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function)
{
    uint16_t r0 = pci_read_word(bus,device,function,2);
    return r0;
}

uint16_t getDeviceClass(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = pci_read_word(bus, device, function, 0xA);
    return r0;
}
uint8_t getDeviceProgIF(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = pci_read_word(bus, device, function, 8) >> 8;
    return r0;
}
uint8_t getDeviceInterrupt(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = pci_read_word(bus, device, function, 0x3C)>>8;
    return r0;
}


uint32_t getDeviceBar(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){
    uint32_t bar_val = pci_read_dword(bus, device, function, 0x10 + bar*4);
    //return bar_val;
	if (bar_val & 1) { // IO BAR
		return bar_val & 0xfffffffc;
	} else { // Memory BAR
		if ((bar_val & 6) == 4) { // 64 bits
			if (pci_read_dword(bus, device, function, 0x10 + bar * 4 + 4) != 0) {
				return 0; // for BAR above 4G, pretend not exist
			}
		}
		return bar_val & 0xfffffff0;
	}
}