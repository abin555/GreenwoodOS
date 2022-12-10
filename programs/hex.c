#include "hex.h"

void hex_editor(uint8_t process, uint32_t args[10]){
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

    printk("\nHex Editor\n");
    if(!numFS){
        kill_process(process);
    }
    FS_read(drive, sector, 1, (uint32_t *) filesystem_default_read_buffer);
    fb_write_xy("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F", 48, 3, 0, 0x0, 0x00FF00);
    fb_write_xy("000", 3, 0, 1, 0x00FF00, 0);
    fb_write_xy("010", 3, 0, 2, 0x00FF00, 0);
    fb_write_xy("020", 3, 0, 3, 0x00FF00, 0);
    fb_write_xy("030", 3, 0, 4, 0x00FF00, 0);
    fb_write_xy("040", 3, 0, 5, 0x00FF00, 0);
    fb_write_xy("050", 3, 0, 6, 0x00FF00, 0);
    fb_write_xy("060", 3, 0, 7, 0x00FF00, 0);
    fb_write_xy("070", 3, 0, 8, 0x00FF00, 0);
    fb_write_xy("080", 3, 0, 9, 0x00FF00, 0);
    fb_write_xy("090", 3, 0, 10, 0x00FF00, 0);
    fb_write_xy("0A0", 3, 0, 11, 0x00FF00, 0);
    fb_write_xy("0B0", 3, 0, 12, 0x00FF00, 0);
    fb_write_xy("0C0", 3, 0, 13, 0x00FF00, 0);
    fb_write_xy("0D0", 3, 0, 14, 0x00FF00, 0);
    fb_write_xy("0E0", 3, 0, 15, 0x00FF00, 0);
    fb_write_xy("0F0", 3, 0, 16, 0x00FF00, 0);
    fb_write_xy("100", 3, 0, 17, 0x00FF00, 0);
    fb_write_xy("200", 3, 0, 18, 0x00FF00, 0);
    fb_write_xy("300", 3, 0, 19, 0x00FF00, 0);
    fb_write_xy("400", 3, 0, 20, 0x00FF00, 0);
    fb_write_xy("500", 3, 0, 21, 0x00FF00, 0);
    fb_write_xy("600", 3, 0, 22, 0x00FF00, 0);
    fb_write_xy("700", 3, 0, 23, 0x00FF00, 0);
    fb_write_xy("800", 3, 0, 24, 0x00FF00, 0);
    fb_write_xy("900", 3, 0, 25, 0x00FF00, 0);
    fb_write_xy("A00", 3, 0, 26, 0x00FF00, 0);
    fb_write_xy("B00", 3, 0, 27, 0x00FF00, 0);
    fb_write_xy("C00", 3, 0, 28, 0x00FF00, 0);
    fb_write_xy("D00", 3, 0, 29, 0x00FF00, 0);
    fb_write_xy("E00", 3, 0, 30, 0x00FF00, 0);
    fb_write_xy("F00", 3, 0, 31, 0x00FF00, 0);
    while(1){
        for(int y = 0; y < 32; y++){
            for(int x = 0; x < 16; x++){
                uint8_t partial_upper = (filesystem_default_read_buffer[16*y + x] & 0xF0);
                uint8_t partial_lower = (filesystem_default_read_buffer[16*y + x] & 0x0F);
                if(
                    (y*32 + 2*x) == index
                ){
                    fb_setChar(3*x+3, y+1, quadToHex(partial_upper), 0, 0xFFFFFF);
                }
                else{
                    fb_setChar(3*x+3, y+1, quadToHex(partial_upper), 0xFFFFFF, 0);
                }
                if(
                    (y*32 + 2*x) + 1 == index
                ){
                    fb_setChar(3*x+4, y+1, quadToHex(partial_lower), 0, 0xFFFFFF);
                }
                else{
                    fb_setChar(3*x+4, y+1, quadToHex(partial_lower), 0xFFFFFF, 0);
                }
            }
        }
        if(keyboard_ascii_index != ascii_index){
            ascii_index = keyboard_ascii_index;
            char symbol = keyboard_ASCIIBuffer[ascii_index-1];
            uint8_t adjustvalue = 0;
            switch(symbol){
                case '0':
                adjustvalue = 0;
                break;
                case '1':
                adjustvalue = 1;
                break;
                case '2':
                adjustvalue = 2;
                break;
                case '3':
                adjustvalue = 3;
                break;
                case '4':
                adjustvalue = 4;
                break;
                case '5':
                adjustvalue = 5;
                break;
                case '6':
                adjustvalue = 6;
                break;
                case '7':
                adjustvalue = 7;
                break;
                case '8':
                adjustvalue = 8;
                break;
                case '9':
                adjustvalue = 9;
                break;
                case 'a':
                case 'A':
                adjustvalue = 0xa;
                break;
                case 'b':
                case 'B':
                adjustvalue = 0xb;
                break;
                case 'c':
                case 'C':
                adjustvalue = 0xc;
                break;
                case 'd':
                case 'D':
                adjustvalue = 0xd;
                break;
                case 'e':
                case 'E':
                adjustvalue = 0xe;
                break;
                case 'f':
                case 'F':
                adjustvalue = 0xf;
                break;
            }
            if((symbol >= '0' && symbol <= '9') || (symbol >= 'a' && symbol <= 'f')){
                if(index % 2){
                    filesystem_default_read_buffer[index/2] = (filesystem_default_read_buffer[index/2] & 0xF0) | adjustvalue; 
                }
                else{
                    filesystem_default_read_buffer[index/2] = (filesystem_default_read_buffer[index/2] & 0x0f) | (adjustvalue << 4);
                }
            }
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
                        index+=32;
                    }
                    break;
                case 0x48:
                    if(kbd_flags.arrow){
                        index-=32;
                    }
                    break;
                default:
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
    
    fb_clear(0);
    console_initialized = 1;
    console_fullPut();
    printk("Hex Editor Close\n");
    kill_process(process);
}