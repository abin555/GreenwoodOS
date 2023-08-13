#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdarg.h>
#include "allocator.h"
#include "serial.h"
#include "window.h"
#include "framebuffer.h"

#define MAX_CONSOLE 10

struct CONSOLE{
	struct WINDOW *window;
	uint32_t width;
	uint32_t height;
	uint32_t buf_size;
	char *buf;
	bool active;
	int last_cursor;
	int cursor;
};

extern struct CONSOLE consoles[MAX_CONSOLE];

void console_init();

struct CONSOLE *console_open(struct WINDOW *window);
void console_close(struct CONSOLE *console);

void print_console(struct CONSOLE *console, char *msg, ...);

#endif