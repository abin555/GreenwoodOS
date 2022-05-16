#include "frame_buffer.h"
#include "font.h"

#define CHAR_W 8
#define CHAR_H 8
int fb_cursor = 0;

void fb_setPixel(u32 x, u32 y, u32 color){
    if (fb_width <= x || fb_height <= y)
        return;
        
    fb[ fb_width * y + x] = color;
}

void init_fb(struct multiboot_tag_framebuffer *tagfb){
    fb = (u64 *) (unsigned long) tagfb->common.framebuffer_addr;
    fb_height = tagfb->common.framebuffer_height;
    fb_width = tagfb->common.framebuffer_width;
    fb_terminal_w = fb_width / CHAR_W;
    fb_terminal_h = fb_height / CHAR_H;
}

void fb_write_cell(u32 index, char c, u32 fg, u32 bg){
    u32 x = (index % fb_terminal_w)*CHAR_W;
    u32 y = (index / fb_terminal_w)*CHAR_H;
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            fb[fb_width * (y+layer) + x+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}
void printChar(unsigned int x, unsigned int y, char c){
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            fb[fb_width * ((y*CHAR_H)+layer) + (x*CHAR_W)+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? FG : BG;
        }
    }
}

void printChar_Scaled(unsigned int x, unsigned int y, char c, int scale){
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            //fb[fb_width * ((y*CHAR_H)+layer) + (x*CHAR_W)+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? FG : BG;
            pixelScaled((x+pixel+scale)*scale,(y+layer+scale)*scale,scale,((FONT[(int)c][layer] >> pixel) & 1) ? FG : BG);
        }
    }
}

void pixelScaled(unsigned int x, unsigned int y, int scale, u32 color){
    for(int scalex = 0; scalex<scale; scalex++){
        for(int scaley = 0; scaley<scale; scaley++){
            fb[fb_width*(y+scaley)+x+scalex] = color;
        }
    }
}

void fb_set_color(unsigned int fg, unsigned int bg){
    FG = fg;
    BG = bg;
}

void fb_clear(unsigned int color){
    for(u32 i = 0; i < fb_width*fb_height; i++){
        fb[i] = color;
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
        fb_write_cell((y*fb_terminal_w)+x+index, Buffer[index+start], FG, BG);
    }
}

void fb_write_xy_scaled(char *Buffer, int len, int start, unsigned int x, unsigned int y, unsigned int scale){
    for(int index = 0; index < len; index++){
        printChar_Scaled(x+index*(4*scale), y, Buffer[index+start], scale);
    }
}

void fb_move_cursor(unsigned int pos){
    fb_cursor = pos;
}
void fb_move_cursor_xy(unsigned int x, unsigned int y){
    fb_cursor = fb_terminal_w*y+x;
}

void fb_copyBuffer(){
    //memcpy((u64 *) &fb_backBuffer, (u64 *) &fb, (u64) fb_width*fb_height);
    for(u32 index = 0; index<fb_width*fb_height; index++){
        fb[index] = fb_backBuffer[index];
    }
}

void fb_clearBackBuffer(u32 color){
    for(u32 index = 0; index<fb_width*fb_height; index++){
        fb_backBuffer[index] = color;
    }
}