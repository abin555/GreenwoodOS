#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdarg.h>
#include "allocator.h"
#include "serial.h"
#include "window.h"
#include "framebuffer.h"
#include "viewport.h"

#define MAX_CONSOLE 10

typedef enum {
	CONSOLE_WINDOW,
	CONSOLE_VIEWPORT
} CONSOLE_TYPE;

struct CONSOLE{
	CONSOLE_TYPE view_type;
	struct WINDOW *window;
	struct Viewport *viewport;
	uint32_t width;
	uint32_t height;
	uint32_t buf_size;
	char *buf;
	bool active;
	int last_cursor;
	int cursor;

	uint32_t Line;
	uint32_t Start;
	uint32_t LinePlace;
	uint32_t LastLine;
	struct{
		uint32_t fg;
		uint32_t bg;
	} color;
};

extern struct CONSOLE consoles[MAX_CONSOLE];
extern struct CONSOLE *kernel_console;
void console_init();

struct CONSOLE *console_open(struct WINDOW *window);
struct CONSOLE *console_open_vp(struct Viewport *vp);
void console_close(struct CONSOLE *console);

void print_console(struct CONSOLE *console, char *msg, ...);
char quadToHex(uint8_t quad);
void console_clear(struct CONSOLE *console);
int printFloat(struct CONSOLE *console, uint32_t data);
int printBinary(struct CONSOLE *console, unsigned int data, int setlength);
int printHex(struct CONSOLE *console, unsigned int data, int setlength);
int printDecimal(struct CONSOLE *console, int data, int offset);
int calculateNumberLength(unsigned int data, int base);
void shiftConsoleUp(struct CONSOLE *console);
void console_putLine(struct CONSOLE *console, uint32_t places);
void console_drawFull(struct CONSOLE *console);

#endif