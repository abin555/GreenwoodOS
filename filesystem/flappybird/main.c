#include "../libc/libc.h"

void clear_img(unsigned int x, unsigned int y);
void delay(unsigned int ticks);
void load_image(FILE* file, unsigned char *target_buffer);
void render_background();
void clear_bird();
void draw_pipe(int x, int y);

//uint32_t fb_width;
//uint32_t fb_height;

#define fb_width 1280
#define fb_height 768

unsigned char bird_buf[0x334];
unsigned char BACK_buf[0x27604];
unsigned char pipe_buf[0x13684];
unsigned char pipe_seg_buf[0xE44];
unsigned int back_scroll;

struct PIPE{
    int x;
    int y;
};

struct display Display;

struct PIPE pipes[4] = {
    {320*1-100, 400},
    {320*2-100, 400},
    {320*3-100, 400},
    {320*4-100, 400}
};
int main(){
    key_index_prev = get_ascii_key_index();
    printval("Ticks %x\n", get_timer_ticks());
    print("Program Start!\n");
    Display = get_display();

    toggle_console();
    toggle_window();
    set_backbuffer(1);
    clear_screen(0x71C5CF);
    swap_buffers();
    FILE* bird = fopen("./FLAPPYBI/BIRD.BIN");
    FILE* BACK = fopen("./FLAPPYBI/BACK.BIN");
    FILE* pipe = fopen("./FLAPPYBI/PIPE.BIN");
    FILE* pipe_seg = fopen("./FLAPPYBI/PIPESEG.BIN");

    load_image(bird, bird_buf);
    load_image(BACK, BACK_buf);
    load_image(pipe, pipe_buf);
    load_image(pipe_seg, pipe_seg_buf);
    
    render_background();
    image_load_buffer(69, 288, pipe_buf);
    draw_image(640, 112);

    image_load_buffer(17,12, bird_buf);

    int run = 1;
    unsigned int clock = 0;
    unsigned int bird_X = 50;
    int bird_Y = 250;
    back_scroll = 0;
    unsigned int draw_timer = 0;
    unsigned int phys_timer = 0;
    unsigned int pipe_timer = 0;
    unsigned int back_timer = 0;
    unsigned int delay = 5;
    draw_image(bird_X,bird_Y);
    while(run){
        clock = get_timer_ticks();
        while(clock+delay > get_timer_ticks()){}
        if(clock > phys_timer + 20 && bird_Y < fb_height-82){
            bird_Y+=4;
            phys_timer = clock;
        }
        switch(get_key()){
            case 'q':
            run = 0;
            break;
            case 'F':
            for(int y = 0; y < fb_height/12; y++){
                for(int x = 0; x < fb_width/17; x++){
                    draw_image(x*17, y*12);
                }
            }
            break;
            case 'w':
            case ' ':
                bird_Y-=24;
                break;
            case 's':
                bird_Y+=12;
                break;
            case '[':
                delay -= 5;
                break;
            case ']':
                delay += 5;
                break;
        } 

        if(clock > back_timer + 7){
            back_timer = clock;
            if(back_scroll >= 192){
                back_scroll = 0;
            }
            else{
                back_scroll++;
            }
        }
        
        if(clock > pipe_timer + 10){
            pipe_timer = clock;
            for(int i = 0; i < 4; i++){
                pipes[i].x-=5;
                if(pipes[i].x <= 0){
                    pipes[i].x = fb_width;
                    pipes[i].y = 200+ (get_timer_ticks() % 200);
                }
            }
        }

        if(clock > draw_timer + 20){
            draw_timer = clock;
            clear_screen(0x71C5CF);
            render_background();    
            
            image_load_buffer(17,12, bird_buf);
            draw_image(bird_X,bird_Y);

            swap_buffers();
        }
    }

    free(bird_buf);
    free(BACK_buf);
    free(pipe_buf);
    fclose(pipe);
    fclose(BACK);
    fclose(bird);
    set_backbuffer(0);
    clear_screen(0);
    toggle_window();
    toggle_console();
    print("Program Done!\n");
    printval("Ticks %x\n", get_timer_ticks());
}

void draw_pipe(int x, int y){
    image_load_buffer(69, 288, pipe_buf);
    draw_image(x-34, y-144);
    image_load_buffer(57,16, pipe_seg_buf);
    for(int i = 0; i < y-144; i+=16){
        draw_image(x-28, i);
    }
    for(int i = fb_height; i > y+144-16; i-=16){
        draw_image(x-28, i);
    }
}

void clear_bird(){
    image_load_buffer(50, fb_height-210, 0);
    clear_img(25, 0);
    image_load_buffer(17,12, bird_buf);
}

void render_background(){
    image_load_buffer(192,210, BACK_buf);
    for(int x = -192; x < fb_width+192; x+=192){
        draw_image(x-back_scroll, fb_height-210);
    }
    
    for(int i = 0; i < 4; i++){
        draw_pipe(pipes[i].x, pipes[i].y);
    }
}

void clear_img(unsigned int x, unsigned int y){
    register unsigned int color asm("edx") = 0x71C5CF;
    register unsigned int x_val asm("ebx") = x;
    register unsigned int y_val asm("ecx") = y;
    register unsigned int syscall asm("eax") = 0x0E;
    asm volatile("int 0x80");
}

void delay(unsigned int ticks){
    unsigned int start_ticks = get_timer_ticks();

    while(get_timer_ticks() < ticks + start_ticks){

    }
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