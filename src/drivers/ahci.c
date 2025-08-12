#include "ahci.h"

#define AHCI_IRQ 3

HBA_MEM *ABAR;
HBA_PORT **Drive_PORTS;
uint32_t driveNUM;

bool AHCI_int_trigger;

int devicePortNums[32] = {0};

uint32_t AHCI_BASE;

void initialize_AHCI(struct PCI_driver *driver){
    print_serial("[AHCI Driver] AHCI DRIVER INIT %x\n", driver);
	if(driver == NULL){
		print_serial("[AHCI Driver] Fatal, NULL PCI Driver");
		return;
	}
	//create_page_entry(AHCI_BASE, AHCI_BASE, 0x83);
	//create_page_entry(AHCI_BASE+0x400000, AHCI_BASE+0x400000, 0x83);
	//create_page_entry((uint32_t) driver->BAR[5], (uint32_t) driver->BAR[5], 0x83);
	int blockIDX = MEM_findRegionIdx(0x400000);
	//MEM_reserveRegion(AHCI_BASE, AHCI_BASE, DRIVER);
	//MEM_reserveRegion(AHCI_BASE+0x400000, AHCI_BASE+0x400000, DRIVER);
	AHCI_BASE = MEM_reserveRegionBlock(blockIDX, 0x400000, 0, DRIVER);//Identity Map AHCI_BASE
	print_serial("[AHCI Driver] BAR: 0x%x New Base: 0x%x\n", driver->BAR[5], AHCI_BASE);
	MEM_reserveRegion((uint32_t) driver->BAR[5], (uint32_t) driver->BAR[5], DRIVER);
	MEM_reserveRegion((uint32_t) driver->BAR[5]+0x400000, (uint32_t) driver->BAR[5]+0x400000, DRIVER);
	//MEM_printRegions();

    //print_serial("[AHCI Driver] AHCI Driver %d BAR: 0x%x INT: %d\n\0", driver->device->progIF, driver->BAR[5], driver->interrupt);
    ABAR = (volatile HBA_MEM *)driver->BAR[5];
    Drive_PORTS = (HBA_PORT **)malloc(32 * sizeof(HBA_PORT));
    AHCI_int_trigger = false;
    print_serial("[AHCI Driver] Ver: %x\n\0", ABAR->vs);
    probe_port(ABAR);
    print_serial("[AHCI Driver] Port CMD base Address: %x %x\n", Drive_PORTS[0]->clb, Drive_PORTS[0]->clbu);
    print_serial("[AHCI Driver] Rebaseing Ports:\n");
    for(unsigned int i = 0; i < driveNUM; i++){
        rebase_PORT(Drive_PORTS[i], devicePortNums[i]);
    }
    print_serial("[AHCI Driver] Port CMD base Address: %x %x\n", Drive_PORTS[0]->clb, Drive_PORTS[0]->clbu);

    print_serial("[AHCI Driver] Enabling AHCI Interrupts\n");

	PCI_write_byte(
		driver->device->bus,
		driver->device->slot,
		driver->device->device,
		0x3C,
		AHCI_IRQ
	);
	print_serial("[AHCI Driver] IRQ: %x\n", PCI_getDeviceInterrupt(
		driver->device->bus,
		driver->device->slot,
		driver->device->device
	));
    //interrupts_init_descriptor(driver->interrupt,(unsigned int) AHCI_Interrupt_Handler);
	interrupt_add_handle(32+AHCI_IRQ, &AHCI_Interrupt_Handler);
	IRQ_clear_mask(AHCI_IRQ);
}

void addSATA_Drive(HBA_PORT *port, int portno){
    Drive_PORTS[driveNUM] = port;
    devicePortNums[driveNUM] = portno;
    driveNUM++;
	drive_add(Drive_AHCI, (void *) port);
}
/*
void addSATAPI_Drive(HBA_PORT *port, int portno){
	addFileSystemDevice(FS_SATAPI_Device, "SATAPI DEVICE_____", (uint32_t *) port, SATAPI_read, 0);
    Drive_PORTS[driveNUM] = port;
    devicePortNums[driveNUM] = portno;
    driveNUM++;
}
*/
void probe_port(HBA_MEM *abar)
{
	// Search disk in implemented ports
	uint32_t pi = abar->pi;
	int i = 0;
	while (i<32)
	{
		if (pi & 1)
		{
			int dt = check_type(&abar->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
				print_serial("[AHCI Driver] SATA drive found at port %x\n", i);
                addSATA_Drive(&abar->ports[i], i);
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				print_serial("[AHCI Driver] SATAPI drive found at port %x\n", i);
                //addSATAPI_Drive(&abar->ports[i], i);
			}
			else if (dt == AHCI_DEV_SEMB)
			{
				print_serial("[AHCI Driver] SEMB drive found at port %x\n", i);
                //addAHCI_Drive(&abar->ports[i], i);
			}
			else if (dt == AHCI_DEV_PM)
			{
				print_serial("[AHCI Driver] PM drive found at port %x\n", i);
                //addAHCI_Drive(&abar->ports[i], i);
			}
			else
			{
				//print_serial("[AHCI Driver] No drive found at port %x\n", i);
			}
		}
 
		pi >>= 1;
		i ++;
	}
}
 
