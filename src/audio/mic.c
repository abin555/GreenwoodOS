#include "mic.h"
#include "audio.h"
#include "ac97.h"

struct Microphone microphone_dev;

void microphone_init(void *audio_device){
    print_serial("[MIC] Init for device at 0x%x\n", audio_device);
    struct audio_driver *dev = audio_device;
    if(dev == NULL) return;
    if(dev->deviceType == AUDIO_AC97){
        struct AC97_driver *ac97 = dev->device.ac97;
        microphone_dev.audio_device = dev;
        microphone_dev.audio_data = ac97->mic_data_ptr;
        microphone_dev.audio_buf_size = 4096 * 2;
    }
    struct SysFS_Chardev *cdev = sysfs_createCharDevice((char *) microphone_dev.audio_data, microphone_dev.audio_buf_size, CDEV_READ);
    struct SysFS_Inode *micFile = sysfs_mkcdev(
        "mic",
        cdev
    );
    struct VFS_Inode *vfs_sysroot = vfs_findRoot('-');
    struct SysFS_Inode *sysfs = vfs_sysroot->root->interface->root;
    sysfs_addChild(sysfs_find(sysfs, "dev\0"), micFile);
}
