#ifndef UTILS_H
#define UTILS_H

#include "stdint.h"

struct IVec2{
    int x;
    int y;
};

int strcmp(const char *s1, const char *s2);
int strlen(char *s);
char *strdup(char *s);
char *strndup(char *s, int n);

#endif