#ifndef SYS_CONSOLE_H
#define SYS_CONSOLE_H

#include <stdint.h>

struct CONSOLE{
	void *window;
	uint32_t width;
	uint32_t height;
	uint32_t buf_size;
	char *buf;
	char active;
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

struct CONSOLE *console_open();

void console_close();

#ifndef SYS_VP_H
struct Viewport;
#endif

struct CONSOLE *console_open_vp(struct Viewport *vp);
void console_print(struct CONSOLE *console, char *str);
struct CONSOLE *console_get_current();

#endif