#ifndef SYS_WINDOW_H
#define SYS_WINDOW_H

#include <stdint.h>

struct WINDOW {
	uint32_t *backbuffer;
	char name[21];
	char active;
	uint32_t width;
	uint32_t height;
	char copyOnPromptOnly;
};	

struct WINDOW *window_open(char *name, uint32_t copyOnPrompt);
void window_close(struct WINDOW *window);
void window_update();

#endif