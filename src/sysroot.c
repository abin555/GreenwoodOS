#include "sysroot.h"
#include "allocator.h"
#include "memory.h"
#include "audio.h"
#include "vfs.h"
#include "utils.h"
#include "audio_cdev.h"
#include "serial.h"

void sysroot_serial_write_callback(void *buf, int offset, int nbytes){
    if(buf == NULL) return;
    struct SysFS_Chardev *serial = buf;
    if(offset >= serial->buf_size) return;
    for(int i = 0; i + offset < serial->buf_size && i < nbytes; i++){
        write_serial(serial->buf[offset+i]);
    }
}

struct SysFS_Inode *sysroot_init(){
    struct SysFS_Inode *root = sysfs_createRoot();

    struct SysFS_Inode *devs = sysfs_mkdir("dev");
    struct SysFS_Inode *systems = sysfs_mkdir("sys");

    struct SysFS_Chardev *serial_cdev = sysfs_createCharDevice(
        malloc(100),
        100,
        CDEV_WRITE
    );
    sysfs_setCallbacks(serial_cdev, sysroot_serial_write_callback, NULL);
    struct SysFS_Inode *serial = sysfs_mkcdev("serial", serial_cdev);
    
    struct SysFS_Inode *audio = sysfs_mkcdev("audio", audio_createCDEV());

    sysfs_addChild(devs, serial);
    sysfs_addChild(systems, audio);
    sysfs_addChild(root, devs);
    sysfs_addChild(root, systems);

    return root;
}