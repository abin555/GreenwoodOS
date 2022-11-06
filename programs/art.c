#include "art.h"

void Art(uint8_t process){
    uint32_t key_index = 0;

    fb_clear(0);
    console_clear();
    memset(keyboard_KEYBUFFER, 0, sizeof(keyboard_KEYBUFFER));
    memset(keyboard_ASCIIBuffer, 0, sizeof(keyboard_ASCIIBuffer));
    keyboard_KEYBUFFER_index = 0;
    keyboard_ascii_index = 0;


    uint32_t x = fb_width/2;
    uint32_t y = fb_height/2;
    int dx = 1;
    int dy = -1;
    uint32_t color = 0;
    uint32_t old_color = 0;

    uint8_t pause = 0;
    while(1){
        if(!pause){
            fb_setPixel(x, y, old_color);
            old_color = color;
            color++;

            x += dx;
            y += dy;
            fb_setPixel(x, y, color);
            
            if(x <= 0){
                dx = -dx;
                x = 1;
            }
            if(x >= fb_width){
                dx = -dx;
                x = fb_width/2;
            }
            if(y <= 0){
                dy = -dy;
            }
            if(y >= fb_height){
                dy = -dy;
                y = fb_height - 1;
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
                case 0x39:
                    fb_clear(0);
                    break;
                case 0x1D:
                    pause = 1;
                break;
                case 0x9D:
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
    fb_clear(0);
 
    kill_process(process);
}