#include "terminal.h"

int strcmp(const char *X, const char *Y, int block){
    int start = 0;
    int end = 0;
    if(block != 0){
        start = terminal_block_index[block-1];
    }
    end = terminal_block_index[block];
    X+=start;
    uint32_t init_scan = (uint32_t) X;
    while(*X != 0 && (uint32_t) X - init_scan < (uint32_t) end){
        if(*X != *Y){
            break;
        }
        X++;
        Y++;
    }
    int dif = *(const unsigned char*)X - *(const unsigned char*)Y;
    //printk("S: %x E: %x B: %x Diff %x\n", start, end, block, dif);
    if(dif == 0x20){
        dif = 0;
    }
    return dif;
}

uint32_t atoi(int block){
    int start = 0;
    int end = 0;
    if(block != 0){
        start = terminal_block_index[block-1];
    }
    end = terminal_block_index[block];
    uint32_t value = 0;
    while(end  > start){
        switch(terminal_buffer[end]){
            case '0':
                value <<= 4;
                value |= 0;
            break;
            case '1':
                value <<= 4;
                value |= 0x1;
            break;
            case '2':
                value <<= 4;
                value |= 0x2;
            break;
            case '3':
                value <<= 4;
                value |= 0x3;
            break;
            case '4':
                value <<= 4;
                value |= 0x4;
            break;
            case '5':
                value <<= 4;
                value |= 0x5;
            break;
            case '6':
                value <<= 4;
                value |= 0x6;
            break;
            case '7':
                value <<= 4;
                value |= 0x7;
            break;
            case '8':
                value <<= 4;
                value |= 0x8;
            break;
            case '9':
                value <<= 4;
                value |= 0x9;
            break;
            case 'a':
            case 'A':
                value <<= 4;
                value |= 0xa;
            break;
            case 'b':
            case 'B':
                value <<= 4;
                value |= 0xb;
            break;
            case 'c':
            case 'C':
                value <<= 4;
                value |= 0xc;
            break;
            case 'd':
            case 'D':
                value <<= 4;
                value |= 0xd;
            break;
            case 'e':
            case 'E':
                value <<= 4;
                value |= 0xe;
            break;
            case 'f':
            case 'F':
                value <<= 4;
                value |= 0xf;
            break;
        }
        end--;
    }
    return value;
}

void init_terminal(){
    terminal_buffer = malloc(sizeof(char) * fb_terminal_w);
    terminal_last_key = keyboard_KEYBUFFER_index;
    terminal_last_char = keyboard_ascii_index;
    terminal_buffer_index = 0;

    memset(terminal_block_index, 0, sizeof(terminal_block_index));

    //timer_attach(2, terminal_callback);
    add_process(terminal_callback, 0);
    //printk("[Terminal Program] Initialized\n");
}

void terminal_locate_blocks(){
    terminal_number_of_blocks = 0;
    for(uint32_t i = 1; i < sizeof(char) * fb_terminal_w; i++){
        if(terminal_buffer[i-1] != ' ' && terminal_buffer[i] == ' ' && terminal_buffer[i+1] != ' '){
            terminal_block_index[terminal_number_of_blocks] = i;
            //printk("Block Border @ %x idx: %x\n", i, terminal_number_of_blocks);
            terminal_number_of_blocks++;
        }
        else if(terminal_buffer[i] == 0){
            terminal_block_index[terminal_number_of_blocks] = i;
            //printk("Block Border @ %x idx: %x\n", i, terminal_number_of_blocks);
            terminal_number_of_blocks++;
            break;
        }
    }
    //printk("Blocks: %x\n", terminal_number_of_blocks);
}

void terminal_parse(){

    if(strcmp(terminal_buffer, "print", 0) == 0){
        printk("Test Terminal\n");
    }
    else if(strcmp(terminal_buffer, "hex", 0) == 0){
        add_process(hex_editor, 2, atoi(1), atoi(2));
    }
    else if(strcmp(terminal_buffer, "ed", 0) == 0){
        add_process(Editor, 2, atoi(1), atoi(2));
    }
    else if(strcmp(terminal_buffer, "art", 0) == 0){
        add_process(Art, 0);
    }
    else if(strcmp(terminal_buffer, "clear", 0) == 0){
        fb_clear(0);
        memset(consoleArray, 0, console_width*console_height);
        consoleLine = 0;
    }    
    else if(strcmp(terminal_buffer, "run", 0) == 0){
        add_process(exec_user_program, 0);
    }
    else if(strcmp(terminal_buffer, "load", 0) == 0){
        uint32_t drive = atoi(1);
        uint32_t sector = atoi(2);
        FS_read(drive, sector, 1, (uint32_t *) filesystem_default_read_buffer);
        memcpy(filesystem_default_read_buffer, (void *) 0, 512);
        //FS_read(drive, sector+1, 1, (uint32_t *) filesystem_default_read_buffer);
        //memcpy(filesystem_default_read_buffer, (void *) 512, 512);
    }
    else if(strcmp(terminal_buffer, "lspci", 0) == 0){
        printk("Listing PCI Devices:\n");
        for(uint32_t i = 0; i < pci_device_num; i++){
            printk("Vendor: %4x Device: %4x Class: %4x\n", pci_devices[i]->vendor, pci_devices[i]->device, pci_devices[i]->class);
        }
    }
    else if(strcmp(terminal_buffer, "echo", 0) == 0){
        printk("%s\n", terminal_buffer + terminal_block_index[0] + 1);
    }
    else if(strcmp(terminal_buffer, "save", 0) == 0){
        uint32_t address = atoi(1);
        uint32_t size = atoi(2);
        uint32_t drive = atoi(3);
        uint32_t sector = atoi(4);
        printk("Saving from memory at %x of size %x to drive #%1x sector %x for %x sectors\n", address, size, drive, sector, size);
        for(uint32_t i = 0; i < size; i++){
            memcpy((void*) 0x0, filesystem_default_read_buffer, 512*i);
            FS_write(drive, sector+i, size, (uint32_t*) filesystem_default_read_buffer);
        }
    }
    else{
        printk("Command Does not Exist\n");
    }
}

void terminal_callback(uint8_t process __attribute__((unused)), uint32_t args[10]__attribute__((unused))){
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

                terminal_locate_blocks();
                terminal_parse();

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