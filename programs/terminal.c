#include "terminal.h"

void init_terminal(){
    terminal_buffer = malloc(sizeof(char) * fb_terminal_w);
    terminal_last_key = keyboard_KEYBUFFER_index;
    terminal_last_char = keyboard_ascii_index;
    terminal_buffer_index = 0;
    //timer_attach(2, terminal_callback);
    add_process(terminal_callback);
    printk("[Terminal Program] Initialized\n");
}

void terminal_callback(uint8_t process __attribute__((unused))){
    char ascii;
    uint8_t keycode;
    if(keyboard_ascii_index != terminal_last_char){
        ascii = keyboard_ASCIIBuffer[keyboard_ascii_index-1];
        terminal_last_char = keyboard_ascii_index;

        
        //memcpy(terminal_buffer+terminal_buffer_index, terminal_buffer+terminal_buffer_index+1, fb_terminal_w-terminal_buffer_index-1);
        for(uint32_t index = fb_terminal_w-1; index > terminal_buffer_index-1; index--){
            terminal_buffer[index+1] = terminal_buffer[index];
        }
        terminal_buffer[terminal_buffer_index] = ascii;
        terminal_buffer_index++;
    }
    if(keyboard_KEYBUFFER_index != terminal_last_key){
        keycode = keyboard_KEYBUFFER[keyboard_KEYBUFFER_index-1];
        terminal_last_key = keyboard_KEYBUFFER_index;
        switch(keycode){
            case 0x0E:
                if(terminal_buffer_index > 0){
                    memcpy(terminal_buffer+terminal_buffer_index, terminal_buffer+terminal_buffer_index-1, fb_terminal_w-terminal_buffer_index);
                    terminal_buffer_index--;
                }
                break;
            case 0x4B:
                if(kbd_flags.arrow){
                    terminal_buffer_index--;
                }
                break;
            case 0x4D:
                if(kbd_flags.arrow){
                    terminal_buffer_index++;
                }
                break;
            case 0x9C:
                printk("> ");
                printk(terminal_buffer);
                printk("\n");
                if(terminal_buffer[0] == 'P'){
                    printk("Test Terminal Command\n");
                }
                if(terminal_buffer[0] == 'F'){
                    if(numFS){
                        FS_read(0, 0, 1, (uint32_t*) filesystem_default_read_buffer);
                        for(int y = 0; y < 1; y++){
                            for(int x = 0; x < 1; x++){
                                printk("%2x ", filesystem_default_read_buffer[y*16+x]);
                            }
                            printk("\n");
                        }
                        filesystem_default_read_buffer[0] += 1;
                        FS_write(0, 0, 1, (uint32_t*) filesystem_default_read_buffer);
                        FS_read(0, 0, 1, (uint32_t*) filesystem_default_read_buffer);
                        for(int y = 0; y < 1; y++){
                            for(int x = 0; x < 1; x++){
                                printk("%2x ", filesystem_default_read_buffer[y*16+x]);
                            }
                            printk("\n");
                        }
                    }
                }
                if(terminal_buffer[0] == 'C'){
                    fb_clear(0);
                    memset(consoleArray, 0, console_width*console_height);
                    consoleLine = 0;
                }
                if(terminal_buffer[0] == 'E'){
                    add_process(Editor);
                }
                if(terminal_buffer[0] == 'A'){
                    add_process(Art);       
                }
                if(terminal_buffer[0] == 'X'){
                    add_process(hex_editor);
                }
                if(terminal_buffer[0] == 'U'){
                    add_process(exec_user_program);
                }
                memset(terminal_buffer, 0, fb_terminal_w);
                terminal_buffer_index = 0;
                break;
        }
    }
    for(uint32_t i = 0; i < fb_terminal_w; i++){
        //fb_setChar(i, fb_terminal_h-1, terminal_buffer[i], 0xFFFFFF, 0);
        if(i == terminal_buffer_index){
            fb_setChar(i, fb_terminal_h-1, terminal_buffer[i], 0, 0xFFFFFF);
        }
        else{
            fb_setChar(i, fb_terminal_h-1, terminal_buffer[i], 0xFFFFFF, 0);
        }
    }
}