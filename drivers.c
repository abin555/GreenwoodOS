#include "drivers.h"

void activate_Drivers(){
    for(uint16_t driver = 0; driver < drivs; driver++){
        pci_drivers[driver]->init_driver();
    }
}