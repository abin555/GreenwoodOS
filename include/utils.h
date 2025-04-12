#ifndef UTILS_H
#define UTILS_H
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

struct IVec2{
    int x;
    int y;
};

int strcmp(const char *s1, const char *s2);
int strncmp( const char * s1, const char * s2, size_t n );
int strlen(char *s);
char *strdup(char *s);
char *strndup(char *s, int n);
int sprintf(char *str, const char *format, ...);

#endif