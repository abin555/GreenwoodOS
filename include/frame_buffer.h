#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "types.h"
#include "multiboot.h"

#define FB_BLACK 0
#define FB_BLUE 0x0000FF
#define FB_GREEN 0x00FF00
#define FB_CYAN 0x00FFFF
#define FB_RED 0xFF0000

#define FB_WHITE 0xFFFFFF

u32 fb_width;
u32 fb_height;
u64* fb;
int fb_terminal_w;
int fb_terminal_h;

unsigned int FG;
unsigned int BG;

void fb_setPixel(u32 x, u32 y, u32 color);

void init_fb(struct multiboot_tag_framebuffer *tagfb);

void fb_write_cell(u32 index, char c, u32 fb, u32 bg);

void printChar(unsigned int x, unsigned int y, char c);

void fb_set_color(unsigned int fg, unsigned int bg);

void fb_clear(unsigned int color);

int fb_write(char *buf, unsigned int len);
int fb_write_start(char *buf, unsigned int len, unsigned int start);
void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y);

void fb_move_cursor(unsigned int pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);


#endif