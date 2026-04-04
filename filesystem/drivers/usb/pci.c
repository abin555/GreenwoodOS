#include "pci.h"

FILE *pci_open_devices(){
    return fopen("/-/dev/pci/devices", "r");
}

struct PCI_device *pci_getDev(FILE *pci, uint16_t class, uint16_t progIF){
    fseek(pci, 0, SEEK_END);
    size_t fsize = ftell(pci);
    fseek(pci, 0, SEEK_SET);
    int count_devs = fsize / sizeof(struct PCI_device *);
    printf("file is %d bytes - %d devices\n", fsize, count_devs);
    struct PCI_device *dev;
    struct PCI_device *target = NULL;
    for(int i = 0; i < count_devs; i++){
        fread(&dev, sizeof(dev), 1, pci);
        if(dev == NULL) continue;
        //printf("0x%x %x %x %x %x\n", dev, dev->vendorID, dev->deviceID, dev->class_code, dev->progIF);
        if(dev->class_code == class && dev->progIF == progIF){
            printf("Found target @ %d\n", i);
            target = dev;
            break;
        }
    }
    return target;
}