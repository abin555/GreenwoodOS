#include "PCI.h"

pci_device **pci_devices = 0;
uint32_t devs = 0;

pci_driver **pci_drivers = 0;
uint32_t drivs = 0;

void pci_load_header0(pci_driver *pdrive, pci_header0 *header){
    for(int bar = 0; bar <= 5; bar++){
        header->BAR[bar] = getDeviceBar(
            pdrive->init_one->device_id->bus,
            pdrive->init_one->device_id->slot,
            pdrive->init_one->device_id->func,
            bar
        );
    }
}

void add_pci_device(pci_device *pdev)
{
	pci_devices[devs] = pdev;
    pci_driver *pdrive;
    pci_header0 *pheader0;
    switch(pdev->Class){
        case 0x0C03:;//Universal Serial Bus Controller
            //printChar(0,0,'U');
            pdrive = (pci_driver *)malloc(sizeof(pci_driver));
            pheader0 = (pci_header0 *)malloc(sizeof(pci_header0));
            pdrive->name = usb_driverName;
            pdrive->init_one = pdev;
            pdrive->driverID = drivs;
            pdrive->init_driver = usb_init_driver;
            pdrive->exit_driver = usb_exit_driver;
            pdrive->header = *pheader0;

            pci_drivers[drivs] = pdrive;
            pci_load_header0(pdrive, pheader0);
            drivs++;
        break;
        case 0x0102:;
            pdrive = (pci_driver *)malloc(sizeof(pci_driver));
            pheader0 = (pci_header0 *)malloc(sizeof(pci_header0));
            pdrive->name = ide_driverName;
            pdrive->init_one = pdev;
            pdrive->driverID = drivs;
            pdrive->init_driver = ide_driver_install;
            pdrive->header = *pheader0;
            
            pci_drivers[drivs] = pdrive;
            pci_load_header0(pdrive, pheader0);
            drivs++;
        break;
    }
	devs ++;
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

void pci_probe()
{
    int DebugLine = 0;
	for(uint32_t bus = 0; bus < 256; bus++)
    {
        for(uint32_t slot = 0; slot < 32; slot++)
        {
            for(uint32_t function = 0; function < 8; function++)
            {
                    uint16_t vendor = getVendorID(bus, slot, function);
                    if(vendor == 0xffff) continue;
                    uint16_t device = getDeviceID(bus, slot, function);
                    uint16_t Class = getDeviceClass(bus, slot, function);
                    uint16_t progIF = getDeviceProgIF(bus,slot,function);

                    fb_write_xy("vendor:", 7, 0, 0, DebugLine);
                    decodeHex(STR_edit, vendor, 16, 0);
                    fb_write_xy(STR_edit, 5, 0, 7, DebugLine);
                    fb_write_xy(" device: ", 10, 0, 12, DebugLine);
                    decodeHex(STR_edit, device, 16, 0);
                    fb_write_xy(STR_edit, 5, 0, 20, DebugLine);
                    fb_write_xy(" class: ", 9, 0, 25, DebugLine);
                    decodeHex(STR_edit, Class, 16, 0);
                    fb_write_xy(STR_edit, 5, 0, 32, DebugLine);
                    DebugLine++;

                    pci_device *pdev = (pci_device *)malloc(sizeof(pci_device));
                    pci_device_id *pdev_id = (pci_device_id *)malloc(sizeof(pci_device_id));
                    pdev_id->bus = bus;
                    pdev_id->slot = slot;
                    pdev_id->func = function;


                    pdev->vendor = vendor;
                    pdev->device = device;
                    pdev->func = function;
                    pdev->Class = Class;
                    pdev->progIF = progIF;
                    pdev->driver = 0;
                    pdev->device_id = pdev_id;
                    add_pci_device(pdev);
            }
        }
    }
}


void pci_init()
{
	devs = drivs = 0;
	pci_devices = (pci_device **)malloc(32 * sizeof(pci_device));
	pci_drivers = (pci_driver **)malloc(32 * sizeof(pci_driver));
	pci_probe();
}