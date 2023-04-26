#ifndef AUDIO_H
#define AUDIO_H

#include "stdint.h"
#include "console.h"
#include "pcspk.h"

typedef struct audio_position {
    uint32_t buffer;
    uint32_t frame;
} audio_position_t;

typedef enum {
    CDI_AUDIO_STOP = 0,
    CDI_AUDIO_PLAY = 1
} audio_status_t;

#ifndef AUDIO_STRUCTS
typedef enum {
    // 16 bit signed integer
    CDI_AUDIO_16SI = 0,
    // 8 bit signed integer
    CDI_AUDIO_8SI,
    // 32 bit signed integer
    CDI_AUDIO_32SI
} audio_sample_format_t;


struct audio_stream{
    struct audio_device_hw* device;
    uint32_t num_buffers;
    uint32_t buffer_size;
    int fixed_sample_rate;
    audio_sample_format_t sample_format;
};

struct audio_device_hw{
    void* device;
    int record;//0 = playback, 1 = recording.
    void* stream;//Generous buffer of two pointers to possible audio streams.
};
#endif


enum sample_format {
    SR_48_KHZ               = 0,
    SR_44_KHZ               = (1 << 14),
    BITS_32                 = (4 <<  4),
    BITS_16                 = (1 <<  4),
};

typedef enum{
    INTEL_HDA = 1
} audio_device_type_t;

struct audio_device{
    audio_device_type_t type;
    struct audio_device_hw* hardware;
    void (*set_volume)(struct audio_stream* stream, uint8_t volume);
    int (*set_sample_rate)(struct audio_stream* stream, int sample_rate);
    int (*set_number_of_channels)(struct audio_device* device, int channels);
    int (*transfer_data)(struct audio_stream* stream, uint32_t size, uint32_t* memory, uint32_t offset);//Size is in units of buffer size??
    audio_status_t (*change_device_status)(struct audio_device* device, audio_status_t status);
    void (*get_position)(struct audio_stream* stream, audio_position_t* position);
};



struct audio_device* audio_devices[4];
int audio_active_driver;
int audio_device_count;

bool audio_muted;
bool audio_active;

void audio_init();

void add_audio_device(struct audio_device* audio_dev);
void audio_play(uint32_t *buffer, uint32_t size);
void audio_beep();

typedef enum{
    MUTE = 1,
    UNMUTE = 0
} audio_enable;

void audio_setEnable(audio_enable state);
#endif