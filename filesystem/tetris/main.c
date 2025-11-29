#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/vp.h>
#include <sys/task.h>
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

int main(int argc, char **argv){
    printf("TETRIS\n");
    initScreen();
    tetris_init(&t);

    invertDisplay(1);
    while(running){
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