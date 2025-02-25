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

struct WAV_audio {
    struct WAV_header header;
    
    uint32_t pcm_start;
    uint32_t pcm_len;
    uint8_t pcm_num_channels;
    uint16_t sample_rate;
    uint8_t bits_per_sample;

    uint32_t len_out_data;
    uint16_t output_sample_rate;
}__attribute__((packed));

extern struct WAV_audio *playing_audio;

struct WAV_audio *wav_read_info(uint8_t *wav_memory, uint32_t length);
uint32_t wav_play(struct WAV_audio *wav, uint32_t offset);
void wav_refill_buffer(uint8_t *buffer);

#endif