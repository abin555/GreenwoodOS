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

void draw_rect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color){
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
        framebuffer[x + (y1 * fb_width)] = color;
    }
    for(uint32_t x = x1; x < x2; x++){
        framebuffer[x + (y2 * fb_width)] = color;
    }
    for(uint32_t y = y1; y < y2; y++){
        framebuffer[x1 + (y * fb_width)] = color;
    }
    for(uint32_t y = y1; y < y2; y++){
        framebuffer[x2 + (y * fb_width)] = color;
    }
}

void fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color){    
    for(uint32_t Y = 0; Y < h; Y++){
        for(uint32_t X = 0; X < w; X++){
            framebuffer[X + x +  ((y + Y) * fb_width)] = color;
        }
    }
}

void reorder_windows(int window_idx){
    struct window* original_idx = windows[window_idx];
    for(int i = window_idx; i > 0; i--){
        windows[i] = windows[i-1];
    }
    windows[0] = original_idx;
}

void window_event_handler(struct window* window){
    if(
        (mouse_x > (int) window->screen_x+2 && mouse_x < (int) window->screen_x+10) &&
        (mouse_y > (int) window->screen_y-12 && mouse_y < (int) window->screen_y-4) && 
        mouse_left_click &&
        window->window_event_handler != 0
    ){
        window->window_event_handler(close);
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
        for(int i = 0; i < (int) max_windows; i++){     
            if(windows[i] != 0){
                if(
                    (mouse_x > (int) windows[i]->screen_x && mouse_x < (int) (windows[i]->screen_x + windows[i]->width) &&
                    mouse_y > ((int) windows[i]->screen_y - 20)&& mouse_y < (int) (windows[i]->screen_y)) || (offset_x != -1 && select == (int) i)
                ){
                    window_event_handler(windows[i]);
                    if(i != 0){
                        reorder_windows(i);
                        break;
                    }
                    if(offset_x == -1 && offset_y == -1){
                        offset_x = windows[i]->screen_x - mouse_x;
                        offset_y = windows[i]->screen_y - 20 - mouse_y;
                        select = i;
                        window_update = true;
                    }
                    windows[i]->screen_x=mouse_x+offset_x;
                    windows[i]->screen_y=mouse_y+offset_y+20;
                    if((int) windows[i]->screen_x < 0) windows[i]->screen_x = 1;
                    if((int) windows[i]->screen_y - 20 < 0) windows[i]->screen_y = 21;
                    if(windows[i]->screen_x > fb_width-windows[i]->width) windows[i]->screen_x = fb_width-windows[i]->width-1;
                    if(windows[i]->screen_y > fb_height-windows[i]->height) windows[i]->screen_y = fb_height-windows[i]->height-1;
                    break;
                }
            }
        }
    }
}

void render_bar(int window_idx){
    fill_rect(
        windows[window_idx]->screen_x,
        windows[window_idx]->screen_y-20,
        windows[window_idx]->width,
        20,
        (window_idx == 0) ? 0xF0F0F0 : 0x808080
    );
    draw_rect(
        windows[window_idx]->screen_x,
        windows[window_idx]->screen_y-20,
        windows[window_idx]->screen_x + windows[window_idx]->width,
        windows[window_idx]->screen_y,
        (window_idx == 0) ? 0x00FF00 : 0xFF0000
    );
    if(windows[window_idx]->window_event_handler) fb_drawChar(windows[window_idx]->screen_x+2, windows[window_idx]->screen_y-12, 'X', 0xFFFFFF, 0x808080);
}

void draw_screen(){
    if(use_window == false){
        return;
    };
    set_backbuffer(1);
    interaction();
    
    //fb_clear(0x4ACCDB);
    fb_clear(0x383838);
    for(int i = max_windows-1; i >= 0; i--){
        if(windows[i] != 0){
            //fill_rect(windows[i]->screen_x, windows[i]->screen_y-10, windows[i]->width, 10);
            render_bar(i);
            redraw_window(windows[i]);
        }
    }

    mouse_draw();
    swap_buffers();
}

void add_window(struct window *window){
    for(int i = 0; i < (int) max_windows; i++){
        if(windows[i] == 0){
            windows[i] = window;
            num_windows++;
            reorder_windows(i);
            break;
        }
    }
}

void remove_window(struct window *window){
    for(int i = 0; i < (int) max_windows; i++){
        if(window == windows[i]){
            windows[i] = 0;
            num_windows--;
            break;
        }
    }
}

struct window *create_window(uint32_t width, uint32_t height, uint32_t *buf){
    struct window* window = (struct window*) malloc(sizeof(struct window));
    window->screen_x = 50;
    window->screen_y = 50;
    window->width = width;
    window->height = height;
    window->window_buf = buf;
    window->window_event_handler = 0;
    add_window(window);
    return window;
}

void close_window(struct window* window){
    remove_window(window);
    //for(int i = 0; i < 0xFFFFFF; i++){}
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

void add_window_event(struct window *window, void (*window_event_handler)(WINDOW_EVENT window_event)){
    window->window_event_handler = window_event_handler;
    printk("Added Window Event Handler\n");
}