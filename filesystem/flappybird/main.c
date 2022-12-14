#include "libc.h"

int key_index_prev;
char get_key();
void clear_img(unsigned int x, unsigned int y);
void delay(unsigned int ticks);
void load_image(FILE* file, unsigned char *target_buffer);

int main(){
    key_index_prev = get_ascii_key_index();
    printval("Ticks %x\n", get_timer_ticks());
    print("Program Start!\n");
    toggle_console();
    clear_screen();
    FILE* bird = fopen("./FLAPPYBI/BIRD.BIN");
    FILE* BACK = fopen("./FLAPPYBI/BACK.BIN");

    unsigned char *bird_buf = malloc(bird->size);
    unsigned char *BACK_buf = malloc(BACK->size);

    load_image(bird, bird_buf);
    load_image(BACK, BACK_buf);
    image_load_buffer(192,210, BACK_buf);
    for(int x = 0; x < 1280; x+=192){
        draw_image(x, 800-210);
    }

    image_load_buffer(17,12, bird_buf);
    for(int i = 0; i < 10; i++){
        draw_image(20*i,12*i);
    }
    int run = 1;
    unsigned int clock = 0;
    unsigned int bird_X = 50;
    unsigned int bird_Y = 250;
    draw_image(bird_X,bird_Y);
    while(run){
        clock++;
        if(!(clock % 0xFFFFF) && bird_Y < 400){
            clear_img(bird_X,bird_Y);
            bird_Y+=12;
            draw_image(bird_X,bird_Y);
        }
        switch(get_key()){
            case 'q':
            run = 0;
            break;
            case 'F':
            for(int y = 0; y < 780/12; y++){
                for(int x = 0; x < 1280/17; x++){
                    draw_image(x*17, y*12);
                }
            }
            break;
            case 'C':
                clear_screen();
            break;
            case 'w':
            case ' ':
                clear_img(bird_X,bird_Y);
                bird_Y-=12;
                draw_image(bird_X,bird_Y);
                break;
            case 's':
                clear_img(bird_X,bird_Y);
                bird_Y+=12;
                draw_image(bird_X,bird_Y);
                break;
        }
    }

    free(bird_buf);
    fclose(bird);
    toggle_console();
    print("Program Done!\n");
    printval("Ticks %x\n", get_timer_ticks());
}

void clear_img(unsigned int x, unsigned int y){
    register unsigned int x_val asm("ebx") = x;
    register unsigned int y_val asm("ecx") = y;
    register unsigned int syscall asm("eax") = 0x0E;
    asm volatile("int 0x80");
}

char get_key(){
    int current_index = get_ascii_key_index();
    if(key_index_prev != current_index){
        key_index_prev = current_index;
        return get_ascii_key();
    }
    else{
        return 0;
    }
}

void delay(unsigned int ticks){
    unsigned int start_ticks = get_timer_ticks();

    while(get_timer_ticks() < ticks + start_ticks){

    }
}

void load_image(FILE* file, unsigned char *target_buffer){
    unsigned char *file_buf;
    for(unsigned int sector = 0; sector < file->sector_count; sector++){
        file_buf = fread(file->sector + sector);
        for(unsigned int i = 0; i < 0x800; i++){
            if(sector * 0x800 + i >= file->size){
                break;
            }
            target_buffer[sector*0x800 + i] = file_buf[i];
        }
    }
}