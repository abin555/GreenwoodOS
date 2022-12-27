#include "../libc/libc.h"

uint32_t calculate_scroll_index(uint32_t scroll, char *target_buffer);
void save_text(FILE* file, uint8_t *target_buffer);
void load_text(FILE* file, uint8_t *target_buffer);

char topMessage[] = "Opened File: ";
FILE* open_file;

int main(){
    register char* arg asm("eax");
    int start_index = 0;
    while(*(arg+start_index) != ' '){
        start_index++;
    }
    char* filename = (arg+start_index+1);
    printval("Editing File: %s\n", (int) filename);
    open_file = fopen(filename);
    char *file_contents = malloc(open_file->size);
    load_text(open_file, file_contents);
    struct display Display = get_display();

    get_keycode_next();
    get_key();
    toggle_console();
    set_backbuffer(1);
    clear_screen(0);
    swap_buffers();
    setPrintColor(0xFFFFFF);

    uint8_t run_editor = 1;
    char key;
    uint8_t keycode;
    uint16_t scroll_y = 0;
    uint32_t scan_x = 0;
    uint32_t scan_y = 0;
    uint32_t cursor_index = 0;

    while(run_editor){
        clear_screen(0);
        for(int i = 0; i < sizeof(topMessage); i++){
            printChar(8*i,1,topMessage[i]);
        }
        for(int i = 0; filename[i] != 0; i++){
            printChar(8*(i+sizeof(topMessage)), 1, filename[i]);
        }
        key = get_key();
        keycode = get_keycode_next();
        if(key != 0){
            file_contents[cursor_index] = key;
            cursor_index++;
        }
        switch(keycode){
            case 0x81://Escape
                run_editor = 0;
                break;
            case 0x49://Page Up
                if(scroll_y != 0) scroll_y--;
                break;
            case 0x51://Page Down
                scroll_y++;
                break;
            case 0x4B://Left Arrow?
                if(cursor_index != 0) cursor_index--;
                break;
            case 0x4D://Right Arrow?
                if(cursor_index != open_file->size) cursor_index++;
                break;
            case 0x1C://Enter
                file_contents[cursor_index] = '\n';
                cursor_index++;
                break;
        }
        scan_x = 0;
        scan_y = 0;

        for(uint32_t scan_file_index = calculate_scroll_index(scroll_y, file_contents); scan_file_index < open_file->size; scan_file_index++){
            char file_sym = file_contents[scan_file_index];
            if(file_sym == '\n'){
                scan_x = 0;
                scan_y++;
                continue;
            }
            if(scan_y >= (Display.height / 8) - 4){
                break;
            }
            if(scan_file_index == cursor_index){
                setPrintColor(0);
            }
            printChar((5+scan_x)*8+1, (4+scan_y)*8+1, file_sym);
            if(scan_file_index == cursor_index){
                setPrintColor(0xFFFFFF);
            }
            scan_x++;            
        }

        swap_buffers();
    }
    clear_screen(0);
    swap_buffers();

    setPrintColor(0);
    set_backbuffer(0);
    toggle_console();
    free(file_contents);
    fclose(open_file);
}

uint32_t calculate_scroll_index(uint32_t scroll, char *target_buffer){
    uint32_t found_ys = 0;
    uint32_t scan_file_index = 0;
    while(found_ys != scroll){
        if(target_buffer[scan_file_index] == '\n'){
            found_ys++;
        }
        if(scan_file_index < open_file->size){
            scan_file_index++;
        }
        else{
            scan_file_index = open_file->size - 1;
            break;
        }
    }
    return scan_file_index;
}

void save_text(FILE* file, unsigned char *target_buffer){

}

void load_text(FILE* file, unsigned char *target_buffer){
    unsigned char *file_buf;
    for(unsigned int sector = 0; sector < file->sector_count; sector++){
        file_buf = fread(file->drive, file->sector + sector);
        for(unsigned int i = 0; i < 0x800; i++){
            if(sector * 0x800 + i > file->size){
                break;
            }
            target_buffer[sector*0x800 + i] = file_buf[i];
        }
    }
}