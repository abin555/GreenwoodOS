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
    register char* filename_val asm("ebx") = filename;
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

unsigned char* fread(unsigned int drive, unsigned int sector){
    register int syscall_num asm("eax") = 0x13;
    register int drive_val asm("ebx") = drive;
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

void clear_screen(unsigned int color){
    register unsigned int color_val asm("ebx") = color;
    register unsigned int syscall asm("eax") = 0x10;
    asm volatile("int 0x80");
}

struct display get_display(){
    struct display Display;
    register unsigned int eax asm("eax") = 0x08;
    asm volatile("int 0x80");
    register unsigned int width asm("ebx");
    register unsigned int height asm("ecx");
    Display.width = width;
    Display.height = height;
    return Display;
}

unsigned int get_timer_ticks(){
    register unsigned int syscall asm("eax") = 0x14;
    asm volatile("int 0x80");
    register unsigned int ticks asm("eax");
    return ticks;
}

void set_backbuffer(unsigned char state){
    register unsigned char ebx asm("ebx") = state;
    register unsigned int syscall asm("eax") = 0x15;
    asm volatile ("int 0x80");
}

void swap_buffers(){
    register unsigned int syscall asm ("eax") = 0x16;
    asm volatile ("int 0x80");
}
int key_index_prev;
char get_key(){
    int current_index = get_ascii_key_index();
    if(key_index_prev != current_index){
        key_index_prev = current_index;
        return get_ascii_key();
    }
    else{
        return 0;
    }
}

int get_keycode_index(){
    register unsigned int syscall asm("eax") = 0x19;
    asm volatile("int 0x80");
    register unsigned int index asm("ebx");
    return index;
}

char get_keycode(){
    register unsigned int syscall asm("eax") = 0x18;
    asm volatile("int 0x80");
    register char keycode asm("ebx");
    return keycode;
}
int keycode_index_prev;
char get_keycode_next(){
    int current_index = get_keycode_index();
    if(keycode_index_prev != current_index){
        keycode_index_prev = current_index;
        return get_keycode();
    }
    else{
        return 0;
    }
}

void printChar(unsigned int x, unsigned int y, char c){
    register unsigned int syscall asm("eax") = 0x04;
    register unsigned int x_reg asm("ebx") = x;
    register unsigned int y_reg asm("ecx") = y;
    register char c_reg asm("edx") = c;
    asm volatile ("int 0x80");
}

void setPrintColor(unsigned int color){
    register unsigned int syscall asm("eax") = 0x1A;
    register unsigned int color_val asm("ebx") = color;
    asm volatile ("int 0x80");
}

unsigned int *getFramebuffer(){
    register unsigned int syscall asm("eax") = 0x17;
    asm volatile ("int 0x80");
    register unsigned int *fb asm("eax");
    return fb;
}

void toggle_mouse(){
    register unsigned int syscall asm("eax") = 0x1B;
    asm volatile ("int 0x80");
}

void mouse_draw(){
    register unsigned int syscall asm("eax") = 0x1C;
    asm volatile ("int 0x80");
}

struct mouse_state get_mouse_data(){
    register uint32_t syscall asm("eax") = 0x1D;
    asm volatile ("int 0x80");
    register uint32_t buttons asm("ebx");
    register uint32_t X asm("ecx");
    register uint32_t Y asm("edx");
    struct mouse_state state = {
        buttons,
        X,
        Y
    };
    return state;
}

void toggle_window(bool enable){
    register uint32_t syscall asm("eax") = 0x1E;
    register uint32_t enable_reg asm("ebx") = enable;
    asm volatile ("int 0x80");
}