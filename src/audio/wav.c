#include "wav.h"

struct WAV_audio *playing_audio;

struct WAV_audio *wav_read_info(uint8_t *wav_memory, uint32_t wav_length)
{
    struct WAV_audio *wav = (struct WAV_audio *)(malloc(sizeof(struct WAV_audio)));

    uint32_t *wav32 = (uint32_t *)wav_memory;
    uint32_t end_of_wav = ((uint32_t)wav_memory + wav_length);

    // check signature
    if (wav32[0] != 0x46464952 && wav32[2] != 0x45564157)
    {
        print_serial("WAV: invalid signature\n");
        return 0;
    }

    // parse chunks
    wav32 = (uint32_t *)((uint32_t)wav_memory + 12);
    while ((uint32_t)wav32 < end_of_wav && wav32[0] != 0x00000000)
    {
        if (wav32[0] == 0x20746D66)
        { // fmt
            // check sound format
            if ((wav32[2] & 0xFFFF) != 0x0001)
            {
                print_serial("WAV: not raw PCM\n");
                return 0;
            }

            // read PCM data format
            wav->pcm_num_channels = (wav32[2] >> 16);
            wav->sample_rate = wav32[3];
            wav->bits_per_sample = (wav32[5] >> 16);
            // recalculate what sample rate we will use
            wav->output_sample_rate = 0;
            if (wav->sample_rate == 48000 || wav->sample_rate == 24000 || wav->sample_rate == 12000)
            {
                wav->output_sample_rate = 48000;
            }
            else if (wav->sample_rate == 44100 || wav->sample_rate == 22050 || wav->sample_rate == 11025)
            {
                wav->output_sample_rate = 44100;
            }

            print_serial("[WAV] Audio: %d %d %d\n", wav->pcm_num_channels, wav->sample_rate, wav->bits_per_sample);
        }
        else if (wav32[0] == 0x61746164)
        { // data
            if (wav->pcm_num_channels == 0 || wav->sample_rate == 0 || wav->bits_per_sample == 0)
            {
                print_serial("WAV: data chunk before fmt chunk\n");
                return 0;
            }

            // read info about PCM data
            wav->pcm_start = ((uint32_t)wav32 + 8);
            wav->pcm_len = wav32[1];
            if (wav->output_sample_rate != 0 && (wav->bits_per_sample == 8 || wav->bits_per_sample == 16))
            {
                // recalculate length for 2 channels
                wav->len_out_data = ((wav->pcm_len / wav->pcm_num_channels) * 2);

                // recalculate according 16 bits per sample
                if (wav->bits_per_sample == 8)
                {
                    wav->len_out_data *= 2;
                }

                // recalculate according to new sample rate
                wav->len_out_data = (wav->len_out_data * (wav->output_sample_rate / wav->sample_rate));
            }
            else
            {
                print_serial("Unplayable WAV: %d %d %d\n", wav->pcm_num_channels, wav->sample_rate, wav->bits_per_sample);
                wav->len_out_data = 0;
                return 0;
            }

            return wav;
        }
        else
        {
            print_serial("WAV: unknown chunk 0x%x\n", wav32[0]);
        }

        // go to next chunk
        wav32 = (uint32_t *)((uint32_t)wav32 + wav32[1] + 8);
    }

    return 0;
}

uint32_t wav_play(struct WAV_audio *wav, uint32_t offset_to_first_uint8_to_play)
{
    print_serial("[WAV] Playing audio\n");
    // calculate mp3 PCM data offset
    uint32_t offset_to_first_uint8_to_pcm_data_in_original_wav = ((offset_to_first_uint8_to_play / 2 / 2) / (wav->output_sample_rate / wav->sample_rate) * (wav->pcm_num_channels * (wav->bits_per_sample / 8)));

    // save wav info pointer
    playing_audio = wav;

    // play wav file
    audio_play_refilling((uint8_t *)wav->pcm_start + offset_to_first_uint8_to_pcm_data_in_original_wav, wav->pcm_len - offset_to_first_uint8_to_pcm_data_in_original_wav, wav->len_out_data - offset_to_first_uint8_to_play, wav->output_sample_rate, wav->output_sample_rate * 4, wav_refill_buffer); // buffer contains 1 sec of PCM data
    return 0;
}

void wav_refill_buffer(uint8_t *buffer)
{
    // clear buffer
    //clear_memory((uint32_t)buffer, sound_buffer_refilling_info->buffer_size);
    memset(buffer, 0, sound_buffer_refilling_info->buffer_size);
    print_serial("[WAV] Refilling Buffer\n");

    // exit if we already converted all file
    if (sound_buffer_refilling_info->source_data_length == 0)
    {
        print_serial("[WAV] Conversion done?\n");
        return;
    }

    // fill buffer
    uint16_t first_channel_sample = 0, second_channel_sample = 0;
    uint32_t number_of_frames_to_convert = 0;//, length_of_processed_data = 0;
    if (sound_buffer_refilling_info->source_data_length < playing_audio->sample_rate)
    {
        number_of_frames_to_convert = (sound_buffer_refilling_info->source_data_length / (playing_audio->pcm_num_channels * (playing_audio->bits_per_sample / 8)));
    }
    else
    {
        number_of_frames_to_convert = playing_audio->sample_rate;
    }

    for (uint32_t i = 0; i < number_of_frames_to_convert; i++)
    {
        uint16_t *wav = (uint16_t *)(sound_buffer_refilling_info->source_data_pointer);

        // read frame
        if (playing_audio->bits_per_sample == 16)
        {
            first_channel_sample = wav[0];
            second_channel_sample = wav[1];
        }
        else if (playing_audio->bits_per_sample == 8)
        {
            first_channel_sample = (wav[0] & 0xFF) * 0x40;
            second_channel_sample = (wav[0] >> 8) * 0x40;
        }

        if (playing_audio->pcm_num_channels == 1)
        {
            second_channel_sample = first_channel_sample;
        }

        // save frame according to new sample rate
        for (uint32_t j = 0; j < (playing_audio->output_sample_rate / playing_audio->sample_rate); j++)
        {
            buffer[0] = (first_channel_sample & 0xFF);
            buffer[1] = (first_channel_sample >> 8);
            buffer[2] = (second_channel_sample & 0xFF);
            buffer[3] = (second_channel_sample >> 8);
            buffer += 4;
        }

        // go to next frame
        sound_buffer_refilling_info->source_data_pointer = (sound_buffer_refilling_info->source_data_pointer + (playing_audio->pcm_num_channels * (playing_audio->bits_per_sample / 8)));
        sound_buffer_refilling_info->source_data_length -= (playing_audio->pcm_num_channels * (playing_audio->bits_per_sample / 8));
    }
}