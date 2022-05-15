#include "PCI.h"

pci_device **pci_devices = 0;
uint32_t devs = 0;

pci_driver **pci_drivers = 0;
uint32_t drivs = 0;


void add_pci_device(pci_device *pdev)
{
	pci_devices[devs] = pdev;
    pci_driver *pdrive;
    switch(pdev->Class){
        case 0x0C03:;//Universal Serial Bus Controller
            //printChar(0,0,'U');
            pdrive = (pci_driver *)malloc(sizeof(pci_driver));
            pdrive->name = usb_driverName;
            pdrive->init_one = pdev;
            pdrive->driverID = drivs;
            pdrive->init_driver = usb_init_driver;
            pdrive->exit_driver = usb_exit_driver;

            pci_drivers[drivs] = pdrive;
            drivs++;
        break;
        case 0x0101:;
            pdrive = (pci_driver *)malloc(sizeof(pci_driver));
            pdrive->name = ide_driverName;
            pdrive->init_one = pdev;
            pdrive->driverID = drivs;
            pdrive->init_driver = ide_driver_install;
            
            pci_drivers[drivs] = pdrive;
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
    uint16_t r0 = pci_read_word(bus, device, function, 10);
    return r0;
}
uint16_t getDeviceProgIF(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = pci_read_word(bus, device, function, 0x2+0xA);
    return r0;
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