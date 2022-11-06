#include "hex.h"

void hex_editor(uint8_t process){
    int index = 0;
    uint32_t ascii_index = 0;
    uint32_t key_index = 0;

    fb_clear(0);
    console_clear();
    memset(keyboard_KEYBUFFER, 0, sizeof(keyboard_KEYBUFFER));
    memset(keyboard_ASCIIBuffer, 0, sizeof(keyboard_ASCIIBuffer));
    keyboard_KEYBUFFER_index = 0;
    keyboard_ascii_index = 0;
    
    printk("\nHex Editor\n");
    if(!numFS){
        kill_process(process);
    }
    FS_read(0, 2, 1, (uint32_t *) filesystem_default_read_buffer);
    while(1){
        for(int y = 0; y < 32; y++){
            for(int x = 0; x < 16; x++){
                uint8_t partial_upper = (filesystem_default_read_buffer[y*16+x] & 0xF0) >> 8;
                uint8_t partial_lower = (filesystem_default_read_buffer[y*16+x] & 0x0F) >> 8;
                fb_setChar(3*x, y, quadToHex(partial_upper), 0xFFFFFF, 0);
                fb_setChar(3*x+1, y, quadToHex(partial_lower), 0xFFFFFF, 0);
            }
        }
        if(keyboard_ascii_index != ascii_index){
            ascii_index = keyboard_ascii_index;
            filesystem_default_read_buffer[index] = keyboard_ASCIIBuffer[ascii_index-1];
            index++;
        }
        if(keyboard_KEYBUFFER_index != key_index){
            key_index = keyboard_KEYBUFFER_index;
            switch(keyboard_KEYBUFFER[keyboard_KEYBUFFER_index - 1]){
                case 0x9C:
                    goto EXIT;
                    break;
                case 0x4B:
                    if(kbd_flags.arrow){
                        index--;
                    }
                    break;
                case 0x4D:
                    if(kbd_flags.arrow){
                        index++;
                    }
                    break;
                case 0x50:
                    if(kbd_flags.arrow){
                        index+=16;
                    }
                    break;
                case 0x48:
                    if(kbd_flags.arrow){
                        index-=16;
                    }
                    break;
            }
        }
    }
    EXIT:
    FS_write(0, 1, 1, (uint32_t *) filesystem_default_read_buffer);
    memset(keyboard_KEYBUFFER, 0, sizeof(keyboard_KEYBUFFER));
    memset(keyboard_ASCIIBuffer, 0, sizeof(keyboard_ASCIIBuffer));
    keyboard_KEYBUFFER_index = 0;
    keyboard_ascii_index = 0;
    //console_clear();
    fb_clear(0);
 
    kill_process(process);
}