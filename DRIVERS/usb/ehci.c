#include "ehci.h"

#define PERIODIC_BASE 0//CREATE STACK LATER
#define ASYNC_LIST 0//DEFINE LATER

void ehci_init(int PCI_Driver_ID){
    printk("[USB] [EHCI] USB EHCI Host Controller Driver Init\n");
    usb_add_host_controller(PCI_Driver_ID);
    ehci_detect_root(PCI_Driver_ID);

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
    for(unsigned int i = 0; i < 20; i++){
        printk("[USB] [EHCI] %8x", *addr);
        //if((uint32_t) addr == (ehci_BAR + capability_reg->CAPLENGTH)) printk("*");
        if((uint32_t) addr == (uint32_t) operational_reg) printk("*");
        printk("\n");
        addr = (uint32_t *) ((uint32_t) addr + 4);
    }
    ehci_write_oper_reg(ehci_BAR, EHCI_OPS_USBINTR, 0x07);
    ehci_write_oper_reg(ehci_BAR, EHCI_OPS_FRINDEX, 0);
    ehci_write_oper_reg(ehci_BAR, EHCI_OPS_PERIODICLISTBASE, PERIODIC_BASE);
    ehci_write_oper_reg(ehci_BAR, EHCI_OPS_ASYNCLISTADDR, ASYNC_LIST);
    ehci_write_oper_reg(ehci_BAR, EHCI_OPS_CTRLDSSEGMENT, 0);
    ehci_write_oper_reg(ehci_BAR, EHCI_OPS_USBSTS, 0x0000003F);
    ehci_write_oper_reg(ehci_BAR, EHCI_OPS_USBCMD, 0x00080031);
    ehci_write_oper_reg(ehci_BAR, EHCI_OPS_CONFIGFLAG, 1);

    for(int i = 0; i < capability_reg->HCSPARAMS.NPORTS; i++){
        printk("[EHCI] Port %2x\n", i);
        printk("Port Status %1x\n", operational_reg->PORTSC[i].Current_Connect_Status);
        printk("Line Status %1x\n", operational_reg->PORTSC[i].Line_Status);
    }
}
/*
void ehci_reset_port(int PCI_Driver_ID, int port){
    uint32_t ehci_BAR = pci_drivers[PCI_Driver_ID]->BAR[0];
    struct ehci_capability_registers* capability_reg = (struct ehci_capability_registers *) ehci_BAR;
    struct ehci_operational_registers* operational_reg = (struct ehci_operational_registers *) (ehci_BAR + capability_reg->CAPLENGTH);


}*/

struct ehci_PORTSC ehci_read_port(int PCI_Driver_ID, int portnum){
    uint32_t ehci_BAR = pci_drivers[PCI_Driver_ID]->BAR[0];
    struct ehci_capability_registers* capability_reg = (struct ehci_capability_registers *) ehci_BAR;
    struct ehci_operational_registers* operational_reg = (struct ehci_operational_registers *) (ehci_BAR + capability_reg->CAPLENGTH);
    return operational_reg->PORTSC[portnum];
}

bool ehci_detect_root(int PCI_Driver_ID){
    uint32_t timeout = 0xFFFFF;
    uint32_t ehci_BAR = pci_drivers[PCI_Driver_ID]->BAR[0];
    struct ehci_capability_registers* capability_reg = (struct ehci_capability_registers *) ehci_BAR;
    struct ehci_operational_registers* operational_reg = (struct ehci_operational_registers *) (ehci_BAR + capability_reg->CAPLENGTH);

    //Set Run/Stop to be clear
    operational_reg->USBCMD.RS = 0;

    // set hcreset bit
    operational_reg->USBCMD.HCRESET = 1;
    while(operational_reg->USBCMD.HCRESET == 1){
        if(--timeout == 0){
            printk("[USB] [EHCI] Did not reset!\n");
            return false;
        }
    }
    printk("%x\n", *(uint32_t *) &operational_reg->USBCMD);

    return true;
}

void ehci_write_oper_reg(uint32_t base, uint32_t offset, uint32_t value){
    struct ehci_capability_registers* capability_reg = (struct ehci_capability_registers *) base;
    uint32_t *addr = (uint32_t*) (base + capability_reg->CAPLENGTH + offset);
    *addr = value;
}
uint32_t ehci_read_oper_reg(uint32_t base, uint32_t offset){
    struct ehci_capability_registers* capability_reg = (struct ehci_capability_registers *) base;
    uint32_t *addr = (uint32_t*) (base + capability_reg->CAPLENGTH + offset);
    return *addr;
}