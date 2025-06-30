#ifndef AC97_H
#define AC97_H

#include "pci.h"
#include "stdint.h"
#include "serial.h"
#include "interrupts.h"
#include "memory.h"
#include "paging.h"
#include "audio.h"

#define AC97_BUFFER_NOT_FILLED 1
#define AC97_BUFFER_FILLED 0

#define AC97_NAM_IO_RESET 0x00
#define AC97_NAM_IO_CAPABILITES 0x00
 #define AC97_CAPABILITY_HEADPHONE_OUTPUT 0x10
#define AC97_NAM_IO_MASTER_VOLUME 0x02
#define AC97_NAM_IO_AUX_OUT_VOLUME 0x04
#define AC97_NAM_IO_PCM_OUT_VOLUME 0x18
#define AC97_NAM_IO_EXTENDED_CAPABILITIES 0x28
 #define AC97_EXTENDED_CAPABILITY_VARIABLE_SAMPLE_RATE 0x1
#define AC97_NAM_IO_EXTENDED_FEATURES_CONTROL 0x2A
#define AC97_NAM_IO_VARIABLE_SAMPLE_RATE_FRONT_DAC 0x2C
#define AC97_NAM_IO_VARIABLE_SAMPLE_RATE_SURR_DAC 0x2E
#define AC97_NAM_IO_VARIABLE_SAMPLE_RATE_LFE_DAC 0x30
#define AC97_NAM_IO_VARIABLE_SAMPLE_RATE_LR_ADC 0x32
#define AC97_NAM_IO_JACK_SENSE 0x72

#define AC97_NABM_IO_PCM_OUTPUT_BUFFER_BASE_ADDRESS 0x10
#define AC97_NABM_IO_PCM_OUTPUT_CURRENTLY_PROCESSED_ENTRY 0x14
#define AC97_NABM_IO_PCM_OUTPUT_LAST_VALID_ENTRY 0x15
#define AC97_NABM_IO_PCM_OUTPUT_STATUS 0x16
#define AC97_NABM_IO_PCM_OUTPUT_CURRENT_ENTRY_POSITION 0x18
#define AC97_NABM_IO_PCM_OUTPUT_CONTROL 0x1B
#define AC97_NABM_IO_GLOBAL_CONTROL 0x2C

#define AC97_NABM_IO_PCM_INPUT_CONTROL 0x0B

// Microphone Input DMA (Bus Master Registers)
#define AC97_NABM_IO_MICROPHONE_INPUT_BUFFER_BASE_ADDRESS      0x20
#define AC97_NABM_IO_MICROPHONE_INPUT_CURRENTLY_PROCESSED_ENTRY 0x24
#define AC97_NABM_IO_MICROPHONE_INPUT_LAST_VALID_ENTRY         0x25
#define AC97_NABM_IO_MICROPHONE_INPUT_STATUS                   0x26
#define AC97_NABM_IO_MICROPHONE_INPUT_CURRENT_ENTRY_POSITION   0x28
#define AC97_NABM_IO_MICROPHONE_INPUT_CONTROL                  0x2B
#define AC97_NAM_IO_MIC_VOLUME 0x0E
#define AC97_NAM_IO_MIC_SAMPLE_RATE 0x3C

#define AC97_SPEAKER_OUTPUT 0
#define AC97_HEADPHONE_OUTPUT 1

#define AC97_SPEAKER_OUTPUT_NUMBER_OF_VOLUME_STEPS 31

struct ac97_buffer_entry {
 uint32_t sample_memory;
 uint16_t number_of_samples;
 uint16_t reserved: 14;
 uint8_t last_buffer_entry: 1;
 uint8_t interrupt_on_completion: 1;
}__attribute__((packed));

struct AC97_driver {
    struct ac97_buffer_entry *buffer_memory_pointer;
    uint8_t headphone_output_present;
    uint8_t free_entry;
    uint8_t playing_state;
    uint8_t selected_output;
    uint8_t aux_out_number_of_volume_steps;
    uint16_t nam_base;
    uint16_t nabm_base;
    uint16_t extended_capabilities;
    uint32_t sound_memory;
    uint32_t number_of_samples_in_one_channel;

    void *private_page;
    uint32_t page_head;
    struct ac97_buffer_entry *microphone_ptr;
    void *mic_data_ptr;
};

struct audio_driver *ac97_init(struct PCI_driver *driver);
uint8_t ac97_is_headphone_connected(struct audio_driver *audio);
void ac97_set_volume_in_register(struct audio_driver *audio, uint32_t offset, uint8_t numVolSteps, uint8_t vol);
void ac97_set_output(struct audio_driver *audio, uint8_t output);
void ac97_set_volume(struct audio_driver *audio, uint8_t volume);
uint8_t ac97_is_supported_sample_rate(struct audio_driver *audio, uint16_t sample_rate);
void ac97_set_sample_rate(struct audio_driver *audio, uint16_t sample_rate);
void ac97_play_pcm_data_in_loop(struct audio_driver *audio, uint16_t sample_rate);
uint32_t ac97_get_actual_stream_position(struct audio_driver *audio);
void ac97_stop_sound(struct audio_driver *audio);
void ac97_start_microphone_listen(struct audio_driver *audio);
#endif