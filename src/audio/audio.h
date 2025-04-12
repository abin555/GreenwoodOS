#ifndef AUDIO_H
#define AUDIO_H
#include "stddef.h"
#include "stdbool.h"
#include "stdint.h"

#define SIZE_OF_PCM_DATA_BUFFER 4*48000*4 //four seconds for 2 channels with 16 bit samples
#define SOUND_BUFFER_0 0
#define SOUND_BUFFER_1 1
struct sound_buffer_refilling_info_t {
 uint8_t *source_data_pointer;
 uint32_t source_data_length;
 void (*fill_buffer)(uint8_t *buffer);
 uint32_t buffer_size;
 uint8_t *buffer_0_pointer;
 uint8_t *buffer_1_pointer;
 uint8_t actually_playing_buffer: 4;
 uint8_t last_filled_buffer: 4;
 uint32_t played_bytes_by_finished_buffers;
 uint32_t played_bytes;
 uint32_t size_of_full_pcm_output_in_bytes;
}__attribute__((packed));
extern struct sound_buffer_refilling_info_t *sound_buffer_refilling_info;

extern int sound_volume;
extern struct audio_driver *audio_drivers[4];
extern uint8_t *pcm_data;
extern int selected_dev;

typedef enum {
	AUDIO_AC97
} AUDIO_DEVICE;

struct AC97_driver;

struct audio_driver {
	struct PCI_driver *pci;

	AUDIO_DEVICE deviceType;
	bool bar_type;
    uint32_t mem_base;
    uint32_t nam_base;
	uint32_t nabm_base;
	unsigned int int_number;

	union {
		struct AC97_driver *ac97;
	} device;
};

void audio_driver_init(struct PCI_driver *pci);

void audio_init();
void audio_set_volume(uint8_t volume);
uint8_t audio_is_format_supported(uint8_t channels, uint8_t bits_per_channel, uint32_t sample_rate);
uint32_t audio_get_actual_stream_position();
void audio_play_refilling(uint8_t *source, uint32_t source_len, uint32_t pcmFullSize, uint32_t sample_rate, uint32_t buffer_size, void (*fill_buffer)(uint8_t *buffer));
void audio_stop();
#endif