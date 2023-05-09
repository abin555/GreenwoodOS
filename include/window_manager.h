#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "framebuffer.h"
#include "mouse.h"
#include "keyboard.h"
#include "processes.h"
#include "filesystem.h"
#include "memory.h"
#include "console.h"
#include "multitasking.h"

struct key_event{
    char ascii;
    uint8_t scancode;
};

struct mouse_event{
    uint8_t buttons;
    int x;
    int y;
};

typedef enum{
    close = 1
} WINDOW_EVENT;

struct window{
    uint32_t screen_x;
    uint32_t screen_y;
    uint32_t width;
    uint32_t height;
    uint32_t *window_buf;
    struct process_task *parent_process;
    void (*keyboard_event_handler)(struct key_event);
    void (*mouse_event_handler)(struct mouse_event);
    void (*window_event_handler)(WINDOW_EVENT window_event);
};

uint8_t num_windows;
struct window *windows[10];
bool window_update = false;
uint32_t max_windows;
extern bool use_window;
extern bool window_init;

void init_window_manager(uint32_t max);
struct window *create_window(uint32_t width, uint32_t height, uint32_t *buf);
void close_window(struct window* window);
void draw_screen();
void redraw_window(struct window* window);
void request_redraw();

void add_window_event(struct window *window, void (*window_event_handler)(WINDOW_EVENT window_event));
#endif