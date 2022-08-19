#include "hci.h"
#include "uhci.h"

void init_uhci(int pciDriverID){
    printk("UHCI Driver Init @ %2h\n", pciDriverID);
}