#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "console.h"
#include "interrupts.h"
#include "pic.h"
#include "memory.h"
#include "ps2.h"
#include "KBD_US_SC2.h"

#define KBD_SSC_CMD 0xF0
#define KBD_SSC_GET 0x00
#define KBD_SSC_2 0x02
#define KBD_SSC_3 0x03

#define KEYBOARD_BUFFERSIZE 0xFF

uint8_t keyboard_KEYBUFFER[KEYBOARD_BUFFERSIZE];
char keyboard_ASCIIBuffer[KEYBOARD_BUFFERSIZE];

struct keyboard_flags{
    uint8_t shift : 1;
    uint8_t ctrl : 1;
    uint8_t arrow : 1;
};
struct keyboard_flags kbd_flags;

uint32_t keyboard_KEYBUFFER_index;
uint32_t keyboard_ascii_index;

void initialize_ps2_keyboard(int device);

void keyboard_handler(struct cpu_state cpu, struct stack_state stack);

#endif