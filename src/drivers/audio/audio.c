#include "audio.h"

void add_audio_device(struct audio_device* audio_dev){
    audio_devices[audio_active_driver] = audio_dev;
    audio_active_driver = audio_device_count;
    audio_device_count++;
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
    audio_devices[audio_active_driver]->set_number_of_channels(audio_devices[audio_active_driver], 2);
    audio_devices[audio_active_driver]->set_sample_rate(audio_devices[audio_active_driver]->hardware->stream, SR_44_KHZ);
    audio_devices[audio_active_driver]->transfer_data(audio_devices[audio_active_driver]->hardware->stream, size, buffer, 0);
    audio_devices[audio_active_driver]->change_device_status(audio_devices[audio_active_driver], CDI_AUDIO_PLAY);
}

void audio_init(){
    printk("[AUDIO] Init\n");
    audio_muted = true;
    audio_active = true;
}

void audio_beep(){
    if(!audio_muted){
        beep();
    }
}

void audio_setEnable(audio_enable state){
    audio_muted = state;
    switch(state){
        case MUTE:
            printk("[AUDIO] Muted!\n");
            break;
        case UNMUTE:
            printk("[AUDIO] Unmuted!\n");
            break;
    }
}