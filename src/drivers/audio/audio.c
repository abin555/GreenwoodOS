#include "audio.h"

void add_audio_device(struct audio_device* audio_dev){
    audio_devices[0] = audio_dev;
    audio_device_present = true;
    char* dev_name;
    switch(audio_dev->type){
        case(INTEL_HDA):
            dev_name = "Intel High Definition Audio";
            break;
        default:
            dev_name = "Unknown Audio";
            break;
    }
    printk("[AUDIO] Added %s Device\n", dev_name);
}

void audio_play(uint32_t *buffer, uint32_t size){
    audio_devices[0]->set_number_of_channels(audio_devices[0], 2);
    audio_devices[0]->set_sample_rate(audio_devices[0]->hardware->stream, SR_44_KHZ);
    audio_devices[0]->transfer_data(audio_devices[0]->hardware->stream, size, buffer, 0);
    audio_devices[0]->change_device_status(audio_devices[0], CDI_AUDIO_PLAY);
}