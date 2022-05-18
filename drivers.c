#include "drivers.h"

void activate_Drivers(){
    for(int driver = 0; driver < (int) drivs; driver++){
        pci_drivers[driver]->init_driver(driver, (drivs-1)-driver);
        //usb_init_driver(2);
    }
}