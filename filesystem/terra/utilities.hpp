#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "libc.h"
#include "application.hpp"

#define PI 3.14159263f
#define PI2 6.283

float atof(char *arr);
int atoi(char *arr);
float sqrtf(float x);
void memset(void *mem, char v, int size);
float cos(float x);
float sin(float x);
int abs(int x);
void memfcpy(void* target, void* source, uint32_t size);
void memcpy(void *dest, void *src, unsigned int len);

float min3(float a, float b, float c);
float max3(float a, float b, float c);

int max(int a, int b);
int min(int a, int b);

#endif