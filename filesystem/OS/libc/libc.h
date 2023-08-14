#ifndef LIBC_H
#define LIBC_H

typedef unsigned int uint32_t;
typedef char bool;

struct WINDOW {
	uint32_t *backbuffer;
	char name[21];
	bool active;
	uint32_t width;
	uint32_t height;
	bool copyOnPromptOnly;
};	

struct WINDOW *window_open(char *name);
void window_close(struct WINDOW *window);
void window_update();
void drawChar(uint32_t x, uint32_t y, char c);
char getc();
void exec(char *filename, int argc, char **argv);
void set_schedule(int type);
void print(char *str);

#endif