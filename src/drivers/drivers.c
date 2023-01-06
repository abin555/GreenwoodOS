#include "drivers.h"

void drivers_init_pci(){
    for(int driver = 0; driver < (int) pci_driver_num; driver++){
        printk("----------------------\n");
        printk("Driver #%x\n", driver);
        pci_drivers[driver]->init_driver(driver);
        printk("----------------------\n");
    }
}