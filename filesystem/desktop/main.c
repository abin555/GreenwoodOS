#include "../libc/libc.h"
uint8_t running = 1;
char key;
uint8_t keycode;
uint8_t was_clicked = 0;
struct display screen;
uint32_t *screen_buf;

struct drag_rect{
    uint32_t startx;
    uint32_t starty;
};

struct drag_rect drag;
struct mouse_state mouse_state;

void draw_rect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);

int main(){
    toggle_mouse();
    get_keycode_next();
    toggle_console();
    toggle_window();
    set_backbuffer(1);
    clear_screen(0);
    swap_buffers();
    screen_buf = getFramebuffer();
    screen = get_display();
    setPrintColor(0xFFFFFF);

    while(running){
        clear_screen(0x4ACCDB);
        mouse_state = get_mouse_data();
        keycode = get_keycode_next();
        switch(keycode){
            case 0x81://Escape
                running = 0;
                break;
        }
        
        if(!was_clicked && (mouse_state.buttons & 0b100) != 0){
            was_clicked = 1;
            drag.startx = mouse_state.x;
            drag.starty = mouse_state.y;
        }
        else if((mouse_state.buttons & 0b100) == 0){
            was_clicked = 0;
        }
        else if(was_clicked && (mouse_state.buttons & 0b100)  != 0){
            draw_rect(drag.startx, drag.starty, mouse_state.x, mouse_state.y);
            printChar(0,0,'D');
        }
        mouse_draw();
        swap_buffers();
    }
    clear_screen(0);
    swap_buffers();

    setPrintColor(0);
    set_backbuffer(0);
    toggle_window();
    toggle_console();
    toggle_mouse();
}

void draw_rect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2){
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

    for(int x = x1; x < x2; x++){
        screen_buf[x + (y1 * screen.width)] = 0xFF0000;
    }
    for(int x = x1; x < x2; x++){
        screen_buf[x + (y2 * screen.width)] = 0xFF0000;
    }
    for(int y = y1; y < y2; y++){
        screen_buf[x1 + (y * screen.width)] = 0xFF0000;
    }
    for(int y = y1; y < y2; y++){
        screen_buf[x2 + (y * screen.width)] = 0xFF0000;
    }
}