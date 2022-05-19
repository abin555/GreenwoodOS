#include "IDE.h"

char ide_driverName[] = "IDE CONTROLLER DRIVER";

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
}