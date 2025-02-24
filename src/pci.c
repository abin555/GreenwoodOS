#include "pci.h"
#include "hda.h"
#include "ethernet.h"
#include "audio.h"


int PCI_numDevices;
struct PCI_device *PCI_devices[50];

int PCI_numDrivers;
struct PCI_driver *PCI_drivers[50];

void PCI_init(){
	print_serial("[PCI] Initializing\n");
	PCI_numDevices = 0;
	PCI_numDrivers = 0;
	PCI_probe();
}

void PCI_probe(){
	print_serial("[PCI] Probing PCI Interface\n");
	for(uint32_t bus = 0; bus < 256; bus++){
		for(uint32_t slot = 0; slot < 32; slot++){
			for(uint32_t device = 0; device < 8; device++){
				uint16_t vendorID = PCI_getVendorID(bus, slot, device);
				if(vendorID == 0xFFFF) continue;

				uint16_t deviceID = PCI_getDeviceID(bus, slot, device);
				uint16_t class_code = PCI_getDeviceClass(bus, slot, device);
				uint16_t progIF = PCI_getDeviceProgIF(bus, slot, device);

				struct PCI_device *pdev = (struct PCI_device *) malloc(sizeof(struct PCI_device));
				pdev->bus = bus;
				pdev->slot = slot;
				pdev->device = device;
				
				pdev->vendorID = vendorID;
				pdev->deviceID = deviceID;
				pdev->class_code = class_code;
				pdev->progIF = progIF;
				pdev->driver = NULL;

				PCI_devices[PCI_numDevices] = pdev;
				PCI_numDevices++;
				PCI_initDevice(pdev);
			}
		}
	}
}

void initialize_AHCI(struct PCI_driver *driver);

void PCI_initDevice(struct PCI_device *pdev){
	print_serial("[PCI] Initializing Device %x:%x -> %x\n", pdev->vendorID, pdev->deviceID, pdev->class_code);
	struct PCI_driver *pdriver;
	switch(pdev->class_code){
		case 0x0C03://USB Controller
			print_serial("[PCI Device] USB Device Identified Type: %x ", pdev->progIF);
			switch(pdev->progIF){
				case 0x00:
					print_serial("UHCI");
					break;
				case 0x10:
					print_serial("OHCI");
					break;
				case 0x20:
					print_serial("EHCI");
					break;
				case 0x30:
					print_serial("XHCI");
					break;
				default:
					print_serial("UNKN");
					break;
			}
			print_serial("\n");
			pdriver = (struct PCI_driver *)malloc(sizeof(struct PCI_driver));
			pdriver->name = "USB Device";
			pdriver->init_driver = NULL;
			goto generic_installation;
			break;
		case 0x0106://AHCI Controller
			print_serial("[PCI Device] AHCI Device Identified\n");
			pdriver = (struct PCI_driver *)malloc(sizeof(struct PCI_driver));
			pdriver->name = "AHCI Device";
			pdriver->init_driver = initialize_AHCI;
			goto generic_installation;
			break;
		case 0x0403:
			print_serial("[PCI Device] Intel High Definition Audio Identified\n");
			pdriver = (struct PCI_driver *)malloc(sizeof(struct PCI_driver));
			pdriver->name = "Intel HDA";
			pdriver->init_driver = hda_init;
			goto generic_installation;
			break;
		case 0x0200:
			print_serial("[PCI Device] Ethernet Identified\n");
			pdriver = (struct PCI_driver *)malloc(sizeof(struct PCI_driver));
			pdriver->name = "Ethernet";
			pdriver->init_driver = ethernet_init;
			goto generic_installation;
			break;
		case 0x0401:
			print_serial("[PCI Device] AC97 Audio Identified\n");
			pdriver = (struct PCI_driver *)malloc(sizeof(struct PCI_driver));
			pdriver->name = "Audio";
			pdriver->init_driver = audio_driver_init;
			goto generic_installation;
			break;
	}
	return;
	generic_installation:;

	pdriver->device = pdev;
	pdev->driver = pdriver;
	pdriver->driverID = PCI_numDrivers;
	pdriver->interrupt = PCI_getDeviceInterrupt(
		pdev->bus,
		pdev->slot,
		pdev->device
	);
	PCI_load_BAR(pdriver);
	for(int i = 0; i < 6; i++){
		print_serial("[PCI BARs] 0x%x\n", pdriver->BAR[i]);
	}
	
	PCI_drivers[PCI_numDrivers] = pdriver;
	print_serial("[PCI Driver] Added Driver ID 0x%x\n", PCI_numDrivers);
	PCI_numDrivers++;
}

