#include "IDE.h"

char ide_driverName[] = "IDE CONTROLLER DRIVER";

unsigned char ide_buf[2048] = {0};
static unsigned char ide_irq_invoked = 0;
static unsigned char atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void ide_driver_install(int driverID){
   fb_write_xy(ide_driverName, sizeof(ide_driverName), 0, 50, driverID+1);
   
   uint32_t dataBar = pci_drivers[driverID]->BAR[4]; 
   
   decodeHex(STR_edit, dataBar, 32, 0);
   fb_write_xy(STR_edit, 8, 1, 50+sizeof(usb_driverName)+9, driverID+1);

   decodeHex(STR_edit, pci_drivers[driverID]->init_one->progIF, 8, 0);
   fb_write_xy(STR_edit, 2, 1, 50+sizeof(ide_driverName)+1, driverID+1);
   #define bus pci_drivers[driverID]->init_one->bus
   #define device pci_drivers[driverID]->init_one->slot
   #define func pci_drivers[driverID]->init_one->func
   #define class pci_drivers[driverID]->init_one->Class
   #define ProgIF pci_drivers[driverID]->init_one->progIF

   outportl((1<<31) | (bus<<16) | (device<<11) | (func<<8) | 8, 0xCF8); // Send the parameters.
   if ((class = inportl(0xCFC)>>16) != 0xFFFF) { // If there is exactly a device
      // Check if this device need an IRQ assignment:
      outportl(0xCF8, (1<<31) | (bus<<16) | (device<<11) | (func<<8) | 0x3C);
      outb(0xCFC, 0xFE); // Change the IRQ field to 0xFE
      outportl(0xCF8, (1<<31) | (bus<<16) | (device<<11) | (func<<8) | 0x3C); // Read the IRQ Field Again.
      if ((inportl(0xCFC) & 0xFF)==0xFE) {
         // This Device needs IRQ assignment.
         fb_write_xy("IRQ Needed", 10, 0, 50+sizeof(ide_driverName)+24, driverID+1);
      } else {
         // The Device doesn't use IRQs, check if this is an Parallel IDE:
         if (class == 0x0101 && (ProgIF == 0x8A || ProgIF == 0x80)) {
            // This is a Parallel IDE Controller which use IRQ 14 and IRQ 15.
            fb_write_xy("P IDE #14 & #15", 15, 0, 50+sizeof(ide_driverName)+24, driverID+1);
         }
      }
   }
   ide_irq_invoked = 1;
   atapi_packet[1] = 0;
   printChar(ide_irq_invoked, 0, ' ');
   printChar(atapi_packet[0], 0, 'A');
}

unsigned char ide_read(unsigned char channel, unsigned char reg){
   unsigned char result;
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   if   (reg < 0x08) result = inb(channels[channel].base  + reg - 0x00);
   else if   (reg < 0x0C) result = inb(channels[channel].base  + reg - 0x06);
   else if   (reg < 0x0E) result = inb(channels[channel].ctrl  + reg - 0x0A);
   else if   (reg < 0x16) result = inb(channels[channel].bmide + reg - 0x0E);
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
   return result;
}

void ide_write(unsigned char channel, unsigned char reg, unsigned char data){
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   if   (reg < 0x08) outb(data, channels[channel].base  + reg - 0x00);
   else if   (reg < 0x0C) outb(data, channels[channel].base  + reg - 0x06);
   else if   (reg < 0x0E) outb(data, channels[channel].ctrl  + reg - 0x0A);
   else if   (reg < 0x16) outb(data, channels[channel].bmide + reg - 0x0E);
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

void ide_read_buffer(unsigned char channel, unsigned char reg, unsigned int buffer, unsigned int quads){
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   asm("pushw %es; pushw %ax; movw %ds, %ax; movw %ax, %es; popw %ax;");
   if   (reg < 0x08) insl(channels[channel].base  + reg - 0x00, (unsigned int *) buffer, quads);
   else if   (reg < 0x0C) insl(channels[channel].base  + reg - 0x06, (unsigned int *) buffer, quads);
   else if   (reg < 0x0E) insl(channels[channel].ctrl  + reg - 0x0A, (unsigned int *) buffer, quads);
   else if   (reg < 0x16) insl(channels[channel].bmide + reg - 0x0E, (unsigned int *) buffer, quads);
   asm("popw %es;");
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

unsigned char ide_polling(unsigned char channel, unsigned int advanced_check){
   // (I) Delay 400 nanosecond for BSY to be set:
   // -------------------------------------------------
   ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
   ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
   ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
   ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.

   // (II) Wait for BSY to be cleared:
   // -------------------------------------------------
   while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY); // Wait for BSY to be zero.

   if (advanced_check) {

      unsigned char state = ide_read(channel, ATA_REG_STATUS); // Read Status Register.

      // (III) Check For Errors:
      // -------------------------------------------------
      if (state & ATA_SR_ERR) return 2; // Error.

      // (IV) Check If Device fault:
      // -------------------------------------------------
      if (state & ATA_SR_DF ) return 1; // Device Fault.

      // (V) Check DRQ:
      // -------------------------------------------------
      // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
      if (!(state & ATA_SR_DRQ)) return 3; // DRQ should be set

   }

   return 0; // No Error.
}

unsigned char ide_print_error(unsigned int drive, unsigned char err){
   if (err == 0) return err;

   printk(" IDE:");
   if (err == 1) {
      printk("- Device Fault\n     "); 
      err = 19;
   }
   else if (err == 2) {
      unsigned char st = ide_read(ide_devices[drive].channel, ATA_REG_ERROR);
      if (st & ATA_ER_AMNF)   {printk("- No Address Mark Found\n     ");   err = 7;}
      if (st & ATA_ER_TK0NF)   {printk("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_ABRT)   {printk("- Command Aborted\n     ");      err = 20;}
      if (st & ATA_ER_MCR)   {printk("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_IDNF)   {printk("- ID mark not Found\n     ");      err = 21;}
      if (st & ATA_ER_MC)   {printk("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_UNC)   {printk("- Uncorrectable Data Error\n     ");   err = 22;}
      if (st & ATA_ER_BBK)   {printk("- Bad Sectors\n     ");       err = 13;}
   } else  if (err == 3)           {printk("- Reads Nothing\n     "); err = 23;}
     else  if (err == 4)  {printk("- Write Protected\n     "); err = 8;}
   /*
   printk("- [%s %s] %s\n",
      (const char *[]){"Primary","Secondary"}[ide_devices[drive].channel],
      (const char *[]){"Master", "Slave"}[ide_devices[drive].drive],
      ide_devices[drive].model);
*/
   return err;
}

/*
void ide_initialize(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3, unsigned int BAR4){

}
*/