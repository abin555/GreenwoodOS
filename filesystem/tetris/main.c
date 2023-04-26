#include "../libc/libc.h"

#define width 600
#define height 400

void clear_window(uint32_t color);
void draw_block(int x, int y, uint32_t color);
void draw_title();

bool run = 1;
uint32_t window_buf[width*height];
void *tetris_window;
uint8_t keycode;

uint32_t color_sequence[6] = {
    0x00B6FF,
    0xFFB600,
    0xB6FF00,
    0xB600FF,
    0xFF00B6,
    0x00FFB6
};

void window_event_handler(WINDOW_EVENT e);

int main(){
    print("TETRIS TIME\n");
    get_keycode_next();
    tetris_window = open_window(width, height, window_buf);
    add_window_event_handler(tetris_window, window_event_handler);
    run = 1;
    clear_window(0);
    print("Tetris Window Open\n");
    
    draw_block(50,50, 0x00B6FF);
    draw_title();

    while(run){
        keycode = get_keycode_next();
        switch(keycode){
            case 0x81://Escape
                run = 0;
                print("Escape Pressed\n");
                break;
        }
    }
    close_window(tetris_window);
    print("Tetris Window Close\n");
}

void clear_window(uint32_t color){
    for(uint32_t i = 0; i < width*height; i++){
        window_buf[i] = color;
    }
}

uint32_t block_map[12][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF, 0xF,  0xFFFFFF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF,  0xFFFFFF,  0xFFFFFF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

void draw_block(int x, int y, uint32_t color){
    for(uint32_t block_y = 0; block_y < 12; block_y++){
        for(uint32_t block_x = 0; block_x < 12; block_x++){
            window_buf[
                x+block_x + (y+block_y)*width
            ] = (block_map[block_y][block_x] == 0xF) ? color : block_map[block_y][block_x];
        }
    }
}

uint32_t title_map[5] = {
    0b0111110111100,
    0b0001000100000,
    0b0001000111100,
    0b0001000100000,
    0b0001000111100
};

void draw_title(){
    for(int y = 0; y < 5; y++){
        for(int x = 0; x < 32; x++){
            if(title_map[y] >> x & 1) draw_block(x*12, y*12, color_sequence[y]);
        }
    }
}

void window_event_handler(WINDOW_EVENT e){
    switch(e){
        case close:
            print("Window Close\n");
            run = 0;
            break;
    }
}