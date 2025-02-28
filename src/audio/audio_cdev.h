#ifndef AUDIO_CDEV_H
#define AUDIO_CDEV_H

#include "wav.h"
#include "audio.h"

struct AudioInterface {
    void (*set_volume)(uint8_t volume);
    void (*stop)();
    void *(*wav_read)(uint8_t * wav_memory, uint32_t len);
    uint32_t (*wav_play)(void *wav, uint32_t offset);
};

void *audio_createCDEV();

#endif