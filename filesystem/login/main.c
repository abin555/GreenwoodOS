#include "../libc/libc.h"
#define Field_Size 24
unsigned char ball_buf[25 * 25 * 4 * sizeof(char)];

void load_image(FILE* file, unsigned char *target_buffer);
void draw_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);
void draw_empty_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);

unsigned int fb_width;
unsigned int fb_height;

char *uname;
char *passwd;
unsigned int *fb;
int main(){
    key_index_prev = get_ascii_key_index();
    print("Starting Login!\n");
    toggle_console();
    toggle_window(0);
    set_backbuffer(1);
    clear_screen(0x00FF8C);
    swap_buffers();    
    setPrintColor(0);
    get_keycode_next();
    fb = getFramebuffer();
    struct display Display = get_display();

    FILE* logo = fopen("./LOGIN/ICON.BIN");
    unsigned char* logo_buffer = malloc(100*100*sizeof(unsigned int));
    uname = malloc(Field_Size * sizeof(char));
    passwd = malloc(Field_Size * sizeof(char));
    load_image(logo, logo_buffer);

    printval("W: %x ", Display.width);
    printval("H: %x\n", Display.height);
    fb_height = Display.height;
    fb_width = Display.width;
    unsigned int field_index = 0;
    char select = 0;
    char run = 1;
    while(run){
        image_load_buffer(100, 100, logo_buffer);
        draw_image(fb_width/2 - 50, 100);

        draw_rect(fb_width/2-((9*Field_Size) / 2),(fb_height / 2)-20,9*Field_Size,12,0xFFFFFF);
        draw_rect(fb_width/2-((9*Field_Size) / 2),(fb_height / 2)+20,9*Field_Size,12,0xFFFFFF);

        for(int i = 0; i < Field_Size; i++){
            printChar(fb_width/2-((9*Field_Size) / 2) + i*8+2, (fb_height / 2)-18, uname[i]);
            printChar(fb_width/2-((9*Field_Size) / 2) + i*8+2, (fb_height / 2)+22, passwd[i]);
        }

        if(select){
            draw_empty_rect(fb_width/2-((9*Field_Size) / 2),(fb_height / 2)+20,9*Field_Size,12,0);
        }
        else{
            draw_empty_rect(fb_width/2-((9*Field_Size) / 2),(fb_height / 2)-20,9*Field_Size,12,0);
        }
        char key = get_key();
        unsigned char keycode = get_keycode_next();
        switch(keycode){
            case 0x0F:
                if(select){
                    select = 0;
                }
                else{
                    select = 1;
                }
                field_index = 0;
                break;
            case 0x9C:
                run = 0;
                break;
        }
        if(key == 'q') break;
        if(key != 0 && field_index < Field_Size){
            if(!select){
                uname[field_index] = key;
            }
            else{
                passwd[field_index] = key;
            }
            field_index++;
        }
        for(int y = 0; y < 10; y++){
            fb[(y+(fb_height / 2)-19)*fb_width + fb_width/2-((9*Field_Size) / 2) + field_index*8+2] = 0;
        }

        swap_buffers();
    }
    free(logo_buffer);
    free(uname);
    free(passwd);
    set_backbuffer(0);
    clear_screen(0);
    toggle_console();
    toggle_window(1);
}

void load_image(FILE* file, unsigned char *target_buffer){
    unsigned char *file_buf;
    for(unsigned int sector = 0; sector < file->sector_count; sector++){
        file_buf = fread(file->drive, file->sector + sector);
        for(unsigned int i = 0; i < 0x800; i++){
            if(sector * 0x800 + i >= file->size){
                break;
            }
            target_buffer[sector*0x800 + i] = file_buf[i];
        }
    }
}

void draw_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color){
    for(unsigned int scan_y = 0; scan_y < h; scan_y++){
        for(unsigned int scan_x = 0; scan_x < w; scan_x++){
            fb[(y+scan_y)*fb_width + (x+scan_x)] = color;
        }
    }
}

void draw_empty_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color){
    unsigned int *fb = getFramebuffer();
    for(unsigned int scan_y = 0; scan_y < h; scan_y++){
        for(unsigned int scan_x = 0; scan_x < w; scan_x++){
            if((scan_y == 0 || scan_y == h-1) || (scan_x == 0 || scan_x == w-1)){
                fb[(y+scan_y)*fb_width + (x+scan_x)] = color;
            }
        }
    }
}