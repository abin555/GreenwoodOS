#ifndef MOUSE_H
#define MOUSE_H
#include "console.h"
#include "frame_buffer.h"
#include "io.h"
#include "stdint.h"

extern uint8_t mouse_cycle;
extern char mouse_byte[3];
extern char mouse_x;
extern char mouse_y;

void mouse_interrupt_handler(unsigned int interrupt);

void mouse_wait(uint8_t type);
void mouse_write(uint8_t data);
uint8_t mouse_read();

void mouse_init();


#endif