#include "framebuffer.h"
#include "console.h"
#include "memory.h"
#include "stdint.h"

struct window{
    char title[16];
    uint32_t *buffer;
    char active;
};

#define WINDOW_ALLOC_WIDTH 1366
#define WINDOW_ALLOC_HEIGHT 768

#define MAX_WINDOW 6

struct window_buf{
    uint32_t buffer[WINDOW_ALLOC_WIDTH * WINDOW_ALLOC_HEIGHT];
};

struct window_buffers{
    struct window_buf buffer_list[MAX_WINDOW];
};

struct window window_list[MAX_WINDOW];
struct window_buffers *window_buffers;

int active_window;

void start_window_mgr();
struct window *create_window(char *title);
void close_window(struct window *window);
void window_callback();