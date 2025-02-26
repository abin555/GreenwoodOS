#include "audio.h"
#include "pci.h"
#include "interrupts.h"
#include "serial.h"
#include "timer.h"
#include "ac97.h"

int sound_volume;
struct audio_driver *audio_drivers[4] = {NULL, NULL, NULL, NULL};
uint8_t *pcm_data;
int selected_dev;

struct sound_buffer_refilling_info_t *sound_buffer_refilling_info;

bool audio_assign_driver(struct audio_driver *driver) {
    if(driver == NULL) return false;
    for (int i = 0; i < 4; i++) {
        if (audio_drivers[i] == NULL) {
            audio_drivers[i] = driver;
            print_serial("[AUDIO] Assigned audio driver to interface %d\n", i);
            return true;
        }
    }

    return false;
}

void audio_driver_init(struct PCI_driver *pci){
    struct audio_driver *audio;
    if(
        pci->device->vendorID == 0x8086 &&
        pci->device->deviceID == 0x2415
    ){
        print_serial("[AUDIO] Init AC97 Driver\n");
        audio = ac97_init(pci);
    }

    if(!audio_assign_driver(audio)){
        print_serial("[AUDIO] Unable to add audio device!\n");
        return;
    }

    print_serial("[AUDIO] Init Complete\n");
}

void audio_init(){
    print_serial("[AUDIO] Init\n");
    selected_dev = -1;
    for(int i = 0; i < 4; i++){
        if(audio_drivers[i] != NULL) selected_dev = i;
    }
    print_serial("[AUDIO] Selected Device %d\n", selected_dev);
    audio_set_volume(30);

    sound_buffer_refilling_info = (struct sound_buffer_refilling_info_t *) malloc(sizeof(struct sound_buffer_refilling_info_t));
    pcm_data = (uint8_t *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(SIZE_OF_PCM_DATA_BUFFER), SIZE_OF_PCM_DATA_BUFFER, 0, OTHER));
    MEM_printRegions();
}

void audio_set_volume(uint8_t volume){
    print_serial("[AUDIO] Set volume to %d (Dev %d)\n", volume, selected_dev);
    if(selected_dev != -1){
        if(audio_drivers[selected_dev]->deviceType == AUDIO_AC97){
            ac97_set_volume(audio_drivers[selected_dev], volume);
        }
    }
    sound_volume = volume;
}

uint8_t audio_is_format_supported(uint8_t channels, uint8_t bits_per_channel, uint32_t sample_rate){
    if(selected_dev == -1) return 0;

    if(audio_drivers[selected_dev]->deviceType == AUDIO_AC97){
        if(channels==2 && bits_per_channel == 16) {
            return ac97_is_supported_sample_rate(audio_drivers[selected_dev], sample_rate);
        }
        else{
            return 0;
        }
    }
    return 0;
}

uint32_t audio_get_actual_stream_position(){
    if(selected_dev == -1) return 0;
    if(audio_drivers[selected_dev]->deviceType == AUDIO_AC97){
        return ac97_get_actual_stream_position(audio_drivers[selected_dev]);
    }
    return 0;
}

void task_refill_sound_buffer();
void audio_play_refilling(uint8_t *source, uint32_t source_len, uint32_t pcmFullSize, uint32_t sample_rate, uint32_t buffer_size, void (*fill_buffer)(uint8_t *buffer)){
    print_serial("[AUDIO] Playing Audio\n");
    sound_buffer_refilling_info->source_data_pointer = source;
    sound_buffer_refilling_info->source_data_length = source_len;
    sound_buffer_refilling_info->fill_buffer = fill_buffer;
    sound_buffer_refilling_info->buffer_size = buffer_size;
    sound_buffer_refilling_info->buffer_0_pointer = (uint8_t *) pcm_data;
    sound_buffer_refilling_info->buffer_1_pointer = (uint8_t *) ((uint32_t)pcm_data + buffer_size);
    sound_buffer_refilling_info->actually_playing_buffer = SOUND_BUFFER_0;
    sound_buffer_refilling_info->last_filled_buffer = SOUND_BUFFER_0;
    sound_buffer_refilling_info->played_bytes_by_finished_buffers = 0;
    sound_buffer_refilling_info->played_bytes = 0;
    sound_buffer_refilling_info->size_of_full_pcm_output_in_bytes = pcmFullSize;

    memset(pcm_data, 0, SIZE_OF_PCM_DATA_BUFFER);

    (*fill_buffer)(pcm_data);

    if(selected_dev != -1){
        if(audio_drivers[selected_dev]->deviceType == AUDIO_AC97){
            ac97_play_pcm_data_in_loop(audio_drivers[selected_dev], sample_rate);
        }
    }

    timer_attach(10, task_refill_sound_buffer);
}

