#ifndef PCSPK_H
#define PCSPK_H

#include "io.h"
#include "timer.h"

void play_sound(uint32_t frequency);
void mute();
void beep();

#endif