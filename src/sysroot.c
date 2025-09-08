#include "sysroot.h"
#include "allocator.h"
#include "memory.h"
#include "audio.h"
#include "vfs.h"
#include "utils.h"
#include "audio_cdev.h"
#include "serial.h"
#include "mouse.h"
#include "keyboard.h"
#include "console.h"

void sysroot_serial_write_callback(void *buf, int offset, int nbytes, int *head){
    if(buf == NULL) return;
    struct SysFS_Chardev *serial = buf;
    if(offset >= serial->buf_size) return;
    for(int i = 0; (i + offset < serial->buf_size && i < nbytes && serial->buf[offset+i] != '\0'); i++){
        write_serial(serial->buf[offset+i]);
    }
    *head = 0;
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
    sysfs_setCallbacks(serial_cdev, sysroot_serial_write_callback, NULL, NULL, NULL);
    struct SysFS_Inode *serial = sysfs_mkcdev("serial", serial_cdev);
    struct SysFS_Inode *console = sysfs_mkcdev("console", console_createCDEV());
    
    struct SysFS_Inode *audio = sysfs_mkcdev("audio", audio_createCDEV());
    struct SysFS_Inode *mouse = sysfs_mkcdev("mouse", mouse_createCDEV());
    struct SysFS_Inode *mouseTxt = sysfs_mkcdev("mouseTxt", mouseTxt_createCDEV());
    struct SysFS_Inode *kbd = sysfs_mkcdev("kbd", kbd_createCDEV());
    struct SysFS_Inode *kbdTxt = sysfs_mkcdev("kbdTxt", kbdTxt_createCDEV());
    struct SysFS_Inode *viewport = sysfs_mkcdev("viewport", 
        sysfs_createCharDevice(
            (char *) &global_viewport_functions,
            sizeof(global_viewport_functions),
            CDEV_READ
        )
    );
    struct SysFS_Inode *window_bar_cdev = sysfs_mkcdev("winbar", 
        sysfs_createCharDevice(
            (char *) window_bar_text,
            sizeof(window_bar_text),
            CDEV_READ | CDEV_WRITE
        )
    );
    struct SysFS_Inode *font = sysfs_mkcdev("font",
        sysfs_createCharDevice(
            (char *) FONT,
            sizeof(FONT),
            CDEV_READ | CDEV_WRITE
        )
    );

    sysfs_addChild(devs, serial);
    sysfs_addChild(devs, console);
    sysfs_addChild(systems, audio);
    sysfs_addChild(systems, mouse);
    sysfs_addChild(systems, mouseTxt);
    sysfs_addChild(systems, kbd);
    sysfs_addChild(systems, kbdTxt);
    sysfs_addChild(systems, viewport);
    sysfs_addChild(systems, font);
    sysfs_addChild(systems, window_bar_cdev);
    sysfs_addChild(root, devs);
    sysfs_addChild(root, systems);
    sysfs_addChild(root, sysfs_createMetaFile(root));

    return root;
}