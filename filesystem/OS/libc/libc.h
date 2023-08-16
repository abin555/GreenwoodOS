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

struct CONSOLE{
	struct WINDOW *window;
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

struct WINDOW *window_open(char *name);
void window_close(struct WINDOW *window);
void window_update();
void drawChar(uint32_t x, uint32_t y, char c);
char getc();
void exec(char *filename, int argc, char **argv);
void set_schedule(int type);
void print(char *str);
void print_arg(char *str, uint32_t arg);
struct CONSOLE *console_open();
void console_close();
void *kmalloc(uint32_t size);

#endif