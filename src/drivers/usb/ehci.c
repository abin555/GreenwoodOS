#include "EHCI.h"

#define EHCI_IRQ 7

void init_EHCI(struct pci_device* device, struct pci_driver* driver){
    struct EHCI_controller *controller = (struct EHCI_controller *) malloc(sizeof(struct EHCI_controller));

    controller->pci_device = device;
    controller->pci_driver = driver;

    controller->mmio = driver->BAR[0];
    create_page_entry(controller->mmio, controller->mmio, 0x83);

    controller->operationalRegs_offset = get_8_offset(controller->mmio, 0);
    if(!ehci_detect_root(controller)){
        printk("[EHCI] Controller not redetected :(\n");
        free(controller);
        return;
    }

    controller->N_PTT = EHCI_get_HCSPARAMS(controller, N_PTT);
    controller->N_TT = EHCI_get_HCSPARAMS(controller, N_TT);
    controller->version = get_16_offset(controller->mmio, 2);
    controller->N_CC = EHCI_get_HCSPARAMS(controller, N_CC);
    controller->N_PORTS = EHCI_get_HCSPARAMS(controller, N_PORTS);
    controller->ecap_offset = EHCI_get_HCCPARAMS(controller, EECP);
    controller->IsochronousSchedThreshold = EHCI_get_HCCPARAMS(controller, IsochronousSchedThreshold);
    switch(EHCI_get_USBCMD(controller, FrameListSize)){
        case 0b00:
            controller->framesize = 1024;
            break;
        case 0b01:
            controller->framesize = 512;
            break;
        case 0b10:
            controller->framesize = 256;
            break;
    }
    ehci_analyze(controller);
    ehci_start(controller);
}

void ehci_start(struct EHCI_controller* e){
    printk("[EHCI] Start EHCI Controller\n");
    ehci_initHC(e);
    ehci_resetHC(e);
    ehci_startHC(e);
}

void ehci_startHC(struct EHCI_controller* e){
    printk("Start\n");
    ehci_deactivateLegacySupport(e);

    EHCI_set_operReg(e, CTRLDSSSegment, 0);

    ehci_initializePeriodicList(e);

}

void ehci_initializePeriodicList(struct EHCI_controller* e){
    EHCI_set_USBCMD(e, FrameListSize, CMD_FRAMELIST_1024);
    e->PeriodicBase = (((uint32_t) malloc(sizeof(uint32_t)*1024+0x400) + 0x400) & ~0x7FF);
    printk("Periodic Base: %x\n", e->PeriodicBase);
    EHCI_set_operReg(e, PeriodicListBase, get_physical(e->PeriodicBase));
}

void ehci_resetHC(struct EHCI_controller* e){
    printk("Reset\n");
    EHCI_set_USBCMD(e, RS, 0);
    while(!EHCI_get_USBSTS(e, HCHalted)){
        delay(10);
    }
    EHCI_set_USBCMD(e, HCRESET, 1);
    uint32_t timeout = 500;
    while((EHCI_get_USBCMD(e, 0xFFFFFFFF) & HCRESET) == 0){
        if(timeout == 0){
            printk("Timeout Error: Reset still set to 1\n");
            break;
        }
        delay(1);
        timeout--;
    }
}

void ehci_initHC(struct EHCI_controller* e){
    printk("Initialize\n");
    uint16_t pciCommandRegister = pci_read_word(
        e->pci_device->bus,
        e->pci_device->slot,
        e->pci_device->dev,
        0x04
    );
    pci_write_word(
        e->pci_device->bus,
        e->pci_device->slot,
        e->pci_device->dev,
        0x04,
        pciCommandRegister | PCI_CMD_MMIO | PCI_CMD_BUSMASTER//Resets status
    );
    uint8_t pciCapabilitiesList = pci_read_byte(
        e->pci_device->bus,
        e->pci_device->slot,
        e->pci_device->dev,
        0x34
    );
    printk("PCI Command Register Before: %x\n", pciCommandRegister);
    printk("PCI Comamnd Register After: %x\n", pci_read_word(
        e->pci_device->bus,
        e->pci_device->slot,
        e->pci_device->dev,
        0x04
    ));
    printk("PCI Capabilities List: %x\n", pciCapabilitiesList);
    interrupt_add_handle(e->pci_driver->interrupt, ehci_handler);
}

