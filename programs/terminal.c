#include "terminal.h"

int strcmp(const char *X, const char *Y, int block){
    int start = 0;
    int end = 0;
    if(block != 0){
        start = terminal_block_index[block-1]+1;
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
    terminal_arg_buffer = malloc(sizeof(char) * fb_terminal_w);
    terminal_last_key = keyboard_KEYBUFFER_index;
    terminal_last_char = keyboard_ascii_index;
    terminal_buffer_index = 0;

    memset(terminal_block_index, 0, sizeof(terminal_block_index));
    printk("Terminal Buffer Address: %x\n", terminal_buffer);
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
            FS_write(drive, sector+i, size, (uint32_t*) address);
        }
    }
    else if(strcmp(terminal_buffer, "dump", 0) == 0){
        uint8_t *address = (uint8_t *) atoi(1);
        uint32_t count = atoi(2);
        for(uint32_t i = 0; i < count; i++){
            printk("%2x ", *address);
            address++;
        }
        printk("\n");
    }
    else if(strcmp(terminal_buffer, "loadfs", 0) == 0){
        uint32_t start = terminal_block_index[0]+1;
        printk("Loading File %s\n", terminal_buffer[start]);
        FILE* file = fopen(terminal_buffer+start);
        uint8_t *data_buf = (uint8_t*) 0;
        printk("Size: %x\n", file->size);
        for(uint32_t sector = 0; sector < file->sector_count; sector++){
            uint8_t* read_buf = ISO_read_sector(file->drive, file->sector+sector);
            for(int index = 0; index < 0x800; index++){
                data_buf[sector*0x800 + index] = read_buf[index];
                if(sector * 0x800 + index >= file->size){
                    break;
                }
            }    
        }
        fclose(file);
    }
    else if(strcmp(terminal_buffer, "exec", 0) == 0){
        uint32_t start = terminal_block_index[0]+1;
        printk("Loading File %s\n", terminal_buffer[start]);
        FILE* file = fopen(terminal_buffer+start);
        uint8_t *data_buf = (uint8_t*) 0;
        //printk("Size: %x\n", file->size);
        for(uint32_t sector = 0; sector < file->sector_count; sector++){
            uint8_t* read_buf = ISO_read_sector(file->drive, file->sector+sector);
            for(int index = 0; index < 0x800; index++){
                data_buf[sector*0x800 + index] = read_buf[index];
                if(sector * 0x800 + index >= file->size){
                    break;
                }
            }    
        }
        fclose(file);

        add_process(exec_user_program, 0);
    }
    else if(strcmp(terminal_buffer, "ls", 0) == 0){
        if(strcmp(terminal_buffer, "-a", 1) == 0){
            ISO_list_files();
        }
        else if(strcmp(terminal_buffer, "-v", 1) == 0){
            printk("File Listing of %s:\n", FS_entries[active_directory].name);
            printk("Parent: %s\n", FS_entries[FS_entries[active_directory].parent_item_entry].name);
            for(uint32_t i = 0; i < num_fs_entries; i++){
                if(FS_entries[i].parent_item_entry == active_directory){
                    printk("- %s Size: %x Sector: %x %x Sector Count: %x\n", FS_entries[i].name, FS_entries[i].size, FS_entries[i].sector, FS_entries[i].sector*4, FS_entries[i].sector_count);
                }
            }
        }
        else{
            printk("File Listing of %s:\n", FS_entries[active_directory].name);
            printk("Parent: %s\n", FS_entries[FS_entries[active_directory].parent_item_entry].name);
            for(uint32_t i = 0; i < num_fs_entries; i++){
                if(FS_entries[i].parent_item_entry == active_directory){
                    printk("- %s\n", FS_entries[i].name);
                }
            }
        }
    }
    else if(strcmp(terminal_buffer, "cd", 0) == 0){
        if(strcmp(terminal_buffer, "..", 1) == 0){
            active_directory = FS_entries[active_directory].parent_item_entry;
        }
        else{
            for(uint32_t i = 0; i < num_fs_entries; i++){
                if(FS_entries[i].parent_item_entry == active_directory){
                    if(strcmp(terminal_buffer, FS_entries[i].name, 1) == 0){
                        active_directory = i;
                        break;
                    }
                }
            }
        }
        active_drive = FS_entries[active_directory].drive;
        printk("Dir: %s\n", FS_entries[active_directory].name);
    }
    else if(strcmp(terminal_buffer, "memory?", 0) == 0){
        printk("Used Memory: %x\nRemaining Memory: %x\n", MEMORY_USED, (HEAP_END-HEAP_BEGIN)-MEMORY_USED);
    }
    else if(strcmp(terminal_buffer, "display?", 0) == 0){
        printk("Display %xx%x\n", fb_width, fb_height);
    }
    else if(strcmp(terminal_buffer, "beep", 0) == 0){
        audio_beep();
    }
    else{
        int select_program = -1;
        uint32_t bin_folder_index;
        for(uint32_t i = 0; i < num_fs_entries; i++){
            if(check_str_equ(FS_entries[i].name, "BIN") == 0 && FS_entries[i].parent_item_entry == 0){
                bin_folder_index = i;
                break;
            }
        }
        for(uint32_t i = 0; i < num_fs_entries; i++){
            if((strcmp(terminal_buffer, FS_entries[i].name, 0) == 0) && (FS_entries[i].parent_item_entry == active_directory || FS_entries[i].parent_item_entry == bin_folder_index) && FS_entries[i].type == File){
                select_program = i;
                printk("Found Index: %x\n", select_program);
                break;
            }
        }
        if(select_program == -1){
            printk("Command Does not Exist\n");
        }
        else{
            uint8_t *data_buf = (uint8_t*) 0;
            struct FS_Item_Entry* file = &FS_entries[select_program];

            for(uint32_t sector = 0; sector < file->sector_count; sector++){
                uint8_t* read_buf = ISO_read_sector(file->drive, file->sector+sector);
                for(int index = 0; index < 0x800; index++){
                    data_buf[sector*0x800 + index] = read_buf[index];
                    if(sector * 0x800 + index >= file->size){
                        break;
                    }
                }    
            }
            
            add_process(exec_user_program, 1, terminal_arg_buffer);
            //exec_user_program(0, 0);
        }
    }
}

