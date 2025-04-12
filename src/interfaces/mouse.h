#ifndef MOUSE_H
#define MOUSE_H

#include "stdint.h"
#include "utils.h"
#include "framebuffer.h"

struct MouseButtons{
    uint8_t right : 1;
    uint8_t left : 1;
    uint8_t middle : 1;
};

struct MouseStatus{
    struct IVec2 pos;
    struct MouseButtons buttons;
    struct IVec2 lastDelta;
};

extern struct MouseStatus mouseStatus;

void mouse_init();
void *mouse_createCDEV();
void *mouseTxt_createCDEV();

void mouse_update(int deltaX, int deltaY, struct MouseButtons btns);

struct IVec2 mouse_getPos();

#endif