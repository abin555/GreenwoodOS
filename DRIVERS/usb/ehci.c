#include "ehci.h"

void ehci_init(int PCI_Driver_ID){
    printk("[USB] [EHCI] USB EHCI Host Controller Driver Init\n");
    uint32_t ehci_BAR = pci_drivers[PCI_Driver_ID]->BAR[0];
    struct ehci_capability_registers* capability_reg = (struct ehci_capability_registers *) ehci_BAR;
    struct ehci_operational_registers* operational_reg = (struct ehci_operational_registers *) (ehci_BAR + capability_reg->CAPLENGTH);
    printk("[USB] [EHCI] CAPLEN: %2x Version: ", capability_reg->CAPLENGTH);
    switch(capability_reg->HCIVERSION){
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
    }
    printk("[USB] [EHCI] PRR: %x Num Ports: %1x\n", capability_reg->HCSPARAMS.Port_Routing_Rules, capability_reg->HCSPARAMS.NPORTS);
    uint32_t* addr = (uint32_t *) ehci_BAR;
    for(int i = 0; i < 20; i++){
        printk("[USB] [EHCI] %8x", *addr);
        //if((uint32_t) addr == (ehci_BAR + capability_reg->CAPLENGTH)) printk("*");
        if((uint32_t) addr == (uint32_t) operational_reg) printk("*");
        printk("\n");
        addr = (uint32_t *) ((uint32_t) addr + 4);
    }
}

struct ehci_PORTSC ehci_read_port(int PCI_Driver_ID, int portnum){
    uint32_t ehci_BAR = pci_drivers[PCI_Driver_ID]->BAR[0];
    struct ehci_capability_registers* capability_reg = (struct ehci_capability_registers *) ehci_BAR;
    struct ehci_operational_registers* operational_reg = (struct ehci_operational_registers *) (ehci_BAR + capability_reg->CAPLENGTH);
    return operational_reg->PORTSC[portnum];
}