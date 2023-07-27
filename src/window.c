#include "window.h"

char *window_render_title_buf = NULL;

void start_window_mgr(){
    active_window = -1;
    window_render_title_buf = malloc(fb_terminal_w);
    memset(window_render_title_buf, ' ', fb_terminal_w);
    memset(window_list, 0, sizeof(window_list));
    window_buffers = (struct window_buffers *) 0x30000000;
    printk("Window Buffer Memory: 0x%x loading to 0x%x\n", sizeof(struct window_buffers), (uint32_t) window_buffers);

    for(unsigned int i = 0; i < (sizeof(struct window_buffers) / 0x400000) + 1; i++){
        create_page_entry((uint32_t) (((uint32_t)window_buffers)+(i*0x400000)), (uint32_t) (((uint32_t)window_buffers)+(i*0x400000)), 0x93);
    }

    memset(window_buffers, 0xFFAAFF, sizeof(struct window_buffers));
    printk("%x\n", *((uint32_t *) 0x30000004));
    //timer_attach(10, window_callback);
}

struct window *create_window(char *title){
    int window_idx = -1;
    for(int i = 0; i < MAX_WINDOW; i++){
        if(window_list[i].active == false){
            window_idx = i;
            break;
        }
    }
    if(window_idx == -1){
        return NULL;
    }
    struct window *new_window = &window_list[window_idx];
    memcpy(title, &new_window->title, 16);
    new_window->active = true;
    new_window->buffer = window_buffers->buffer_list[window_idx].buffer;
    active_window = window_idx;
    return new_window;
}

void window_callback(){
    if(active_window < 0) return;
    memcpy(window_list[active_window].buffer, framebuffer, (WINDOW_ALLOC_WIDTH * WINDOW_ALLOC_HEIGHT - (8 * WINDOW_ALLOC_WIDTH)) * sizeof(uint32_t));
    fb_write_xy(window_render_title_buf, fb_terminal_w, 0, fb_terminal_h-1, 0, 0xb16b00b);
}