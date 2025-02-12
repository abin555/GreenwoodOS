#ifndef MEMORY_H
#define MEMORY_H

#include "libc.h"

void memsetup(uint32_t regionSize);

void *malloc(uint32_t size);

void memfinish();

#endif