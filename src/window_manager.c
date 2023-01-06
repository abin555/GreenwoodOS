#include "window_manager.h"

bool use_window = false;
bool window_init = false;

void init_window_manager(uint32_t max){
    printk("[WINDOW MGR] Init\n");
    //windows = (struct window **) malloc(sizeof(struct window *) * max_windows);
    memset(windows, 0, sizeof(windows));
    max_windows = max;
    mouse_enable = false;
    use_window = true;
    window_init = true;
    printk("[WINDOW MGR] Done Init\n");
}

void draw_rect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2){
    if(!(x1 > 0 && x1 < fb_width && x2 > 0 && x2 < fb_width && y1 > 0 && y1 < fb_height && y2 > 0 && y2 < fb_height)) return;
    if(x1 > x2){
        uint32_t temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 > y2){
        uint32_t temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for(uint32_t x = x1; x < x2; x++){
        framebuffer[x + (y1 * fb_width)] = 0xFF0000;
    }
    for(uint32_t x = x1; x < x2; x++){
        framebuffer[x + (y2 * fb_width)] = 0xFF0000;
    }
    for(uint32_t y = y1; y < y2; y++){
        framebuffer[x1 + (y * fb_width)] = 0xFF0000;
    }
    for(uint32_t y = y1; y < y2; y++){
        framebuffer[x2 + (y * fb_width)] = 0xFF0000;
    }
}

void fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h){
    for(uint32_t X = x; X < w; X++){
        for(uint32_t Y = y; Y < h; Y++){
            framebuffer[X + (Y * fb_width)] = 0x0F0FFF;
        }
    }
}

bool was_clicked = 0;
int offset_x = 0;
int offset_y = 0;
int select = -1;
void interaction(){
    if(!was_clicked && mouse_left_click){
        was_clicked = 1;
        offset_x = -1;
        offset_y = -1;
        select = -1;
    }
    else if(!mouse_left_click){
        was_clicked = 0;
    }
    else if(mouse_left_click && was_clicked){
        fb_setChar(0,0,'L',0xFFFFFF,0);
        for(uint32_t i = 0; i < max_windows; i++){     
            if(windows[i] != 0){
                if(
                    (mouse_x > (int) windows[i]->screen_x && mouse_x < (int) (windows[i]->screen_x + windows[i]->width) &&
                    mouse_y > ((int) windows[i]->screen_y - 20)&& mouse_y < (int) (windows[i]->screen_y)) || (offset_x != -1 && select == (int) i)
                ){
                    if(offset_x == -1 && offset_y == -1){
                        offset_x = windows[i]->screen_x - mouse_x;
                        offset_y = windows[i]->screen_y - 20 - mouse_y;
                        select = i;
                        window_update = true;
                    }
                    fb_setChar(0,1,'F',0xFFFFFF,0);
                    windows[i]->screen_x=mouse_x+offset_x;
                    windows[i]->screen_y=mouse_y+offset_y+20;
                    break;
                }
            }
        }
    }
}

void draw_screen(){
    if(use_window == false) return;
    //return;
    set_backbuffer(1);
    interaction();
    
    fb_clear(0x4ACCDB);
    for(uint32_t i = 0; i < max_windows; i++){
        if(windows[i] != 0){
            //fill_rect(windows[i]->screen_x, windows[i]->screen_y-10, windows[i]->width, 10);
            draw_rect(
                windows[i]->screen_x,
                windows[i]->screen_y-20,
                windows[i]->screen_x + windows[i]->width,
                windows[i]->screen_y
            );
            redraw_window(windows[i]);
        }
    }

    mouse_draw();
    swap_buffers();
}

void add_window(struct window *window){
    for(uint32_t i = 0; i < max_windows; i++){
        if(windows[i] == 0){
            windows[i] = window;
            break;
        }
    }
}

void remove_window(struct window *window){
    for(uint32_t i = 0; i < max_windows; i++){
        if(window == windows[i]){
            windows[i] = 0;
            break;
        }
    }
}

struct window *create_window(uint32_t width, uint32_t height){
    struct window* window = (struct window*) malloc(sizeof(struct window));
    window->screen_x = 50;
    window->screen_y = 50;
    window->width = width;
    window->height = height;
    window->window_buf = malloc(sizeof(uint32_t) * width * height);
    add_window(window);
    return window;
}

void close_window(struct window* window){
    remove_window(window);
    free(window->window_buf);
    free(window);
}

void redraw_window(struct window* window){
    for(uint32_t y = 0; y < window->height && y < fb_height - window->screen_y; y++){
        for(uint32_t x = 0; x < window->width; x++){
            framebuffer[
                window->screen_x + x + (window->screen_y*fb_width + y*fb_width)
            ] = window->window_buf[
                x + y*window->width
            ];
        }
    }    
}

void request_redraw(){
    window_update = true;
}