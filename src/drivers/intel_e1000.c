#include "intel_e1000.h"

void e1000_writeCommand(struct INTEL_E1000 *e1000, uint16_t p_address, uint32_t p_value){
    if(e1000->bar_type == 0){
        uint32_t *addr = (uint32_t *) (e1000->mem_base + p_address);
        *addr = p_value;
    }
    else{
        outportl(e1000->io_base, p_address);
        outportl(e1000->io_base + 4, p_value);
    }
}

uint32_t e1000_readCommand(struct INTEL_E1000 *e1000, uint16_t p_address){
    if(e1000->bar_type == 0){
        uint32_t *addr = (uint32_t *) (e1000->mem_base + p_address);
        return *addr;
    }
    else{
        outportl(e1000->io_base, p_address);
        return inportl(e1000->io_base + 4);
    }
}

bool e1000_detectEEProm(struct INTEL_E1000 *e1000){
    uint32_t val = 0;
    e1000->writeCommand(e1000, REG_EEPROM, 0x1);

    for(int i = 0; i < 1000 && !e1000->eeprom_exists; i++){
        val = e1000->readCommand(e1000, REG_EEPROM);
        if(val & 0x10){
            e1000->eeprom_exists = true;
        }
        else{
            e1000->eeprom_exists = false;
        }
    }
    return e1000->eeprom_exists;
}

uint32_t e1000_eepromRead(struct INTEL_E1000 *e1000, uint8_t addr){
    uint16_t data = 0;
	uint32_t tmp = 0;
    if(e1000->eeprom_exists)
    {
        e1000->writeCommand(e1000, REG_EEPROM, (1) | ((uint32_t)(addr) << 8) );
        while( !((tmp = e1000->readCommand(e1000, REG_EEPROM)) & (1 << 4)) );
    }
    else
    {
        e1000->writeCommand(e1000, REG_EEPROM, (1) | ((uint32_t)(addr) << 2) );
        while( !((tmp = e1000->readCommand(e1000, REG_EEPROM)) & (1 << 1)) );
    }
    data = (uint16_t)((tmp >> 16) & 0xFFFF);
	return data;
}

bool e1000_readMACAddress(struct INTEL_E1000 *e1000){
    if (e1000->eeprom_exists)
    {
        uint32_t temp;
        temp = e1000->eepromRead(e1000, 0);
        e1000->mac[0] = temp &0xff;
        e1000->mac[1] = temp >> 8;
        temp = e1000->eepromRead(e1000, 1);
        e1000->mac[2] = temp &0xff;
        e1000->mac[3] = temp >> 8;
        temp = e1000->eepromRead(e1000, 2);
        e1000->mac[4] = temp &0xff;
        e1000->mac[5] = temp >> 8;
    }
    else
    {
        uint8_t * mem_base_mac_8 = (uint8_t *) (e1000->mem_base+0x5400);
        uint32_t * mem_base_mac_32 = (uint32_t *) (e1000->mem_base+0x5400);
        if ( mem_base_mac_32[0] != 0 )
        {
            for(int i = 0; i < 6; i++)
            {
                e1000->mac[i] = mem_base_mac_8[i];
            }
        }
        else return false;
    }
    return true;
}

void e1000_startLink(struct INTEL_E1000 *e1000 __attribute__((unused))){

}

void e1000_rxinit(struct INTEL_E1000 *e1000 __attribute__((unused))){

}

void e1000_txinit(struct INTEL_E1000 *e1000 __attribute__((unused))){

}

void e1000_enableInterrupt(struct INTEL_E1000 *e1000 __attribute__((unused))){

}

void e1000_handleReceive(struct INTEL_E1000 *e1000 __attribute__((unused))){

}


uint8_t *e1000_getMACAddress(struct INTEL_E1000 *e1000 __attribute__((unused))){
    return NULL;
}

void e1000_interrupt(struct cpu_state cpu  __attribute__((unused)), struct stack_state stack __attribute__((unused))){

}

int e1000_sendPacket(struct INTEL_E1000 *e1000 __attribute__((unused)), void *data __attribute__((unused)), uint16_t len __attribute__((unused))){
    return 0;
}


void e1000_init(struct PCI_driver *driver){
    print_serial("[E1000] Init: \"%s\"\n", driver->name);
    print_serial("[E1000] Wants interrupt #%d\n", driver->interrupt);
    struct INTEL_E1000 *e1000 = malloc(sizeof(struct INTEL_E1000));
    driver->driver_data = e1000;

    if(driver->BAR[0] & 0x1){
        e1000->bar_type = 1;
    }
    else{
        e1000->bar_type = 0;
    }

    e1000->io_base = driver->BAR[0] & ~1;
    e1000->mem_base = driver->BAR[0] & ~3;
    MEM_reserveRegion((uint32_t) e1000->mem_base, (uint32_t) e1000->mem_base, DRIVER);

    e1000->eeprom_exists = false;

    e1000->writeCommand = e1000_writeCommand;
    e1000->readCommand = e1000_readCommand;
    e1000->detectEEProm = e1000_detectEEProm;
    e1000->eepromRead = e1000_eepromRead;
    e1000->readMACAddress = e1000_readMACAddress;
    e1000->startLink = e1000_startLink;
    e1000->rxinit = e1000_rxinit;
    e1000->txinit = e1000_txinit;
    e1000->enableInterrupt = e1000_enableInterrupt;
    e1000->handleReceive = e1000_handleReceive;
    e1000->getMACAddress = e1000_getMACAddress;
    e1000->interrupt = e1000_interrupt;
    e1000->sendPacket = e1000_sendPacket;

    e1000->detectEEProm(e1000);
    if(!e1000->readMACAddress(e1000)){
        print_serial("[E1000] unable to read MAC address\n");
    }
    print_serial("[E1000] MAC: ");
    for(int i = 0; i < 6; i++){
        print_serial("%2x-", e1000->mac[i]);
    }
    print_serial("\n");

}