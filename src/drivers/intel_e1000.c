#include "intel_e1000.h"

#include "ethernet.h"

/*
#define E1000_DEV 0x100E

#define REG_CTRL        0x0000
#define REG_STATUS      0x0008
#define REG_EEPROM      0x0014
#define REG_CTRL_EXT    0x0018
#define REG_IMASK       0x00D0
#define REG_RCTRL       0x0100
#define REG_RXDESCLO    0x2800
#define REG_RXDESCHI    0x2804
#define REG_RXDESCLEN   0x2808
#define REG_RXDESCHEAD  0x2810
#define REG_RXDESCTAIL  0x2818

#define REG_TCTRL       0x0400
#define REG_TXDESCLO    0x3800
#define REG_TXDESCHI    0x3804
#define REG_TXDESCLEN   0x3808
#define REG_TXDESCHEAD  0x3810
#define REG_TXDESCTAIL  0x3818


#define REG_RDTR         0x2820 // RX Delay Timer Register
#define REG_RXDCTL       0x2828 // RX Descriptor Control
#define REG_RADV         0x282C // RX Int. Absolute Delay Timer
#define REG_RSRPD        0x2C00 // RX Small Packet Detect Interrupt



#define REG_TIPG         0x0410      // Transmit Inter Packet Gap
#define ECTRL_SLU        0x40        //set link up


#define RCTL_EN                         (1 << 1)    // Receiver Enable
#define RCTL_SBP                        (1 << 2)    // Store Bad Packets
#define RCTL_UPE                        (1 << 3)    // Unicast Promiscuous Enabled
#define RCTL_MPE                        (1 << 4)    // Multicast Promiscuous Enabled
#define RCTL_LPE                        (1 << 5)    // Long Packet Reception Enable
#define RCTL_LBM_NONE                   (0 << 6)    // No Loopback
#define RCTL_LBM_PHY                    (3 << 6)    // PHY or external SerDesc loopback
#define RTCL_RDMTS_HALF                 (0 << 8)    // Free Buffer Threshold is 1/2 of RDLEN
#define RTCL_RDMTS_QUARTER              (1 << 8)    // Free Buffer Threshold is 1/4 of RDLEN
#define RTCL_RDMTS_EIGHTH               (2 << 8)    // Free Buffer Threshold is 1/8 of RDLEN
#define RCTL_MO_36                      (0 << 12)   // Multicast Offset - bits 47:36
#define RCTL_MO_35                      (1 << 12)   // Multicast Offset - bits 46:35
#define RCTL_MO_34                      (2 << 12)   // Multicast Offset - bits 45:34
#define RCTL_MO_32                      (3 << 12)   // Multicast Offset - bits 43:32
#define RCTL_BAM                        (1 << 15)   // Broadcast Accept Mode
#define RCTL_VFE                        (1 << 18)   // VLAN Filter Enable
#define RCTL_CFIEN                      (1 << 19)   // Canonical Form Indicator Enable
#define RCTL_CFI                        (1 << 20)   // Canonical Form Indicator Bit Value
#define RCTL_DPF                        (1 << 22)   // Discard Pause Frames
#define RCTL_PMCF                       (1 << 23)   // Pass MAC Control Frames
#define RCTL_SECRC                      (1 << 26)   // Strip Ethernet CRC

// Buffer Sizes
#define RCTL_BSIZE_256                  (3 << 16)
#define RCTL_BSIZE_512                  (2 << 16)
#define RCTL_BSIZE_1024                 (1 << 16)
#define RCTL_BSIZE_2048                 (0 << 16)
#define RCTL_BSIZE_4096                 ((3 << 16) | (1 << 25))
#define RCTL_BSIZE_8192                 ((2 << 16) | (1 << 25))
#define RCTL_BSIZE_16384                ((1 << 16) | (1 << 25))


// Transmit Command

#define CMD_EOP                         (1 << 0)    // End of Packet
#define CMD_IFCS                        (1 << 1)    // Insert FCS
#define CMD_IC                          (1 << 2)    // Insert Checksum
#define CMD_RS                          (1 << 3)    // Report Status
#define CMD_RPS                         (1 << 4)    // Report Packet Sent
#define CMD_VLE                         (1 << 6)    // VLAN Packet Enable
#define CMD_IDE                         (1 << 7)    // Interrupt Delay Enable


// TCTL Register

#define TCTL_EN                         (1 << 1)    // Transmit Enable
#define TCTL_PSP                        (1 << 3)    // Pad Short Packets
#define TCTL_CT_SHIFT                   4           // Collision Threshold
#define TCTL_COLD_SHIFT                 12          // Collision Distance
#define TCTL_SWXOFF                     (1 << 22)   // Software XOFF Transmission
#define TCTL_RTLC                       (1 << 24)   // Re-transmit on Late Collision

#define TSTA_DD                         (1 << 0)    // Descriptor Done
#define TSTA_EC                         (1 << 1)    // Excess Collisions
#define TSTA_LC                         (1 << 2)    // Late Collision
#define LSTA_TU                         (1 << 3)    // Transmit Underrun

*/
#define E1000_NUM_RX_DESC 32
#define E1000_NUM_TX_DESC 8

