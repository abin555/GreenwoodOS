#ifndef FFT_H
#define FFT_H

typedef struct {
    float real;
    float imag;
} Complex;

void fft(Complex *x, int N, Complex *scratch);
void fft_workspace_init(int size);
void fft_from_audio(float *samples, Complex *signal_buf, int N, char isDualChannel);

#endif