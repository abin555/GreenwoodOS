#ifndef INCLUDE_FRAME_BUFFER_H
#define INCLUDE_FRAME_BUFFER_H

#define FB_BLACK 0
#define FB_BLUE 1
#define FB_GREEN 2
#define FB_CYAN 3
#define FB_RED 4

#define FB_WHITE 15

char *fb;
int fb_cursor;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

void printChar(unsigned int x, unsigned int y, char c);

void fb_clear(char c, unsigned char fg, unsigned char bg);

int fb_write(char *buf, unsigned int len);
int fb_write_start(char *buf, unsigned int len, unsigned int start);
//int fb_print_buf(char *buf, unsigned int len, unsigned int start, unsigned int x, unsigned int y)

//CURSOR SETTINGS
//Define I/O Ports
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
//Define I/O Commands
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

void fb_move_cursor(unsigned short pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);


#endif
