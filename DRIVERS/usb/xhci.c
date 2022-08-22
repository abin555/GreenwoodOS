#include "xhci.h"

uint32_t xhci_get_capability_register(uint32_t BaseAddr, int component){
    uint32_t temp_holder;
    switch(component){
        case CAPLENGTH:
            return 0xFF & *((uint8_t*) BaseAddr);
            break;
        case HCIVERSION:
            temp_holder = *((uint32_t*) (BaseAddr));
            return 0xFFFF & (temp_holder >> 16);
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
uint32_t xhci_get_operational_register(uint32_t BaseAddr, int offset){
    uint32_t oper_addr = BaseAddr + xhci_get_capability_register(BaseAddr, CAPLENGTH);
    uint32_t reg_addr = oper_addr + offset;
    return *((uint32_t*) reg_addr);
}

void xhci_write_operational_register(uint32_t BaseAddr, int offset, uint32_t value){
    uint32_t oper_addr = BaseAddr + xhci_get_capability_register(BaseAddr, CAPLENGTH);
    uint32_t reg_addr = oper_addr + offset;
    *((uint32_t*) reg_addr) = value;
}

void xhci_init(uint32_t base_address, uint8_t irq){
    uint32_t Base_Addr_Reg = base_address;
    xhci_capability_regs* capability_regs = (xhci_capability_regs*) Base_Addr_Reg;
    uint32_t HCSPARAMS1_reg = xhci_get_capability_register(Base_Addr_Reg, HCSPARAMS1);
    xhci_operational_regs* operational_regs = (xhci_operational_regs *) (Base_Addr_Reg + capability_regs->CAPLENGTH);
    printk("[USB] [XHCI] Initialization: %1x %x %x %2x %2x\n", irq, capability_regs, operational_regs, capability_regs->CAPLENGTH, xhci_get_capability_register(base_address, CAPLENGTH));
    printk("[USB] [XHCI] %x %x\n", sizeof(xhci_capability_regs), sizeof(xhci_operational_regs));
    printk("[USB] [XHCI] Version: ");
    switch(xhci_get_capability_register(base_address, HCIVERSION)){
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
            printk("%4x\n", xhci_get_capability_register(base_address, HCIVERSION));
            break;
    }
    printk("[USB] [XHCI] Max Device Slots: %2x\n", (HCSPARAMS1_reg & 0xFF000000) >> 24);
    printk("[USB] [XHCI] Number of Interrupts : %2x\n", capability_regs->HCSPARAMS1.MaxIntrs);
    printk("[USB] [XHCI] Max Ports: %2x\n", HCSPARAMS1_reg & 0xFF);
    printk("[USB] [XHCI] Page Size: %16b\n", xhci_get_operational_register(Base_Addr_Reg, 0x08));
}
/*
bool det_xhci_root(uint32_t BaseAddr){
    
}
*/