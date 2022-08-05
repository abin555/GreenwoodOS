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
        for(int indexX = 0; indexX < fb_terminal_w; indexX++){
            char sym = consoleArray[indexX + indexY * fb_terminal_w];
            if(sym != '\0' && sym != '\n'){
                fb_write_cell(indexX + indexY * fb_terminal_w, sym, 0xFFFFFF, 0);
            }   
            else if(sym == '\n'){
                indexY++;
                indexX=-1;
            }
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
                consoleLine++;            
                if(consoleLine == (unsigned int) fb_terminal_h){
                    consoleLine = 0;
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
    va_end(listptd);
}
/*
void kprintF(const char* restrict fmt, ...){

}
void panic(const char* restrict msg){

}*/