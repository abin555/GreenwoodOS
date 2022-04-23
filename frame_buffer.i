# 1 "frame_buffer.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "frame_buffer.c"
# 1 "./include/frame_buffer.h" 1
# 12 "./include/frame_buffer.h"
char *fb;
unsigned char *Buffer;
int fb_cursor;
unsigned char FG;
unsigned char BG;

void screen_init();

void fb_putPixel(int x, int y, unsigned int COLOR);

void fb_set_color(unsigned char fg, unsigned char bg);

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

void printChar(unsigned int x, unsigned int y, char c);

void fb_clear(char c, unsigned char fg, unsigned char bg);

int fb_write(char *buf, unsigned int len);
int fb_write_start(char *buf, unsigned int len, unsigned int start);
void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y);
# 42 "./include/frame_buffer.h"
void fb_move_cursor(unsigned short pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);
# 2 "frame_buffer.c" 2
# 1 "./include/io.h" 1



# 1 "./include/gcc_stdint.h" 1
# 34 "./include/gcc_stdint.h"
typedef signed char int8_t;


typedef short int int16_t;


typedef int int32_t;


typedef long long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef unsigned int uint32_t;


typedef long long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef long long unsigned int uint_least64_t;



typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long long unsigned int uint_fast64_t;




typedef int intptr_t;


typedef unsigned int uintptr_t;




typedef long long int intmax_t;
typedef long long unsigned int uintmax_t;
# 5 "./include/io.h" 2

extern void outb(unsigned short port, unsigned char data);

extern unsigned char inb(unsigned short pos);

void WriteMem(uint32_t Address, uint32_t Value);
uint32_t ReadMem(uint32_t Address);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char *INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
extern void PROGA();

extern unsigned int *externalProgram;
# 3 "frame_buffer.c" 2

char *fb = (char *)0x000B8000;
int fb_cursor = 0;
unsigned char FG = 15;
unsigned char BG = 0;
# 16 "frame_buffer.c"
void screen_init() {

    outb(0x3C6, 0xFF);
    outb(0x3C8, 0);
    for (unsigned char i = 0; i < 255; i++) {
        outb(0x3C9, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outb(0x3C9, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outb(0x3C9, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }


    outb(0x3C9, 0x3F);
    outb(0x3C9, 0x3F);
    outb(0x3C9, 0x3F);
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


    return 0;
}

int fb_write_start(char *buf, unsigned int len, unsigned int start){
    for (unsigned int index = 0; index < len; index++)
    {
        fb_write_cell(fb_cursor+index, buf[index+start], FG, BG);
    }


    return 0;
}

void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y){
    for(int index = 0; index < len; index++){


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
    outb(0x3D4, 14);
    outb(0x3D5, ((pos >> 8) & 0x00FF));
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0x00FF);
}
void fb_move_cursor_xy(unsigned int x, unsigned int y){
    fb_cursor = 80*y+x;
    outb(0x3D4, 14);
    outb(0x3D5, ((fb_cursor >> 8) & 0x00FF));
    outb(0x3D4, 15);
    outb(0x3D5, fb_cursor & 0x00FF);
}
