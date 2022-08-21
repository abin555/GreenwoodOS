#include "uhci.h"

void init_uhci(int pciDriverID){
    printk("UHCI Driver Init @ %2h\n", pciDriverID);
    uint32_t base = pci_drivers[pciDriverID]->BAR[4];
    struct uhci_io_reg* io_reg = (struct uhci_io_reg*) base;
    uint16_t *data = (uint16_t *) base;
    for(int i = 0; i < 10; i++){
        printk("[UHCI] %1x %4x\n", i, *(data+i));
    }
    if(uhci_detect_root(io_reg)){
        printk("[UHCI] controller detected successfully!\n");
    }
    else{
        printk("[UHCI] controller detection failed!\n");
        for(int i = 0; i < 10; i++){
            printk("[UHCI] %1x %4x\n", i, *(data+i));
        }
    }
}

bool uhci_detect_root(struct uhci_io_reg* io_reg){
    struct uhci_io_reg_vals* io_reg_vals = (struct uhci_io_reg_vals*) io_reg;
    //global reset
    io_reg_vals->USBCMD = 0x8000;
    delay(10);//Delay for 10 ms
    io_reg_vals->USBCMD = 0x0000;
    delay(1);//Delay for 1 ms

    if(io_reg_vals->PortSC1 == 0x8000){
        printk("[UHCI] Error: Big-endian controller!\n");
        return false;
    }

    if(io_reg_vals->USBCMD != 0x0000){//Command needs to be default 0x0000
        printk("[UHCI] CMD is not default!\n");
        return false;
    }

    if(io_reg_vals->USBSTS == 0x0020){//Status needs to be default 0x0020
        printk("[UHCI] STS is not default!\n");
        return false;
    }

    io_reg_vals->USBSTS = 0x00FF;//Status is write clear, clearing with 0xFF

    if(io_reg_vals->SOFMOD != 0x40){//SOF needs to be default 0x40
        printk("[UHCI] SOF is not default!\n");
        return false;
    }
    io_reg_vals->USBCMD = 0x0002;//Set HCRESET to 1
    delay(42);
    if(io_reg_vals->USBCMD & 0x0002){//If HCRESET is still set, problem happened
        printk("[UHCI] HCRESET is still set!\n");
        return false;
    }
    return true;
};
