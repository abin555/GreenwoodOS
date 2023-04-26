#ifndef LIBC_H
#define LIBC_H

typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char bool;

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
unsigned char* fread(unsigned int drive, unsigned int sector);
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

void toggle_mouse();
void mouse_draw();

struct mouse_state{
    uint32_t buttons;
    uint32_t x;
    uint32_t y;
};

struct mouse_state get_mouse_data();

extern int key_index_prev;

void toggle_window();

void *open_window(uint32_t width, uint32_t height, uint32_t *buf);
void close_window(void *window);

typedef enum{
    close = 1
} WINDOW_EVENT;

void add_window_event_handler(void *window, void *window_handler);
#endif