uint8_t PCI_read_byte(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset){
    int shift = ((offset & 3) * 8);
    uint32_t val = 0x80000000 |
        (bus << 16) |
        (slot << 11) |
        (device << 8) |
        (offset & 0xFC);
    outdw(0xCF8, val);
    return (indw(0xCFC) >> shift) & 0xFF;
}

uint16_t PCI_read_word(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset)
{
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t ldevice = (uint64_t)device;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (ldevice << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outportl (0xCF8, address);
    tmp = (uint16_t)((inportl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

uint32_t PCI_read_dword(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset){
    outdw(
        PCI_IO_CONFADD, 
        bus << PCI_IO_CONF_BUSNUM | 
        slot << PCI_IO_CONF_DEVNUM |
        device << PCI_IO_CONF_FUNCNUM | 
        (offset & 0xfc) |
        1 << PCI_IO_CONF_CONE
    );
	return indw(PCI_IO_CONFDATA);
}

void PCI_write_byte(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset, uint8_t value){
    uint32_t shift = ((offset & 3) * 8);
    uint32_t val = 0x80000000 |
        (bus << 16) |
        (slot << 11) |
        (device << 8) |
        (offset & 0xFC);
    outdw(0xCF8, val);
    outdw(0xCFC, (indw(0xCFC) & ~(0xFF << shift)) | (value << shift));
}

void PCI_write_word(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset, uint16_t value){
    if((offset & 3) <= 2){
        uint32_t val = 0x80000000 |
            (bus << 16) |
            (slot << 11) |
            (device << 8) |
            (offset & 0xFC);
        outdw(0xCF8, val);
        outdw(0xCFC, value);
    }
    else{
        PCI_write_byte(bus, slot, device, offset+0, (value & 0xFFFF));
        PCI_write_byte(bus, slot, device, offset+2, (value >> 16));
    }
}

void PCI_write_dword(uint32_t bus, uint32_t slot, uint32_t device, uint32_t offset, uint32_t value){
    if((offset & 3) == 0){
        uint32_t shift = ((offset & 3) * 8);
        uint32_t val = 0x80000000 |
            (bus << 16) |
            (slot << 11) |
            (device << 8) |
            (offset & 0xFC);
        outdw(0xCF8, val);
        outdw(0xCFC, (indw(0xCFC) & ~(0xFFFF << shift)) | (value << shift));
    }
    else{
        PCI_write_byte(bus, slot, device, offset+0, (value & 0xFF));
        PCI_write_byte(bus, slot, device, offset+1, (value >> 8));
    }
}

void pci_enable_io_busmastering(uint32_t bus, uint32_t device, uint32_t function) {
 PCI_write_dword(bus, device, function, 0x04, ((PCI_read_dword(bus, device, function, 0x04) & ~(1<<10)) | (1<<2) | (1<<0))); //enable interrupts, enable bus mastering, enable IO space
}

uint16_t PCI_getVendorID(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = PCI_read_word(bus,device,function,0);
    return r0;
}

uint16_t PCI_getDeviceID(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = PCI_read_word(bus,device,function,2);
    return r0;
}

uint16_t PCI_getDeviceClass(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = PCI_read_word(bus, device, function, 0xA);
    return r0;
}
uint8_t PCI_getDeviceProgIF(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = PCI_read_word(bus, device, function, 8) >> 8;
    return r0;
}
uint8_t PCI_getDeviceInterrupt(uint16_t bus, uint16_t device, uint16_t function){
    uint16_t r0 = PCI_read_byte(bus, device, function, 0x3C);
    return r0;
}


uint32_t PCI_getDeviceBar(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){
    uint32_t bar_val = PCI_read_dword(bus, device, function, 0x10 + bar*4);
    //return bar_val;
	if (bar_val & 1) { // IO BAR
		return bar_val & 0xfffffffc;
	} else { // Memory BAR
		if ((bar_val & 6) == 4) { // 64 bits
			if (PCI_read_dword(bus, device, function, 0x10 + bar * 4 + 4) != 0) {
				return 0; // for BAR above 4G, pretend not exist
			}
		}
		return bar_val & 0xfffffff0;
	}
}

void PCI_load_BAR(struct PCI_driver *driver){
	for(int bar = 0; bar < 6; bar++){
		uint32_t bar_data = PCI_getDeviceBar(
			driver->device->bus,
			driver->device->slot,
			driver->device->device,
			bar
		);
		driver->BAR[bar] = bar_data;
	}
}