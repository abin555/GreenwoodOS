#ifndef MIC_H
#define MIC_H

#include "stdint.h"
#include "vfs.h"
#include "sysfs.h"

struct Microphone {
    struct SysFS_Inode *micfile;
    uint16_t *audio_data;
    size_t audio_buf_size;
    void *audio_device;
};

void microphone_init(void *audio_device);

#endif