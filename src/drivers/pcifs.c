#include "pcifs.h"
#include "pci.h"
#include "vfs.h"

struct SysFS_Inode *pcifs_deviceListTxt(){
    size_t cdev_bufsize = PCI_numDevices * (50);
    char *cdev_buf = malloc(sizeof(char) * cdev_bufsize);
    memset(cdev_buf, 0, cdev_bufsize);
    char *write_offset = cdev_buf;
    for(int i = 0; i < PCI_numDevices; i++){
        write_offset += snprintf(
            write_offset, cdev_bufsize - (write_offset - cdev_buf),
            "%d 0x%x:0x%x -> 0x%x (0x%x)\n",
            i, PCI_devices[i]->vendorID, PCI_devices[i]->deviceID, PCI_devices[i]->class_code, PCI_devices[i]->progIF
        );
    }
    size_t cdev_fileSize = write_offset - cdev_buf;
    print_serial("%s\n", cdev_buf);
    struct SysFS_Chardev *cdev = sysfs_createCharDevice(cdev_buf, cdev_fileSize, CDEV_READ);
    struct SysFS_Inode *file = sysfs_mkcdev("devicesTxt", cdev);
    return file;
}

struct SysFS_Inode *pcifs_driverListTxt(){
    size_t cdev_bufsize = PCI_numDrivers * (100);
    char *cdev_buf = malloc(sizeof(char) * cdev_bufsize);
    memset(cdev_buf, 0, cdev_bufsize);
    char *write_offset = cdev_buf;
    for(int i = 0; i < PCI_numDrivers; i++){
        write_offset += snprintf(
            write_offset, cdev_bufsize - (write_offset - cdev_buf),
            "%d \"%s\" init=0x%x 0x%x:0x%x -> 0x%x\n",
            i, PCI_drivers[i]->name, PCI_drivers[i]->init_driver, PCI_drivers[i]->device->vendorID, PCI_drivers[i]->device->deviceID, PCI_drivers[i]->device->class_code
        );
    }
    size_t cdev_fileSize = write_offset - cdev_buf;
    print_serial("%s\n", cdev_buf);
    struct SysFS_Chardev *cdev = sysfs_createCharDevice(cdev_buf, cdev_fileSize, CDEV_READ);
    struct SysFS_Inode *file = sysfs_mkcdev("driversTxt", cdev);
    return file;
}

struct SysFS_Inode *pcifs_init(){
    struct SysFS_Inode *pci_dir = sysfs_mkdir("pci");
    sysfs_addChild(pci_dir, pcifs_deviceListTxt());
    sysfs_addChild(pci_dir, pcifs_driverListTxt());
    return pci_dir;
}