void ehci_deactivateLegacySupport(struct EHCI_controller* e){
    struct pci_device* dev = e->pci_device;
    uint8_t eecp = EHCI_get_HCCPARAMS(e, EECP);

    printk("DeactivateLegacySupport: eecp = %2x\n",eecp);

    if(eecp >= 0x40){
        uint8_t eecp_id = 0;
        while(eecp){
            printk("eecp = %x\n", eecp);
            eecp_id = pci_read_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                eecp
            );
            printk("eecp_id = %2x\n", eecp_id);
            if(eecp_id == 1){
                break;
            }
            eecp = pci_read_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                eecp + 1
            );
        }
        uint8_t BIOSownedSemaphore = eecp + 2; // R/W - only Bit 16 (Bit 23:17 Reserved, must be set to zero)
        uint8_t OSownedSemaphore   = eecp + 3; // R/W - only Bit 24 (Bit 31:25 Reserved, must be set to zero)
        uint8_t USBLEGCTLSTS       = eecp + 4; // USB Legacy Support Control/Status (DWORD, cf. EHCI 1.0 spec, 2.1.8)
        if(eecp_id == 1 && (pci_read_byte(
            dev->bus,
            dev->slot,
            dev->dev,
            BIOSownedSemaphore) & 0x01)
        ){
            printk("Check: BIOSownedSemaphore: %2x OSownedSemaphore %2x\n",
                pci_read_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                BIOSownedSemaphore),
                pci_read_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                OSownedSemaphore)
            );
            printk("Set OS-Semaphore\n");
            pci_write_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                OSownedSemaphore, 0x01
            );
            uint32_t timeout = 250;
            while((pci_read_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                BIOSownedSemaphore) & 0x1) == 0
            ){
                if(timeout == 0){
                    printk("BIOS-Semaphore still set.\n");
                    break;
                }
                timeout--;
                delay(1);
            }
            timeout = 250;
            while((pci_read_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                OSownedSemaphore) & 0x1) != 0
            ){
                if(timeout == 0){
                    printk("OS-Semaphore still set.\n");
                    break;
                }
                timeout--;
                delay(1);
            }
            printk("Check: BIOSownedSemaphore: %2x OSownedSemaphore %2x\n",
                pci_read_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                BIOSownedSemaphore),
                pci_read_byte(
                dev->bus,
                dev->slot,
                dev->dev,
                OSownedSemaphore)
            );

            pci_write_dword(
                dev->bus,
                dev->slot,
                dev->dev,
                USBLEGCTLSTS,
                0x0
            );
        }
        else{
            printk("BIOS did not own the EHCI.\n");
        }
    }
    else{
        printk("No Valid EECP found");
    }
}

void ehci_analyze(struct EHCI_controller* e){
    printk("    BAR: %x\n", e->mmio);
    printk("    HCIVERSION: %x\n", e->version);
    printk("    HCSPARAMS: %x\n", EHCI_get_HCSPARAMS(e, 0xFFFFFFFF));
    printk("    Ports: %2x\n", e->N_PORTS);
    printk("    HCCPARAMS: %x\n", EHCI_get_HCCPARAMS(e, 0xFFFFFFFF));
    if(e->ecap_offset == 0)printk(" No Extended Capabilities\n");
    printk("    OpRegs Offset: %2x\n", e->operationalRegs_offset);
}


bool ehci_detect_root(struct EHCI_controller* controller){
    uint32_t timeout = 1000;
    EHCI_set_USBCMD(controller, RS, 0);
    EHCI_set_USBCMD(controller, HCRESET, 1);
    while(EHCI_get_USBCMD(controller, HCRESET)){
        if(--timeout == 0){
            printk("[EHCI] Did not reset in time!\n");
            return false;
        }
        delay(1);
    }
    if(EHCI_get_HCCPARAMS(controller, ProgrammableFrameListFlag) && EHCI_get_operReg(controller, USBCMD) != 0x0080B00) return false;
    if(!EHCI_get_HCCPARAMS(controller, ProgrammableFrameListFlag) && EHCI_get_operReg(controller, USBCMD) != 0x0080000) return false;
    if(EHCI_get_operReg(controller, USBSTS) != 0x00001000) return false;
    if(EHCI_get_operReg(controller, USBINTR) != 0x00000000) return false;
    if(EHCI_get_operReg(controller, FRINDEX) != 0x00000000) return false;
    if(EHCI_get_operReg(controller, CTRLDSSSegment) != 0x00000000) return false;
    if(EHCI_get_operReg(controller, ConfigFlag) != 0x00000000) return false;
    return true;   
}

