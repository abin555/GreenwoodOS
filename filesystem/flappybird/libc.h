#ifndef LIBC_H
#define LIBC_H

typedef struct FILE{
    unsigned char drive;
    unsigned int sector;
    unsigned int sector_count;
    unsigned int size;
} FILE;

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
void clear_screen();
unsigned int get_timer_ticks();

#endif