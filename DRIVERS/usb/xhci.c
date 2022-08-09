#include "xhci.h"

uint32_t xhci_get_capability_register(int PCI_DriverID, int component){
    pci_driver* xhci_PCI_driver = pci_drivers[PCI_DriverID];
    uint32_t BaseAddr = xhci_PCI_driver->BAR[0];
    switch(component){
        case CAPLENGTH:
            return 0xFF & *((uint8_t*) BaseAddr);
            break;
        case HCIVERSION:
            return 0xFFFF & *((uint16_t*) (BaseAddr + 0x2));
            break;
        case HCSPARAMS1:
            return 0xFFFFFFFF & *((uint32_t*) (BaseAddr + 0x4));
            break;
        case HCSPARAMS2:
            return 0xFFFFFFFF & *((uint32_t*) (BaseAddr + 0x8));
            break;
        case HCSPARAMS3:
            return 0xFFFFFFFF & *((uint32_t*) (BaseAddr + 0xC));
            break;
        case HCCPARAMS1:
            return 0xFFFFFFFF & *((uint32_t*) (BaseAddr + 0x10));
            break;
        case DBOFF:
            return 0xFFFFFFFF & *((uint32_t*) (BaseAddr + 0x14));
            break;
        case RTSOFF:
            return 0xFFFFFFFF & *((uint32_t*) (BaseAddr + 0x18));
            break;
        case HCCPARAMS2:
            if((0xFFFF & *((uint16_t*) (BaseAddr + 0x2))) > 0x0100){
                return 0xFFFFFFFF & *((uint32_t*) (BaseAddr + 0x1C));
                break;
            }
            return 0;
            break;
        case VTIOSOFF:
            if((0xFFFF & *((uint16_t*) (BaseAddr + 0x2))) > 0x0110){
                return 0xFFFFFFFF & *((uint32_t*) (BaseAddr + 0x20));
                break;
            }
            return 0;
            break;
    }
    return 0;
}

void xhci_init(int PCI_DriverID){
    pci_driver* xhci_PCI_driver = pci_drivers[PCI_DriverID];
    uint32_t Base_Addr_Reg = xhci_PCI_driver->BAR[0];
    xhci_capability_regs* capability_regs = (xhci_capability_regs*) Base_Addr_Reg;
    xhci_operational_regs* operational_regs = (xhci_operational_regs *) (Base_Addr_Reg + capability_regs->CAPLENGTH);
    printk("[USB] [XHCI] Initialization: %2x %x %x %2x %2x\n", PCI_DriverID, capability_regs, operational_regs, capability_regs->CAPLENGTH, xhci_get_capability_register(PCI_DriverID, CAPLENGTH));
    printk("[USB] [XHCI] %x %x\n", sizeof(xhci_capability_regs), sizeof(xhci_operational_regs));
    printk("[USB] [XHCI] Version: ");
    switch(xhci_get_capability_register(PCI_DriverID, HCIVERSION)){
        case 0x0095:
            printk("0.95\n");
            break;
        case 0x0096:
            printk("0.96\n");
            break;
        case 0x0100:
            printk("1.00\n");
            break;
        case 0x0110:
            printk("1.10\n");
            break;
        default:
            printk("???\n");
            break;
    }

    printk("[USB] [XHCI] Max Device Slots: %2x\n", capability_regs->HCSPARAMS1.MaxPorts);
    printk("[USB] [XHCI] Number of Interrupts : %3x\n", capability_regs->HCSPARAMS1.MaxIntrs);
    printk("[USB] [XHCI] Max Ports: %2x\n", capability_regs->HCSPARAMS1.MaxPorts);
    printk("[USB] [XHCI] Page Size: %16b\n", operational_regs->pagesize.PageSize);
    for(uint32_t* addr = (uint32_t *) operational_regs-2, i = 0; i < 12; i++){
        addr++;
        printk("[USB] [XHCI] %8x %32b %8x\n", (uint32_t) addr, *addr, *addr);
    }
}