void terminal_callback(uint8_t process __attribute__((unused)), uint32_t args[10]__attribute__((unused))){
    char ascii;
    uint8_t keycode;
    int directory_namesize = 0;
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

                for(uint32_t i = 0; i < fb_terminal_w; i++){
                    terminal_arg_buffer[i] = terminal_buffer[i];
                }

                terminal_locate_blocks();
                terminal_parse();

                memset(terminal_buffer, 0, fb_terminal_w);
                terminal_buffer_index = 0;
                break;
        }
    }
    directory_namesize=0;
    while(FS_entries[active_directory].name[directory_namesize]){
        fb_setChar(directory_namesize, fb_terminal_h-1, FS_entries[active_directory].name[directory_namesize], 0xFFFFFF, 0);
        directory_namesize++;
    }
    fb_setChar(directory_namesize, fb_terminal_h-1, '>', 0xFFFFFF, 0);

    for(uint32_t i = 0; i < fb_terminal_w; i++){
        //fb_setChar(i, fb_terminal_h-1, terminal_buffer[i], 0xFFFFFF, 0);
        if(i == terminal_buffer_index){
            fb_setChar(directory_namesize+i+1, fb_terminal_h-1, terminal_buffer[i], 0, 0xFFFFFF);
        }
        else{
            fb_setChar(directory_namesize+i+1, fb_terminal_h-1, terminal_buffer[i], 0xFFFFFF, 0);
        }
    }
}