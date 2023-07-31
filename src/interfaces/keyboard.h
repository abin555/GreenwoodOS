#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "KBD_US_SC2.h"
#include "allocator.h"
#include "serial.h"
#include "framebuffer.h"

uint32_t keyboard_buffer_size;
uint8_t *keyboard_KEYBuffer;
char *keyboard_ASCIIBuffer;

typedef enum{
	PS2_KBD = 1,
	USB_KBD = 2
} KBD_SOURCE;

struct KBD_flags{
	uint8_t shift;
	uint8_t ctrl;
	uint8_t arrow;
};

struct KBD_flags KBD_flags;

uint32_t KBD_scancode_buffer_idx;
uint32_t KBD_ascii_buffer_idx;

void kbd_init(uint32_t buffer_size);

void kbd_recieveScancode(uint8_t scancode, KBD_SOURCE source);
uint32_t KBD_last_key_idx;
char kbd_getChar();

#endif