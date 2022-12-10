#include "editor.h"

void Editor(uint8_t process, uint32_t args[10]__attribute__((unused))){
    uint32_t drive = args[0];
    uint32_t sector = args[1];

    int index = 0;
    uint32_t ascii_index = 0;
    uint32_t key_index = 0;
    console_initialized = 0;
    fb_clear(0);
    //console_clear();
    memset(keyboard_KEYBUFFER, 0, sizeof(keyboard_KEYBUFFER));
    memset(keyboard_ASCIIBuffer, 0, sizeof(keyboard_ASCIIBuffer));
    keyboard_KEYBUFFER_index = 0;
    keyboard_ascii_index = 0;
    
    printk("Editor\n");
    if(!numFS){
        kill_process(process);
    }
    FS_read(drive, sector, 1, (uint32_t *) filesystem_default_read_buffer);
    while(1){
        for(int y = 0; y < 32; y++){
            for(int x = 0; x < 16; x++){
                if(index == y*16+x){ 
                    fb_setChar(x, y+1, filesystem_default_read_buffer[y*16+x], 0, 0xFFFFFF);
                }
                else{
                    fb_setChar(x, y+1, filesystem_default_read_buffer[y*16+x], 0xFFFFFF, 0);
                }
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
    FS_write(drive, sector, 1, (uint32_t *) filesystem_default_read_buffer);
    memset(keyboard_KEYBUFFER, 0, sizeof(keyboard_KEYBUFFER));
    memset(keyboard_ASCIIBuffer, 0, sizeof(keyboard_ASCIIBuffer));
    keyboard_KEYBUFFER_index = 0;
    keyboard_ascii_index = 0;
    //console_clear();
    fb_clear(0);
    console_initialized = 1;
    console_fullPut();
    printk("Text Editor Ended\n");

    kill_process(process);
}