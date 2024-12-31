#include "rtl8139.h"
#include "ethernet.h"

enum RTL8139_registers {
  MAC0             = 0x00,       // Ethernet hardware address
  MAC1             = 0x04,
  MAR0             = 0x08,       // Multicast filter
  TxStatus0        = 0x10,       // Transmit status (Four 32bit registers)
  TxAddr0          = 0x20,       // Tx descriptors (also four 32bit)
  RxBuf            = 0x30, 
  RxEarlyCnt       = 0x34, 
  RxEarlyStatus    = 0x36,
  ChipCmd          = 0x37,
  RxBufPtr         = 0x38,
  RxBufAddr        = 0x3A,
  IntrMask         = 0x3C,
  IntrStatus       = 0x3E,
  TxConfig         = 0x40,
  RxConfig         = 0x44,
  Timer            = 0x48,        // A general-purpose counter
  RxMissed         = 0x4C,        // 24 bits valid, write clears
  Cfg9346          = 0x50, 
  Config0          = 0x51, 
  Config1          = 0x52,
  FlashReg         = 0x54, 
  GPPinData        = 0x58, 
  GPPinDir         = 0x59, 
  MII_SMI          = 0x5A, 
  HltClk           = 0x5B,
  MultiIntr        = 0x5C, 
  TxSummary        = 0x60,
  MII_BMCR         = 0x62, 
  MII_BMSR         = 0x64, 
  NWayAdvert       = 0x66, 
  NWayLPAR         = 0x68,
  NWayExpansion    = 0x6A,
  
  // Undocumented registers, but required for proper operation
  FIFOTMS          = 0x70,        // FIFO Control and test
  CSCR             = 0x74,        // Chip Status and Configuration Register
  PARA78           = 0x78, 
  PARA7c           = 0x7c,        // Magic transceiver parameter register
};

enum ChipCmdBits {
  RxBufEmpty = 0x01,
  CmdTxEnb   = 0x04,
  CmdRxEnb   = 0x08,
  CmdReset   = 0x10,
};

#define RX_READ_POINTER_MASK (~3)
#define ROK     (1<<0)
#define RER     (1<<1)
#define TOK     (1<<2)
#define TER     (1<<3)
#define TX_TOK  (1<<15)

#define RX_BUF_SIZE 8192

uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};

uint32_t rtl8139_sendPacket(struct ethernet_driver *ether __attribute__((unused)), void *packet __attribute__((unused)), uint32_t size __attribute__((unused))){
    while(ether->tx_lock){}
    print_serial("[RTL8139] Sending Packet\n");
    void *transfer_data = ether->tx_buffer;
    uint32_t phys_addr = get_physical((uint32_t) transfer_data);
    memcpy(transfer_data, packet, size);

    outportl(ether->io_base + TSAD_array[ether->tx_buffer_end], (uint32_t) phys_addr);
    outportl(ether->io_base + TSD_array[ether->tx_buffer_end++], size);
    if(ether->tx_buffer_end > 3){
        ether->tx_buffer_end = 0;
    }
    
    return 0;
}

void rtl8139_receive_packet(struct ethernet_driver *ether) {
    uint16_t *t = (uint16_t *) (ether->rx_buffer + ether->rx_buffer_end);
    uint16_t packet_length = *(t + 1);

    t = t + 2;

    void *packet = malloc(packet_length);
    memcpy(packet, t, packet_length);
    ethernet_handle_packet((struct ethernet_header *) packet, packet_length);

    ether->rx_buffer_end = (ether->rx_buffer_end + packet_length + 4 + 3) & RX_READ_POINTER_MASK;

    if(ether->rx_buffer_end > RX_BUF_SIZE){
        ether->rx_buffer_end -= RX_BUF_SIZE;
    }

    outports(ether->io_base + RxBufPtr, ether->rx_buffer_end - 0x10);
}

void rtl8139_enableInterrupt(struct ethernet_driver *ether __attribute__((unused))){
    print_serial("[RTL8139] Enable Interrupts\n");
	IRQ_clear_mask(ether->int_number);
    outb(ether->io_base + ChipCmd, CmdTxEnb | CmdRxEnb);
    
}

