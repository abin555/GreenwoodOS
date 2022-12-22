#ifndef LIBC_H
#define LIBC_H

typedef struct FILE{
    unsigned char drive;
    unsigned int sector;
    unsigned int sector_count;
    unsigned int size;
} FILE;

struct display{
    unsigned int width;
    unsigned int height;
};

FILE* fopen(char* filename);
int fclose(FILE* file);
unsigned char* fread(unsigned int sector);
void image_load_buffer(unsigned int width, unsigned int height, unsigned char* buffer);
void draw_image(unsigned int x, unsigned int y);
void print(char *str);
void printval(char *str, int val);
unsigned char *malloc(unsigned int size);
void free(unsigned char *buf);
void toggle_console();
int get_ascii_key_index();
char get_ascii_key();
char get_key();
void clear_screen(unsigned int color);
unsigned int get_timer_ticks();
void set_backbuffer(unsigned char state);
void swap_buffers();
struct display get_display();

int get_keycode_index();
char get_keycode();

char get_keycode_next();

void printChar(unsigned int x, unsigned int y, char c);
void setPrintColor(unsigned int color);
unsigned int *getFramebuffer();

extern int key_index_prev;
#endif