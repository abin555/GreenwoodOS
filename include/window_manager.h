#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "framebuffer.h"
#include "mouse.h"
#include "keyboard.h"
#include "processes.h"
#include "filesystem.h"
#include "memory.h"
#include "console.h"

struct window{
    uint32_t screen_x;
    uint32_t screen_y;
    uint32_t width;
    uint32_t height;
    uint32_t *window_buf;
    struct process_task *parent_process;
};

struct window *windows[10];
bool window_update = false;
uint32_t max_windows;
extern bool use_window;
extern bool window_init;

void init_window_manager(uint32_t max);
struct window *create_window(uint32_t width, uint32_t height);
void close_window(struct window* window);
void draw_screen();
void redraw_window(struct window* window);
void request_redraw();
#endif