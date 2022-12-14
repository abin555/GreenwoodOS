#include "libc.h"

void print(char *str){
    register int syscall_num asm("eax") = 1;
    register char* arg       asm("ebx") = str;
    asm volatile("int 0x80");
}

void printval(char *str, int val){
    register int syscall_num asm("eax") = 3;
    register char* arg       asm("ebx") = str;
    register int edx asm("edx") = val;
    asm volatile("int 0x80");
}

FILE* fopen(char* filename){
    register int syscall_num asm("eax") = 0x11;
    register int drive_val asm("ebx") = 0;
    register char* filename_val asm("ecx") = filename;
    asm volatile("int 0x80");
    register FILE* return_val asm("eax");
    return return_val;
}

int fclose(FILE* file){
    register int syscall_num asm("eax") = 0x12;
    register FILE* file_val asm("ebx") = file;
    asm volatile("int 0x80");
    register int return_val asm("eax");
    return return_val;
}

unsigned char* fread(unsigned int sector){
    register int syscall_num asm("eax") = 0x13;
    register int drive asm("ebx") = 0;
    register int sector_val asm("ecx") = sector;
    asm volatile("int 0x80");
    register unsigned char *return_val asm("eax");
    return return_val;
}

void image_load_buffer(unsigned int width, unsigned int height, unsigned char* buffer){
    register int width_val asm("ebx") = width;
    register int height_val asm("ecx") = height;
    register unsigned char* buffer_val asm("edx") = buffer;
    register int syscall_num asm("eax") = 0x0B;
    asm volatile("int 0x80");
}

void draw_image(unsigned int x, unsigned int y){
    register int syscall_num asm("eax") = 0x0C;
    register unsigned int x_val asm("ebx") = x;
    register unsigned int y_val asm("ecx") = y;
    asm volatile("int 0x80");
}

unsigned char *malloc(unsigned int size){
    register unsigned int syscall_num asm("eax") = 0x06;
    register unsigned int size_val asm("ebx") = size;
    asm volatile("int 0x80");
    register unsigned char *buf asm("eax");
    return buf;
}

void free(unsigned char *buf){
    register unsigned int syscall_num asm("eax") = 0x07;
    register unsigned int buf_val asm("ebx") = (unsigned int) buf;
    asm volatile("int 0x80");
}

void toggle_console(){
    register unsigned int syscall asm("eax") = 0x0F;
    asm volatile("int 0x80");
}

int get_ascii_key_index(){
    register unsigned int syscall asm("eax") = 0x0A;
    asm volatile("int 0x80");
    register unsigned int index asm("ebx");
    return index;
}

char get_ascii_key(){
    register unsigned int syscall asm("eax") = 0x09;
    asm volatile("int 0x80");
    register char ascii asm("ebx");
    return ascii;
}

void clear_screen(){
    register unsigned int syscall asm("eax") = 0x10;
    asm volatile("int 0x80");
}

unsigned int get_timer_ticks(){
    register unsigned int syscall asm("eax") = 0x14;
    asm volatile("int 0x80");
    register unsigned int ticks asm("eax");
    return ticks;
}