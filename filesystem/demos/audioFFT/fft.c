#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "fft.h"

void fft(Complex *x, int N, Complex *scratch){
    if(N <= 1) return;
    for(int i = 0; i < N / 2; i++){
        scratch[i] = x[i * 2];
        scratch[i + N / 2] = x[i * 2 + 1];
    }

    fft(scratch, N/2, x);
    fft(scratch + N / 2, N/2, x);

    for(int k = 0; k < N / 2; k++){
        float theta = 2.0f * M_PI * k / N;
        float cos_theta = cos(theta);
        float sin_theta = sin(theta);

        Complex t;
        t.real = cos_theta * scratch[k + N / 2].real - sin_theta * scratch[k + N / 2].imag;
        t.imag = cos_theta * scratch[k + N / 2].imag + sin_theta * scratch[k + N / 2].real;

        x[k].real = scratch[k].real + t.real;
        x[k].imag = scratch[k].imag + t.imag;
        x[k + N/2].real = scratch[k].real - t.real;
        x[k + N/2].imag = scratch[k].imag - t.imag;
    }
}

Complex *workspace;

void fft_workspace_init(int size){
    workspace = malloc(sizeof(Complex) * size);
}

void fft_from_audio(float *samples, Complex *signal_buf, int N, char isDualChannel){
    for(int i = 0; i < N; i++){
        signal_buf[i].real = isDualChannel ? (float) samples[i * 2] : (float) samples[i];
        signal_buf[i].imag = 0.0f;
    }
    fft(signal_buf, N, workspace);
}