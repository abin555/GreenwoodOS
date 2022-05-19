#include "console.h"

void initializeConsole(){
    consoleArray = (char *) malloc(fb_terminal_w * fb_terminal_h);
    consoleSize = mgetSize(consoleArray);
    memset(consoleArray, 0, consoleSize);
    consoleLine = 0;//Top of the screen
    consoleStart = 0;
}

void console_putScreen(){
    for(int index = 0; index < fb_terminal_w * fb_terminal_h; index++){
        if(consoleArray[index] != '\n'){
            fb_write_cell(index, consoleArray[index], 0xFFFFFF, 0);
        }
        else{
            index += fb_terminal_w - (fb_terminal_w % index);
        }
    }
}

void printk(char* msg){
    uint32_t p = 0;
    char allowed = 1;
    //printChar(p, consoleLine-consoleStart, msg[p]);
    //memset(consoleArray + consoleStart*fb_terminal_w - consoleLine*fb_terminal_w, 0, fb_terminal_w);
    while(p < (unsigned int)fb_terminal_w){
        if(msg[p] != '\n' && allowed){
            consoleArray[consoleLine*fb_terminal_w + p] = msg[p];
        }
        else if(msg[p] == '\n'){
            consoleArray[consoleLine*fb_terminal_w + p] = msg[p];
            allowed = 0;
        }
        else{
            consoleArray[consoleLine*fb_terminal_w + p] = 0;
        }
        p++;
    }

    console_putScreen();
    consoleLine++;
    if(consoleLine == (unsigned int) fb_terminal_h){
        consoleLine = 0;
    }
}
/*
void kprintF(const char* restrict fmt, ...){

}
void panic(const char* restrict msg){

}*/