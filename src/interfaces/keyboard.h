#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "KBD_US_SC2.h"
#include "allocator.h"
#include "memory.h"
#include "serial.h"
#include "framebuffer.h"
#include "window.h"
#include "multitasking.h"
#include "program.h"

extern uint32_t keyboard_buffer_size;
extern uint8_t *keyboard_KEYBuffer;
extern char *keyboard_ASCIIBuffer;

typedef enum{
	PS2_KBD = 1,
	USB_KBD = 2
} KBD_SOURCE;

typedef enum{
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
} KBD_ARROW;

struct KBD_flags{
	uint8_t shift;
	uint8_t ctrl;
	KBD_ARROW arrow;
	uint8_t backspace;
	uint8_t release;
	uint8_t special;
	char key;
};

extern struct KBD_flags KBD_flags;
extern uint32_t KBD_scancode_buffer_idx;
extern uint32_t KBD_ascii_buffer_idx;
extern uint32_t KBD_last_key_idx;
extern char key_pressed_map[0xFF];

void kbd_init(uint32_t buffer_size);

void kbd_recieveScancode(uint8_t scancode, KBD_SOURCE source);

char kbd_getChar();
char getc_blk();
int getArrow();

void *kbd_createCDEV();
void *kbdTxt_createCDEV();
#endif