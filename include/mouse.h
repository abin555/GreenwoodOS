#ifndef MOUSE_H
#define MOUSE_H
#include "console.h"
#include "frame_buffer.h"
#include "io.h"
#include "stdint.h"
#include "ps2.h"
#include "interrupts.h"


// Mouse-specific commands
#define MOUSE_SET_SAMPLE 0xF3
#define MOUSE_SET_RESOLUTION 0xE8
#define MOUSE_ENABLE_SCALING 0xE7
#define MOUSE_DISABLE_SCALING 0xE7

// Bits of the first mouse data packet
#define MOUSE_Y_OVERFLOW (1 << 7)
#define MOUSE_X_OVERFLOW (1 << 6)
#define MOUSE_Y_NEG (1 << 5)
#define MOUSE_X_NEG (1 << 4)
#define MOUSE_ALWAYS_SET (1 << 3)
#define MOUSE_MIDDLE (1 << 2)
#define MOUSE_RIGHT (1 << 1)
#define MOUSE_LEFT (1 << 0)

// Bits of the fourth mouse data packet
#define MOUSE_UNUSED_A (1 << 7)
#define MOUSE_UNUSED_B (1 << 6)

extern uint8_t mouse_cycle;
extern char mouse_byte[3];
extern int mouse_x;
extern int mouse_y;
extern uint32_t mouse_replace_pixels[8*8];

void mouse_interrupt_handler();
void mouse_handle_packet();


void mouse_init(uint8_t device);

void mouse_set_sample_rate(uint8_t rate);
void mouse_set_resolution(uint8_t level);
void mouse_set_scaling(char enabled);
void mouse_enable_scroll_wheel();
void mouse_enable_five_buttons();

#endif