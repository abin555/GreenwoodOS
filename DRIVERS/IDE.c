#include "IDE.h"

char ide_driverName[] = "IDE CONTROLLER DRIVER";

void ide_driver_install(int driverID){
   fb_write_xy(ide_driverName, sizeof(ide_driverName), 0, 40, driverID);
   decodeHex(STR_edit, 
   
   getDeviceClass(
      pci_drivers[driverID]->init_one->device_id->bus,
      pci_drivers[driverID]->init_one->device_id->slot,
      pci_drivers[driverID]->init_one->device_id->func
   ),
   
   //pci_drivers[driverID]->init_one->vendor, 
   16, 
   0);
   fb_write_xy(STR_edit, 4, 1, 40+sizeof(ide_driverName)+1,driverID);
}