void e1000_writeCommand(struct ethernet_driver *ether, uint16_t p_address, uint32_t p_value){
    if(ether->bar_type == 0){
        uint32_t *addr = (uint32_t *) (ether->mem_base + p_address);
        *addr = p_value;
    }
    else{
        outportl(ether->io_base, p_address);
        outportl(ether->io_base + 4, p_value);
    }
}

uint32_t e1000_readCommand(struct ethernet_driver *ether, uint16_t p_address){
    if(ether->bar_type == 0){
        uint32_t *addr = (uint32_t *) (ether->mem_base + p_address);
        return *addr;
    }
    else{
        outportl(ether->io_base, p_address);
        return inportl(ether->io_base + 4);
    }
}

bool e1000_detectEEProm(struct ethernet_driver *ether){
    uint32_t val = 0;
    e1000_writeCommand(ether, E1000_REG_EERD, 0x1);

    for(int i = 0; i < 1000 && !ether->eeprom_exists; i++){
        val = e1000_readCommand(ether, E1000_REG_EERD);
        if(val & 0x10){
            ether->eeprom_exists = true;
        }
        else{
            ether->eeprom_exists = false;
        }
    }
    return ether->eeprom_exists;
}

uint32_t e1000_eepromRead(struct ethernet_driver *ether, uint8_t addr){
    uint16_t data = 0;
	uint32_t tmp = 0;
    if(ether->eeprom_exists)
    {
        e1000_writeCommand(ether, E1000_REG_EERD, (1) | ((uint32_t)(addr) << 8) );
        while( !((tmp = e1000_readCommand(ether, E1000_REG_EERD)) & (1 << 4)) );
    }
    else
    {
        e1000_writeCommand(ether, E1000_REG_EERD, (1) | ((uint32_t)(addr) << 2) );
        while( !((tmp = e1000_readCommand(ether, E1000_REG_EERD)) & (1 << 1)) );
    }
    data = (uint16_t)((tmp >> 16) & 0xFFFF);
	return data;
}

bool e1000_readMACAddress(struct ethernet_driver *ether){
    if (ether->eeprom_exists)
    {
        uint32_t temp;
        temp = e1000_eepromRead(ether, 0);
        ether->mac[0] = temp &0xff;
        ether->mac[1] = temp >> 8;
        temp = e1000_eepromRead(ether, 1);
        ether->mac[2] = temp &0xff;
        ether->mac[3] = temp >> 8;
        temp = e1000_eepromRead(ether, 2);
        ether->mac[4] = temp &0xff;
        ether->mac[5] = temp >> 8;
    }
    else
    {
        uint8_t * mem_base_mac_8 = (uint8_t *) (ether->mem_base+0x5400);
        uint32_t * mem_base_mac_32 = (uint32_t *) (ether->mem_base+0x5400);
        if ( mem_base_mac_32[0] != 0 )
        {
            for(int i = 0; i < 6; i++)
            {
                ether->mac[i] = mem_base_mac_8[i];
            }
        }
        else return false;
    }
    return true;
}

void e1000_startLink(struct ethernet_driver *ether __attribute__((unused))){

}

void e1000_rxinit(struct ethernet_driver *ether __attribute__((unused))){

}

