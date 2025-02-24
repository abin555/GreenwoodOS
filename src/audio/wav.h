#ifndef WAV_H
#define WAV_H

#include "pci.h"
#include "interrupts.h"
#include "serial.h"
#include "stdint.h"
#include "audio.h"

struct WAV_header {
    char riff[4];
    uint32_t fileSize;
    char wave[4];
    char fmt[4];
    uint32_t formatLength;
    uint16_t audioFormat;//1 = PCM
    uint16_t numChannels; //1 = Mono, 2 = Stero
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
};

void play_wav(void *wav_file_buffer);

#endif