void rtl8139_disableInterrupt(struct ethernet_driver *ether __attribute__((unused))){
    print_serial("[RTL8139] Disable Interrupts\n");
    IRQ_set_mask(ether->int_number);
    outb(ether->io_base + ChipCmd, inb(ether->io_base + ChipCmd) & ~(CmdTxEnb | CmdRxEnb));
}

void rtl8139_int_handler(struct ethernet_driver *ether __attribute__((unused))){
    print_serial("[RTL8139] Interrupt!\n");

    uint16_t status = inports(ether->io_base + IntrStatus);

    if(status & TOK){
        print_serial("[RTL8139] Packet Sent\n");
        ether->tx_lock = false;
    }
    if(status & ROK){
        print_serial("[RTL8139] Recieved Packet\n");
        rtl8139_receive_packet(ether);

    }

    outports(ether->io_base + IntrStatus, 0x5);
    pic_acknowledge(32 + ether->int_number);
}

void rtl8139_read_mac(struct ethernet_driver *ether){
    uint32_t mac_p1 = inportl(ether->io_base + MAC0);
    uint16_t mac_p2 = (uint16_t) (inportl(ether->io_base + MAC1) & 0xFFFF);
    ether->mac[0] = mac_p1 >> 0;
    ether->mac[1] = mac_p1 >> 8;
    ether->mac[2] = mac_p1 >> 16;
    ether->mac[3] = mac_p1 >> 24;
    ether->mac[4] = mac_p2 >> 0;
    ether->mac[5] = mac_p2 >> 8;
    print_serial("[RTL8139] MAC Read\n");
}

void rtl8139_reset_device(struct ethernet_driver *ether){
    print_serial("[RTL8139] Resetting Device!\n");
    outb(ether->io_base + ChipCmd, CmdReset);
    while(inb(ether->io_base + ChipCmd) & CmdReset){}
}

struct ethernet_driver *rtl8139_init(struct PCI_driver *driver){
	print_serial("[RTL8139] Init: \"%s\"\n", driver->name);
    print_serial("[RTL8139] Wants interrupt #%d\n", driver->interrupt);
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
    print_serial("[RTL8139] BAR Type: %d\n", ether->bar_type);

    ether->io_base = driver->BAR[0] & (~0x3);
    ether->mem_base = driver->BAR[0] & (~0xf);
    MEM_reserveRegion((uint32_t) ether->mem_base, (uint32_t) ether->mem_base, DRIVER);

    print_serial("[RTL8139] Use %s based access (base: 0x%x)\n", (ether->bar_type) ? "mem" : "port", (ether->bar_type != 0) ? ether->io_base : ether->mem_base);

	ether->private_page_base = (void *) MEM_reserveRegionBlock(MEM_findRegionIdx(PAGE_SIZE), PAGE_SIZE, 0, DRIVER);
    MEM_printRegions();
    ether->private_page_offset = 0;
    ether->num_private_pages = 1;

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

    ether->write = rtl8139_sendPacket;
    ether->int_enable = rtl8139_enableInterrupt;
    ether->int_disable = rtl8139_disableInterrupt;
    ether->int_handler = rtl8139_int_handler;
    

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

    outb(ether->io_base + Config1, 0x0);

    rtl8139_reset_device(ether);

    ether->rx_buffer = ether->private_page_base + ether->private_page_offset;
    ether->private_page_offset += RX_BUF_SIZE + 16 + 1500;
    memset(ether->rx_buffer, 0x0, RX_BUF_SIZE + 16 + 1500);
    ether->rx_buffer_end = 0;

    ether->tx_buffer = ether->private_page_base + ether->private_page_offset;
    ether->tx_buffer_end = 0;

    outportl(ether->io_base + RxBuf, (uint32_t) get_physical((uint32_t) ether->rx_buffer));

    outports(ether->io_base + IntrMask, 0x0005);

    outportl(ether->io_base + RxConfig, 0xf | (1 << 7));

    rtl8139_read_mac(ether);

    return ether;
}