#include "art.h"
#define window_size 240

uint32_t art_window_buf[window_size*window_size*sizeof(uint32_t)];

void Art(uint8_t process, uint32_t args[10]__attribute__((unused))){
    struct window* art_window = create_window(window_size,window_size, art_window_buf);
    struct window* art_window2 = create_window(window_size,window_size, art_window_buf);
    uint32_t key_index = 0;

    fb_clear(0);
    console_clear();
    memset(keyboard_KEYBUFFER, 0, sizeof(keyboard_KEYBUFFER));
    memset(keyboard_ASCIIBuffer, 0, sizeof(keyboard_ASCIIBuffer));
    keyboard_KEYBUFFER_index = 0;
    keyboard_ascii_index = 0;


    uint32_t x = window_size/2;
    uint32_t y = window_size/2;
    int dx = 1;
    int dy = -1;
    uint32_t color = 0;
    uint32_t old_color = 0;

    uint8_t pause = 0;
    while(1){
        if(!pause){
            //fb_setPixel(x, y, old_color);
            art_window->window_buf[x + y*window_size] = old_color; 
            old_color = color;
            color++;

            x += dx;
            y += dy;
            //fb_setPixel(x, y, color);
            art_window->window_buf[x + y*window_size] = color;
            
            if(x <= 0){
                dx = -dx;
                x = 1;
            }
            if(x >= window_size){
                dx = -dx;
                x = window_size - 1;
            }
            if(y <= 0){
                dy = -dy;
            }
            if(y >= window_size){
                dy = -dy;
                y = window_size - 1;
            }
        }

        if(keyboard_KEYBUFFER_index != key_index){
            key_index = keyboard_KEYBUFFER_index;
            switch(keyboard_KEYBUFFER[keyboard_KEYBUFFER_index - 1]){
                case 0x9C:
                    goto EXIT;
                    break;
                case 0x4B:
                    if(kbd_flags.arrow){
                        dx++;
                    }
                    break;
                case 0x4D:
                    if(kbd_flags.arrow){
                        dx--;
                    }
                    break;
                case 0x50:
                    if(kbd_flags.arrow){
                        dy--;
                    }
                    break;
                case 0x48:
                    if(kbd_flags.arrow){
                        dy++;
                    }
                    break;
                case 0x39://Space Bar
                    //fb_clear(0);
                    memset(art_window_buf, 0, sizeof(art_window_buf));
                    break;
                case 0x1D://Left Control
                    pause = 1;
                break;
                case 0x9D://Right Control
                    pause = 0;
            }
        }
    }
    EXIT:
    memset(keyboard_KEYBUFFER, 0, sizeof(keyboard_KEYBUFFER));
    memset(keyboard_ASCIIBuffer, 0, sizeof(keyboard_ASCIIBuffer));
    keyboard_KEYBUFFER_index = 0;
    keyboard_ascii_index = 0;
    //console_clear();
    //fb_clear(0);
    close_window(art_window);
    close_window(art_window2);
    kill_process(process);
}