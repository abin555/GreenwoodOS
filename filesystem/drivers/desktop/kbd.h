#ifndef DTOP_KBD_H
#define DTOP_KBD_H

#include <stdint.h>

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

#endif