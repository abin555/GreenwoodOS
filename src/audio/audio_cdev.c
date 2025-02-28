#include "audio_cdev.h"
#include "vfs.h"
#include "sysfs.h"

struct AudioInterface audioInterface;

void *audio_createCDEV(){
    print_serial("[AUDIO] Generating CDEV\n");
    audioInterface.set_volume = audio_set_volume;
    audioInterface.stop = audio_stop;
    audioInterface.wav_read = (void *(*)(uint8_t * wav_memory, uint32_t len)) wav_read_info;
    audioInterface.wav_play = (uint32_t (*)(void *wav, uint32_t offset)) wav_play;

    struct SysFS_Chardev *audio_cdev = sysfs_createCharDevice(
        (char *) &audioInterface,
        sizeof(struct AudioInterface),
        CDEV_READ
    );
    return (void *) audio_cdev;
}