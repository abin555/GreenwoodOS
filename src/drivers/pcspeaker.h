#ifndef PCSPEAKER_H
#define PCSPEAKER_H

#include "io.h"
#include "interrupts.h"

struct PCSpeaker_Handle{
    void (*play)(uint32_t);
    void (*mute)(void);
    void (*beep)(void);
};

extern struct PCSpeaker_Handle PCSpeaker_Handle;


#endif