void task_refill_sound_buffer(){
    //print_serial("[AUDIO] Refilling Buffer\n");
    // calculate how many bytes were played
    if (sound_buffer_refilling_info->actually_playing_buffer == SOUND_BUFFER_0){
        if (audio_get_actual_stream_position() < sound_buffer_refilling_info->buffer_size){
            sound_buffer_refilling_info->played_bytes = (sound_buffer_refilling_info->played_bytes_by_finished_buffers + audio_get_actual_stream_position());
        }
        else{
            sound_buffer_refilling_info->played_bytes_by_finished_buffers += sound_buffer_refilling_info->buffer_size;
            sound_buffer_refilling_info->played_bytes = (sound_buffer_refilling_info->played_bytes_by_finished_buffers + audio_get_actual_stream_position() - sound_buffer_refilling_info->buffer_size);
            sound_buffer_refilling_info->actually_playing_buffer = SOUND_BUFFER_1;
        }
    }
    else{
        if (audio_get_actual_stream_position() >= sound_buffer_refilling_info->buffer_size){
            sound_buffer_refilling_info->played_bytes = (sound_buffer_refilling_info->played_bytes_by_finished_buffers + audio_get_actual_stream_position() - sound_buffer_refilling_info->buffer_size);
        }
        else{
            sound_buffer_refilling_info->played_bytes_by_finished_buffers += sound_buffer_refilling_info->buffer_size;
            sound_buffer_refilling_info->played_bytes = (sound_buffer_refilling_info->played_bytes_by_finished_buffers + audio_get_actual_stream_position());
            sound_buffer_refilling_info->actually_playing_buffer = SOUND_BUFFER_0;
        }
    }

    // if is everything played, stop sound
    if (sound_buffer_refilling_info->played_bytes >= sound_buffer_refilling_info->size_of_full_pcm_output_in_bytes){
        //audio_stop();
        sound_buffer_refilling_info->played_bytes = sound_buffer_refilling_info->size_of_full_pcm_output_in_bytes;
    }

    // update values of actually played audio file
    //audio_calculate_time_of_sound_data_offset(actually_played_audio_file, actually_played_audio_file->offset_to_start_of_playing + sound_buffer_refilling_info->played_bytes);

    // fill buffer with new data
    if (sound_buffer_refilling_info->actually_playing_buffer == sound_buffer_refilling_info->last_filled_buffer){
        if (sound_buffer_refilling_info->last_filled_buffer == SOUND_BUFFER_0){
            print_serial("[AUDIO] Fill A\n");
            sound_buffer_refilling_info->fill_buffer((uint8_t *)((uint32_t)pcm_data + sound_buffer_refilling_info->buffer_size));
            sound_buffer_refilling_info->last_filled_buffer = SOUND_BUFFER_1;
        }
        else{
            print_serial("[AUDIO] Fill B\n");
            sound_buffer_refilling_info->fill_buffer(pcm_data);
            sound_buffer_refilling_info->last_filled_buffer = SOUND_BUFFER_0;
        }
        asm("wbinvd"); // flush processor cache to RAM to be sure sound card will read correct data
    }
}

void audio_stop(){
    print_serial("[AUDIO] Stopping Audio\n");
    if(selected_dev == -1) return;
    timer_detach(task_refill_sound_buffer);

    if(audio_drivers[selected_dev]->deviceType == AUDIO_AC97){
        ac97_stop_sound(audio_drivers[selected_dev]);
    }
}