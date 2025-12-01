#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/vp.h>
#include <sys/task.h>
#include <sys/io.h>
#include "tetris.h"
#include "screen.h"

int running;
struct Tetris t;
extern struct Viewport *window;

void fallEvent(){
    if(!tetris_moveDown(&t)){
        tetris_addBlock(&t);
        tetris_checkClear(&t);
        tetris_newBlock(&t);
    }
}

struct RealTimeClock {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;
	unsigned int msec;
};

void gettime(int rtc_fd, int *sec, int *usec){
    struct RealTimeClock rtc;
    read(rtc_fd, &rtc, sizeof(rtc));
    *sec = 60*rtc.minute + rtc.second;
    *usec = rtc.msec;
}

int main(int argc, char **argv){
    int rtc_fd = open("/-/dev/RTC", O_READ);
    printf("TETRIS\n");
    initScreen();
    tetris_init(&t);

    invertDisplay(1);

    int sec, msec;
    int lock = 0;
    
    while(running){
        //gettime(rtc_fd, &sec, &msec);
        clearDisplay();
        tetris_draw(&t);
        
        showScreen();

        if(window->ascii == 's'){
            window->ascii = '\0';
            tetris_move(&t, Down);
        }
        if(window->ascii == 'w'){
            window->ascii = '\0';
            tetris_move(&t, Rotate);
        }
        if(window->ascii == 'f'){
            window->ascii = '\0';
            fallEvent();
        }
        if(window->ascii == 'a'){
            window->ascii = '\0';
            tetris_move(&t, Left);
        }
        if(window->ascii == 'd'){
            window->ascii = '\0';
            tetris_move(&t, Right);
        }
        yield();
    }
}