#include "console.h"

void initialize_console(uint32_t width, uint32_t height){
    consoleArray = (char *) malloc(width * height);
    consoleSize = mgetSize(consoleArray);
    memset(consoleArray, 0, consoleSize);
    consoleLine = 0;
    consoleStart = 0;
    consoleLinePlace = 0;
    console_width = width;
    console_height = height;
    console_last_line = 0;
    console_initialized = 1;
    console_color_fg = 0xFFFFFF;
    console_color_bg = 0;
}

void console_putScreen(){
    if(!console_initialized) return;
    //fb_clear(0);
    for(uint32_t indexY = console_last_line; indexY < consoleLine+1; indexY++){
        char clearline = 0;
        for(uint32_t indexX = 0; indexX < console_width; indexX++){
            //fb_write_cell(indexX + indexY * fb_terminal_w, ' ', 0xFFFFFF, 0);
            char sym = consoleArray[indexX + indexY * fb_terminal_w];
            if(sym != '\0' && sym != '\n' && !clearline){
                fb_write_cell(indexX + indexY * fb_terminal_w, sym, console_color_fg, console_color_bg);
            }
            else{
                clearline = 1;
            }
        }
    }
    console_last_line = consoleLine-1;
}

void console_putLine(uint32_t places){
    for(uint32_t index = 0; index < places; index++){
        fb_write_cell(consoleLine*fb_terminal_w + index, consoleArray[consoleLine*fb_terminal_w + index], 0xFFFFFF, 0);
    }
}

void shiftConsoleUp(){
    console_last_line = 0;
    for(uint32_t places = 0; places < fb_terminal_w; places++){
        for(uint32_t i = 0; i < fb_terminal_h*fb_terminal_w-1; i++){
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
    if(!console_initialized) return;
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
                    fb_clear(0);
                    //memset(consoleArray, 0, console_width*console_height);
                    //consoleLine = 0;
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
    //console_putLine(p);
    //fb_setChar(0, consoleLine, '*', 0xFFFFFF, 0);
    va_end(listptd);
    /*
    for(int loop1 = 0; loop1 < 0xFFFF; loop1++){
        for(int loop2 = 0; loop2 < 0x6FF; loop2++){

        }
    }*/
}

char quadToHex(char quad){
  switch(quad){
    case 0x00:
      return '0';
      break;
    case 0x01:
      return '1';
      break;
    case 0x02:
      return '2';
      break;
    case 0x03:
      return '3';
      break;
    case 0x04:
      return '4';
      break;
    case 0x05:
      return '5';
      break;
    case 0x06:
      return '6';
      break;
    case 0x07:
      return '7';
      break;
    case 0x08:
      return '8';
      break;
    case 0x09:
      return '9';
      break;
    case 0x0a:
      return 'A';
      break;
    case 0x0b:
      return 'B';
      break;
    case 0x0c:
      return 'C';
      break;
    case 0x0d:
      return 'D';
      break;
    case 0x0e:
      return 'E';
      break;
    case 0x0f:
      return 'F';
      break;
  }
  return 'x';
}

void console_clear(){
    if(!console_initialized) return;
    memset(consoleArray, 0, fb_terminal_h*fb_terminal_w);
    memset(consoleArray, 0, consoleSize);
    consoleLine = 0;
    consoleStart = 0;
    consoleLinePlace = 0;
    console_last_line = 0;
}