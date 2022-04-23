#include "frame_buffer.h"
#include "io.h"

char *fb = (char *)0x000B8000;
int fb_cursor = 0;
unsigned char FG = FB_WHITE;
unsigned char BG = FB_BLACK;

#define FB_WIDTH 1048

#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

void screen_init() {
    // configure palette with 8-bit RRRGGGBB color
    outb(PALETTE_MASK, 0xFF);
    outb(PALETTE_WRITE, 0);
    for (unsigned char i = 0; i < 255; i++) {
        outb(PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outb(PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outb(PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }

    // set color 255 = white
    outb(PALETTE_DATA, 0x3F);
    outb(PALETTE_DATA, 0x3F);
    outb(PALETTE_DATA, 0x3F);
}

void fb_putPixel(int x, int y, unsigned int COLOR){
    unsigned char* location = (unsigned char*)0xA0000 + x + y +3;
    *location = COLOR;
}

void fb_set_color(unsigned char fg, unsigned char bg){
    FG = fg;
    BG = bg;
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[2 * i] = c;
    fb[2 * i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void printChar(unsigned int x, unsigned int y, char c)
{
    int index = y * 80 + x;
    fb_write_cell(index, c, FG, BG);
}

void fb_clear(char c, unsigned char fg, unsigned char bg)
{
    for (int i = 0; i < 80 * 24; i++)
    {
        fb_write_cell(i, c, fg, bg);
    }
}

int fb_write(char *buf, unsigned int len)
{
    for (unsigned int index = 0; index < len; index++)
    {
        fb_write_cell(fb_cursor+index, buf[index], FG, BG);
    }
    //fb_cursor+=len;
    //fb_move_cursor(fb_cursor);
    return 0;
}

int fb_write_start(char *buf, unsigned int len, unsigned int start){
    for (unsigned int index = 0; index < len; index++)
    {
        fb_write_cell(fb_cursor+index, buf[index+start], FG, BG);
    }
    //fb_cursor+=len;
    //fb_move_cursor(fb_cursor);
    return 0;
}

void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y){
    for(int index = 0; index < len; index++){
        //fb_write_cell((y*80)+x, Buffer[index], FB_WHITE, FB_BLACK);
        //printChar(x+index,y, Buffer[index+start]);
        fb_write_cell((y*80)+x+index, Buffer[index+start], FG, BG);
    }
}

int fb_print_buf(char *buf, unsigned int len, unsigned int start, unsigned int x, unsigned int y){
    unsigned int real_index = start;
    for(unsigned int index = 0; index < len; index++){
        real_index++;
        if(real_index > len){
            real_index = 0;
        }
        fb_write_cell((y*80)+x, buf[real_index], FG, BG);
    }
    return 0;
}

void fb_move_cursor(unsigned short pos)
{
    fb_cursor = pos;
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}
void fb_move_cursor_xy(unsigned int x, unsigned int y){
    fb_cursor = 80*y+x;
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((fb_cursor >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, fb_cursor & 0x00FF);
}