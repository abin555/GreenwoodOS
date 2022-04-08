#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "io.h"
#include "stdint.h"

#define KEYBOARD_MAX_ASCII 256
#define KEYBOARD_BUFFERSIZE 100

enum KEYBOARD_ENCODER_IO{
    KEYBOARD_ENC_INPUT_BUF  =   0x60,
    KEYBOARD_ENC_CMD_REG    =   0x60
};

enum KEYBOARD_CTRL_IO{
    KEYBOARD_CTRL_STATS_REG =   0x64,
    KEYBOARD_CTRL_CMD_REG   =   0x64
};

enum KYBRD_CTRL_STATS_MASK {
 
	KYBRD_CTRL_STATS_MASK_OUT_BUF	=	1,		//00000001
	KYBRD_CTRL_STATS_MASK_IN_BUF	=	2,		//00000010
	KYBRD_CTRL_STATS_MASK_SYSTEM	=	4,		//00000100
	KYBRD_CTRL_STATS_MASK_CMD_DATA	=	8,		//00001000
	KYBRD_CTRL_STATS_MASK_LOCKED	=	0x10,		//00010000
	KYBRD_CTRL_STATS_MASK_AUX_BUF	=	0x20,		//00100000
	KYBRD_CTRL_STATS_MASK_TIMEOUT	=	0x40,		//01000000
	KYBRD_CTRL_STATS_MASK_PARITY	=	0x80		//10000000
};

bool KYBRD_CAPS_LOCK;
bool KYBRD_SHIFT;

uint8_t keyboard_ctrl_read_status ();

void keyboard_ctrl_send_cmd(uint8_t cmd);

uint8_t keyboard_enc_read_buf();

void keyboard_enc_send_cmd(uint8_t cmd);

void keyboard_set_leds(bool num, bool caps, bool scroll);

bool keyboard_self_test();

void keyboard_disable();
void keyboard_enable();

int keyboard_keyread();

void keyboard_flag_handler(unsigned char scan_code);
void keyboard_handle_interrupt();

char convertascii(unsigned char scan_code);

char kbd_US[KEYBOARD_MAX_ASCII];
char keyboard_KEYBUFFER[KEYBOARD_BUFFERSIZE];
unsigned int keyboard_KEYBUFFER_POINTER;
unsigned char prev_Scancode;
unsigned char char_scancode;

#endif