void e1000_txinit(struct ethernet_driver *ether __attribute__((unused))){

}

void e1000_enableInterrupt(struct ethernet_driver *ether __attribute__((unused))){

}

void e1000_handleReceive(struct ethernet_driver *ether __attribute__((unused))){

}


uint8_t *e1000_getMACAddress(struct ethernet_driver *ether){
    return ether->mac;
}

void e1000_interrupt(struct cpu_state cpu  __attribute__((unused)), struct stack_state stack __attribute__((unused))){

}

uint32_t e1000_sendPacket(struct ethernet_driver *ether __attribute__((unused)), struct ethernet_packet *packet __attribute__((unused)), uint32_t size __attribute__((unused))){
    return 0;
}


struct ethernet_driver *e1000_init(struct PCI_driver *driver){
    print_serial("[E1000] Init: \"%s\"\n", driver->name);
    print_serial("[E1000] Wants interrupt #%d\n", driver->interrupt);
    struct ethernet_driver *ether = malloc(sizeof(struct ethernet_driver));
    ether->int_number = driver->interrupt;

    driver->driver_data = ether;
    ether->pci = driver;

    if(driver->BAR[0] & 0x1){
        ether->bar_type = 1;
    }
    else{
        ether->bar_type = 0;
    }

    ether->io_base = driver->BAR[0] & ~1;
    ether->mem_base = driver->BAR[0] & ~3;
    MEM_reserveRegion((uint32_t) ether->mem_base, (uint32_t) ether->mem_base, DRIVER);

    ether->ipv4.ip[0] = 0;
    ether->ipv4.ip[1] = 0;
    ether->ipv4.ip[2] = 0;
    ether->ipv4.ip[3] = 0;
    ether->ipv4.netmask[0] = 0;
    ether->ipv4.netmask[1] = 0;
    ether->ipv4.netmask[2] = 0;
    ether->ipv4.netmask[3] = 0;
    ether->ipv4.gateway[0] = 0;
    ether->ipv4.gateway[1] = 0;
    ether->ipv4.gateway[2] = 0;
    ether->ipv4.gateway[3] = 0;

    ether->write = e1000_sendPacket;
    

    uint16_t u16_pci_cmd_reg = PCI_read_word(
        ether->pci->device->bus,
        ether->pci->device->slot,
        ether->pci->device->device,
        0x04
    );
    struct PCI_command_reg pci_cmd_reg = *((struct PCI_command_reg *) &u16_pci_cmd_reg);
    pci_cmd_reg.bus_master = 1;
    pci_cmd_reg.memory_space = 1;
    
    PCI_write_word(
        ether->pci->device->bus,
        ether->pci->device->slot,
        ether->pci->device->device,
        0x04,
        *(uint16_t *)&pci_cmd_reg
    );

    e1000_writeCommand(ether, E1000_REG_EECD, E1000_REGBIT_EECD_SK | E1000_REGBIT_EECD_CS | E1000_REGBIT_EECD_DI);

    e1000_detectEEProm(ether);
    if(!e1000_readMACAddress(ether)){
        print_serial("[E1000] unable to read MAC address\n");
    }

    print_serial("[E1000] MAC: %2x:%2x:%2x:%2x:%2x:%2x\n", ether->mac[0], ether->mac[1], ether->mac[2], ether->mac[3], ether->mac[4], ether->mac[5]);

    e1000_writeCommand(ether, E1000_REG_FCAL, 0);
    e1000_writeCommand(ether, E1000_REG_FCAH, 0);
    e1000_writeCommand(ether, E1000_REG_FCT, 0);
    e1000_writeCommand(ether, E1000_REG_FCTTV, 0);

    // Auto-Speed Detection
    e1000_writeCommand(ether, E1000_REG_CTRL, E1000_REGBIT_CTRL_ASDE | E1000_REGBIT_CTRL_SLU);

    e1000_rxinit(ether);
    e1000_txinit(ether);

    e1000_writeCommand(ether, E1000_REG_RADV, 0);
    e1000_writeCommand(ether, E1000_REG_RDTR, E1000_REGBIT_RDT_RDTR_FPD | 0);
    e1000_writeCommand(ether, E1000_REG_ITR, 5000);

    return ether;
}