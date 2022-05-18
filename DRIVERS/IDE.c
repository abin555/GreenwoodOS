#include "IDE.h"

char ide_driverName[] = "IDE CONTROLLER DRIVER";

void ide_driver_install(int driverID, int reversedID){
   fb_write_xy(ide_driverName, sizeof(ide_driverName), 0, 50, driverID+1);
   
   uint32_t dataBar = pci_drivers[reversedID]->BAR[0]; 
   
   decodeHex(STR_edit, dataBar, 32, 0);
   fb_write_xy(STR_edit, 8, 1, 50+sizeof(usb_driverName)+9, driverID+1);

   decodeHex(STR_edit, driverID, 8, 0);
   fb_write_xy(STR_edit, 2, 1, 50+sizeof(ide_driverName)+1, driverID+1);
}