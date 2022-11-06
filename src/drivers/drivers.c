#include "drivers.h"

void drivers_init_pci(){
    for(int driver = 0; driver < (int) pci_driver_num; driver++){
        pci_drivers[driver]->init_driver(driver);
    }
}