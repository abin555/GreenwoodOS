#ifndef WINDOW_H
#define WINDOW_H

#include "serial.h"
#include "memory.h"
#include "framebuffer.h"
#include "timer.h"

struct WINDOW{
	uint32_t *backbuffer;
	char name[21];
	bool active;
	uint32_t width;
	uint32_t height;
	/*
	If true, backbuffer will only be copied to frontbuffer if:
	Window is currently focused.
	There is a function call to copy buffer.

	If false,
	backbuffer will be copied to front buffer on a timer if:
	Window is currently focused.
	OR
	If there is a function call to copy buffer
	*/
	bool copyOnPromptOnly;
};

#define MAX_WINDOWS 10
extern int window_selected;
extern struct WINDOW windows[MAX_WINDOWS];

void window_render_bar();
void window_init();
struct WINDOW *window_open(char *name, bool copyPromptOnly);
void window_close(struct WINDOW *window);
void window_copy_buffer(struct WINDOW *window);
#endif