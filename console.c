#include "console.h"

void initializeConsole(){
    consoleArray = (char *) malloc(fb_terminal_w * fb_terminal_h);
    consoleSize = mgetSize(consoleArray);
    memset(consoleArray, 0, consoleSize);
    consoleLine = 0;//Top of the screen
    consoleStart = 0;
    consoleLinePlace = 0;
}

void console_putScreen(){
    for(int indexY = 0; indexY < fb_terminal_h; indexY++){
        char clearline = 0;
        for(int indexX = 0; indexX < fb_terminal_w; indexX++){
            fb_write_cell(indexX + indexY * fb_terminal_w, ' ', 0xFFFFFF, 0);
            char sym = consoleArray[indexX + indexY * fb_terminal_w];
            if(sym != '\0' && sym != '\n' && !clearline){
                fb_write_cell(indexX + indexY * fb_terminal_w, sym, 0xFFFFFF, 0);
            }
            else{
                clearline = 1;
            }
        }
    }
}

void shiftConsoleUp(){
    for(int places = 0; places < fb_terminal_w; places++){
        for(int i = 0; i < fb_terminal_h*fb_terminal_w-1; i++){
            consoleArray[i] = consoleArray[i+1];
        }
    }
}

int calculateNumberLength(unsigned int data, int base){
    switch(base){
        case(16):
            if(data > 0x0FFFFFFF){
                return 8;
            }
            else if(data > 0x00FFFFFF){
                return 8;
            }
            else if(data > 0x000FFFFF){
                return 8;
            }
            else if(data > 0x0000FFFF){
                return 8;
            }
            else if(data > 0x00000FFF){
                return 4;
            }
            else if(data > 0x000000FF){
                return 4;
            }
            else if(data > 0x0000000F){
                return 2;
            }
            return 1;
        break;
        case(10):
            return 8;
        break;
        case(2):
        return sizeof(unsigned int);
    }
    return 8;
}


int printDecimal(unsigned int data, int setlength){
    return data % setlength;
}
int printHex(unsigned int data, int setlength){
    for(int i = 0; i < (setlength ? setlength : 8); i++){
        consoleArray[consoleLine*fb_terminal_w + consoleLinePlace + (setlength ? setlength : 8) - 1 - i] = quadToHex((data >> 4*i) & 0xF);
    }
    return (setlength ? setlength : 8);
}
int printBinary(unsigned int data, int setlength){
    for(int i = 0; i < (setlength ? setlength : 32); i++){
        if((data >> i) & 1){
            consoleArray[consoleLine*fb_terminal_w + consoleLinePlace + i] = '1';
        }
        else{
            consoleArray[consoleLine*fb_terminal_w + consoleLinePlace + i] = '0';
        }
    }
    return 32;
}

void printk(char* msg, ...){
    uint32_t p = 0;
    char allowed = 1;
    va_list listptd;
    va_start(listptd, msg);
    //printChar(p, consoleLine-consoleStart, msg[p]);
    //memset(consoleArray + consoleStart*fb_terminal_w - consoleLine*fb_terminal_w, 0, fb_terminal_w);
    while(p < (unsigned int)fb_terminal_w){
        int setlength = 0;
        if(msg[p] == '%'){
            parseaftermodify:;
            p++;
            switch(msg[p]){
                case 'h'://Hexadecimal
                case 'H':
                case 'x':
                case 'X':
                    consoleLinePlace += printHex(va_arg(listptd, unsigned int), setlength);
                break;
                case 'b'://Binary
                case 'B':
                    consoleLinePlace += printBinary(va_arg(listptd, unsigned int), setlength);
                break;
                case 'd'://Decimal
                case 'D':
                    consoleLinePlace += printDecimal(va_arg(listptd, unsigned int), setlength);
                break;
                case 'c':
                case 'C':
                    consoleLinePlace += va_arg(listptd, unsigned int);
                    break;
                default:
                if(msg[p] == '1' && msg[p+1] == '6'){
                    setlength = 16;
                    p++;
                    goto parseaftermodify;
                }
                else if(msg[p] == '3' && msg[p+1] == '2'){
                    setlength = 32;
                    p++;
                    goto parseaftermodify;
                }
                else if(msg[p] == '1'){
                    setlength = 1;
                    goto parseaftermodify;
                }
                else if(msg[p] == '2'){
                    setlength = 2;
                    goto parseaftermodify;
                }
                else if(msg[p] == '3'){
                    setlength = 3;
                    goto parseaftermodify;
                }
                else if(msg[p] == '4'){
                    setlength = 4;
                    goto parseaftermodify;
                }
                else if(msg[p] == '8'){
                    setlength = 8;
                    goto parseaftermodify;
                }
            }
        }
        else if(msg[p] != '\0' && allowed){
            
            consoleArray[consoleLine*fb_terminal_w + consoleLinePlace] = msg[p];
            if(msg[p] == '\n'){
                consoleLinePlace = 0;          
                if(consoleLine == (unsigned int) fb_terminal_h-2){
                    shiftConsoleUp();
                }
                else{
                    consoleLine++;  
                }
            }
            else{
                consoleLinePlace++;
            }
        }
        else if(msg[p] == '\0'){
            //consoleArray[consoleLine*fb_terminal_w + cp] = msg[p];
            allowed = 0;
            break;
        }
        else{
            //consoleArray[consoleLine*fb_terminal_w + cp] = 0;
        }
        p++;
    }

    console_putScreen();
    printChar(0, consoleLine, '*');
    va_end(listptd);
    /*
    for(int loop1 = 0; loop1 < 0xFFFF; loop1++){
        for(int loop2 = 0; loop2 < 0x6FF; loop2++){

        }
    }*/
}
/*
void kprintF(const char* restrict fmt, ...){

}
void panic(const char* restrict msg){

}*/