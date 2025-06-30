#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/task.h>
#include "fft.h"
#include "tinywav.h"
#include "graph.h"

#define NumPoints 2000

int main(int argc, char *argv){
    freopen("/-/dev/serial", "w", stdout);
    printf("Starting Audio FFT\n");
    fft_workspace_init(NumPoints);

    //FILE *mic = fopen("/-/dev/mic", "r");
    
    TinyWav tw;
    tinywav_open_read(&tw, 
        "/A/Audio/FDJ.wav",
        TW_INLINE
    );
    printf("There are %d channels\n%d frames in header\nsample rate is %d\n",
        tw.numChannels, tw.numFramesInHeader, tw.h.SampleRate
    );
    

    float *samples = malloc(sizeof(float) * tw.numChannels * tw.h.SampleRate);
    //uint16_t *rawsamples = malloc(sizeof(uint16_t) * 4096 * 2);
    //float *samples = malloc(sizeof(float) * 4096 * 2);
    float *mag = malloc(sizeof(float) * NumPoints);
    struct GraphWindow *graph = graph_open(250, 250, "Real Time FFT");
    Complex *signal = malloc(sizeof(Complex) * NumPoints);
    
    for(int step = 0; step >= 0; step++){
        /*
        fread(rawsamples, 4096*2*sizeof(float), 1, mic);
        fseek(mic, 0, SEEK_SET);
        for(int i = 0; i < 4096*2; i++){
            samples[i] = rawsamples[i] / 32768.0f;
        }
        */
        tinywav_read_f(&tw, samples, NumPoints);
        fft_from_audio(samples, signal, NumPoints, 0);
        
        for(int i = 0; i < NumPoints / 2; i++){
            float real = signal[i].real;
            float imag = signal[i].imag;
            float magnitude = sqrt(real * real + imag * imag);
            mag[i] = magnitude;
        }
        graph_update(graph, mag, NumPoints / 2);
        yield();
    }

    tinywav_close_read(&tw);

    graph_close(graph);

    return 0;
}