#include "IDE.h"

char ide_driverName[] = "IDE CONTROLLER DRIVER";

void ide_driver_install(int driverID){
   fb_write_xy(ide_driverName, sizeof(ide_driverName), 0, 30, fb_terminal_h-driverID-1);
   decodeHex(STR_edit, 
      getDeviceProgIF(
         pci_drivers[driverID]->init_one->device_id->bus,
         pci_drivers[driverID]->init_one->device_id->slot,
         pci_drivers[driverID]->init_one->device_id->func
      ),
   16, 
   0);
   fb_write_xy(STR_edit, 4, 1, 30+sizeof(ide_driverName)+1,fb_terminal_h-driverID-1);
   uint32_t dataBar = pci_drivers[driverID]->header.BAR[0];
   decodeHex(STR_edit, dataBar, 32, 0);
   fb_write_xy(STR_edit, 8, 1, fb_terminal_w-9, fb_terminal_h-driverID-1);
}