struct cpu_state ehci_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    printk("[EHCI] Interrupt!\n");
    return cpu;
}









uint32_t EHCI_get_HCSPARAMS(struct EHCI_controller* controller, EHCI_HCSPARAMS field){
    uint32_t HCSPARAMS = get_32_offset(controller->mmio, EHCI_HCSPARAMS_OFFSET);
    return get_bitfield(HCSPARAMS, field);
}

uint32_t EHCI_get_HCCPARAMS(struct EHCI_controller* controller, EHCI_HCCPARAMS field){
    uint32_t HCCPARAMS = get_32_offset(controller->mmio, EHCI_HCCPARAMS_OFFSET);
    return get_bitfield(HCCPARAMS, field);
}

uint32_t EHCI_get_USBCMD(struct EHCI_controller* controller, EHCI_USBCMD field){
    uint32_t USBCMD = get_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_USBCMD_OFFSET);
    return get_bitfield(USBCMD, field);
}
uint32_t EHCI_set_USBCMD(struct EHCI_controller* controller, EHCI_USBCMD field, uint32_t value){
    uint32_t USBCMD = get_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_USBCMD_OFFSET);
    USBCMD &= ~field;
    USBCMD |= (value << calculate_shift(field));
    set_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_USBCMD_OFFSET, USBCMD);
    return USBCMD;
}

uint32_t EHCI_get_USBSTS(struct EHCI_controller* controller, EHCI_USBSTS field){
    uint32_t USBSTS = get_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_USBSTS_OFFSET);
    return get_bitfield(USBSTS, field);
}

uint32_t EHCI_get_USBINTR(struct EHCI_controller* controller, EHCI_USBINTR field){
    uint32_t USBINTR = get_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_USBINTR_OFFSET);
    return get_bitfield(USBINTR, field);
}
uint32_t EHCI_set_USBINTR(struct EHCI_controller* controller, EHCI_USBINTR field, uint32_t value){
    uint32_t USBINTR = get_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_USBINTR_OFFSET);
    USBINTR &= ~field;
    USBINTR |= (value << calculate_shift(field));
    set_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_USBINTR_OFFSET, USBINTR);
    return USBINTR;
}

uint32_t EHCI_get_FRINDEX(struct EHCI_controller* controller){
    return get_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_FRINDEX_OFFSET) & 0x3FFF;
}
uint32_t EHCI_set_FRINDEX(struct EHCI_controller* controller, uint32_t value){
    set_32_offset(
        controller->mmio+controller->operationalRegs_offset, 
        EHCI_FRINDEX_OFFSET, 
        (get_32_offset(controller->mmio+controller->operationalRegs_offset, EHCI_FRINDEX_OFFSET) & ~0x3FFF) | (value & 0x3FFF));
    return EHCI_get_FRINDEX(controller);
}

uint32_t EHCI_get_operReg(struct EHCI_controller* controller, EHCI_OperationalRegister offset){
    return get_32_offset(controller->mmio+controller->operationalRegs_offset, offset);
}
uint32_t EHCI_set_operReg(struct EHCI_controller* controller, EHCI_OperationalRegister offset, uint32_t value){
    set_32_offset(controller->mmio+controller->operationalRegs_offset, offset, value);
    return EHCI_get_operReg(controller, offset);
}

uint32_t EHCI_get_portSC(struct EHCI_controller* controller, uint32_t port, EHCI_PORTSC field){
    uint32_t PORTSC = get_32_offset(controller->mmio + controller->operationalRegs_offset, PORTSC_BASE+ (4*port));
    //return get_bitfield(PORTSC, field);
    return PORTSC & field;
}
uint32_t EHCI_set_portSC(struct EHCI_controller* controller, uint32_t port, EHCI_PORTSC field, uint32_t value){
    uint32_t PORTSC = get_32_offset(controller->mmio + controller->operationalRegs_offset, PORTSC_BASE+ (4*port));
    PORTSC &= ~field;
    PORTSC |= (value << calculate_shift(field));
    set_32_offset(controller->mmio+controller->operationalRegs_offset, PORTSC_BASE + (4*port), PORTSC);
    return EHCI_get_portSC(controller, port, field);
}