// Check device type
int check_type(HBA_PORT *port)
{
	uint32_t ssts = port->ssts;
 
	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;
 
	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;
 
	switch (port->sig)
	{
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}


// Start command engine
void start_cmd(HBA_PORT *port)
{
	// Wait until CR (bit15) is cleared
	while (port->cmd & HBA_PxCMD_CR)
		;
 
	// Set FRE (bit4) and ST (bit0)
	port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= HBA_PxCMD_ST; 
}
 
// Stop command engine
void stop_cmd(HBA_PORT *port)
{
	// Clear ST (bit0)
	port->cmd &= ~HBA_PxCMD_ST;
 
	// Clear FRE (bit4)
	port->cmd &= ~HBA_PxCMD_FRE;
 
	// Wait until FR (bit14), CR (bit15) are cleared
	while(1)
	{
		if (port->cmd & HBA_PxCMD_FR)
			continue;
		if (port->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}
 
}


void rebase_PORT(HBA_PORT *port, int portno){
    stop_cmd(port); // Stop commands
    print_serial("[AHCI Driver] Rebasing Port: %x\n", portno);
    // Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	port->clb = AHCI_BASE + (portno<<10);
	port->clbu = 0;
	memset((void*)(port->clb), 0, 1024);
	print_serial("[AHCI Driver] Command List Addr: %x\n", port->clb);
 
	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	port->fb = AHCI_BASE + (32<<10) + (portno<<8);
	port->fbu = 0;
	memset((void*)(port->fb), 0, 256);
 
	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
		cmdheader[i].ctbau = 0;
		memset((void*)cmdheader[i].ctba, 0, 256);
	}
    
	start_cmd(port);	// Start command engine
}

void AHCI_Interrupt_Handler(struct cpu_state *cpu __attribute__((unused)), struct stack_state *stack __attribute__((unused))){
    AHCI_int_trigger = true;
	print_serial("[AHCI Driver] AHCI Interrupt Called\n");
	return;
}
 
bool AHCI_read(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf)
{
	//print_serial("[AHCI Driver] Reading Sector %x%x (%d) for %d sectors\n", starth, startl, startl, count);
	//uint32_t phy_buf = ((uint32_t) buf) - 0xC0000000;
	uint32_t phy_buf = get_physical((uint32_t) buf);
	port->is = (uint32_t) -1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	if (slot == -1){
		print_serial("[AHCI Driver] Fail to find cmd slot\n");
		return false;
	}
 
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);	// Command FIS size
	cmdheader->w = 0;		// Read from device
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count
 
	HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) +
 		(cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
 
	// 8K bytes (16 sectors) per PRDT
	for (int i=0; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint32_t) phy_buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024-1;	// 8K bytes (this value should always be set to 1 less than the actual value)
		cmdtbl->prdt_entry[i].i = 1;
		phy_buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[cmdheader->prdtl-1].dba = (uint32_t) phy_buf;
	cmdtbl->prdt_entry[cmdheader->prdtl-1].dbc = (count<<9)-1;	// 512 bytes per sector
	cmdtbl->prdt_entry[cmdheader->prdtl-1].i = 1;
 
	// Setup command
	FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EXT;
 
	cmdfis->lba0 = (uint8_t)startl;
	cmdfis->lba1 = (uint8_t)(startl>>8);
	cmdfis->lba2 = (uint8_t)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t)(startl>>24);
	cmdfis->lba4 = (uint8_t)starth;
	cmdfis->lba5 = (uint8_t)(starth>>8);
 
	cmdfis->countl = count & 0xFF;
	cmdfis->counth = (count >> 8) & 0xFF;
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		print_serial("[AHCI Driver] Port is hung\n");
		return false;
	}
 
	port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is & HBA_PxIS_TFES)	// Task file error
		{
			print_serial("[AHCI Driver] Read disk error\n");
			return false;
		}
	}
 
	// Check again
	if (port->is & HBA_PxIS_TFES)
	{
		print_serial("[AHCI Driver] Read disk error\n");
		return false;
	}
 
	return true;
}

bool AHCI_write(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count, uint32_t *buf)
{
	uint32_t phy_buf = get_physical((uint32_t) buf);
	port->is = (uint32_t) -1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	if (slot == -1)
		return false;
 
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);	// Command FIS size
	cmdheader->w = 1;		// Write to device
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count
 
	HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) +
 		(cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
 
	// 8K bytes (16 sectors) per PRDT
	for (int i=0; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint32_t) phy_buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024-1;	// 8K bytes (this value should always be set to 1 less than the actual value)
		cmdtbl->prdt_entry[i].i = 1;
		phy_buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[cmdheader->prdtl-1].dba = (uint32_t) phy_buf;
	cmdtbl->prdt_entry[cmdheader->prdtl-1].dbc = (count<<9)-1;	// 512 bytes per sector
	cmdtbl->prdt_entry[cmdheader->prdtl-1].i = 1;
 
	// Setup command
	FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_WRITE_DMA_EXT;
 
	cmdfis->lba0 = (uint8_t)startl;
	cmdfis->lba1 = (uint8_t)(startl>>8);
	cmdfis->lba2 = (uint8_t)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t)(startl>>24);
	cmdfis->lba4 = (uint8_t)starth;
	cmdfis->lba5 = (uint8_t)(starth>>8);
 
	cmdfis->countl = count & 0xFF;
	cmdfis->counth = (count >> 8) & 0xFF;
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		print_serial("[AHCI Driver] Port is hung\n");
		return false;
	}
 
	port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is & HBA_PxIS_TFES)	// Task file error
		{
			print_serial("[AHCI Driver] Write disk error\n");
			return false;
		}
	}
 
	// Check again
	if (port->is & HBA_PxIS_TFES)
	{
		print_serial("[AHCI Driver] Write disk error\n");
		return false;
	}
 
	return true;
}

// Find a free command list slot
int find_cmdslot(HBA_PORT *port)
{
	// If not set in SACT and CI, the slot is free
	uint32_t slots = (port->sact | port->ci);
	for (int i=0; i<32; i++)
	{
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
	print_serial("[AHCI Driver] Cannot find free command list entry\n");
	return -1;
}
/*
bool SATAPI_read(uint16_t port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf){
	
}
*/