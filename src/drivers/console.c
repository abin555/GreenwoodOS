#include "console.h"
#include "window_manager.h"

void initialize_console(uint32_t width, uint32_t height){
    print_serial("Console Init! %x x %x\n", width, height);
    consoleArray = (char *) malloc(width * height);
    consoleSize = width*height;
    //memset(consoleArray, 0, consoleSize);
    consoleLine = 0;
    consoleStart = 0;
    consoleLinePlace = 0;
    console_width = width;
    console_height = height;
    console_last_line = 0;
    console_initialized = 1;
    console_color_fg = 0xFFFFFF;
    console_color_bg = 0;
    console_fb = framebuffer;
    print_serial("Console Init Finished!\n");
}

void console_putScreen(){
    if(!console_initialized) return;
    //fb_clear(0);
    for(uint32_t indexY = console_last_line; indexY < consoleLine+1; indexY++){
        char clearline = 0;
        for(uint32_t indexX = 0; indexX < console_width; indexX++){
            //fb_write_cell(indexX + indexY * fb_terminal_w, ' ', 0xFFFFFF, 0);
            char sym = consoleArray[indexX + indexY * console_width];
            if(sym != '\0' && sym != '\n' && !clearline){
                buf_write_cell(console_fb, console_width*8, indexX + indexY * console_width, sym, console_color_fg, console_color_bg);
            }
            else{
                clearline = 1;
            }
        }
    }
    console_last_line = consoleLine-1;
    if(use_window) request_redraw();
}

void console_fullPut(){
    if(!console_initialized) return;
    //fb_clear(0);
    for(uint32_t indexY = 0; indexY < consoleLine+1; indexY++){
        char clearline = 0;
        for(uint32_t indexX = 0; indexX < console_width; indexX++){
            //fb_write_cell(indexX + indexY * fb_terminal_w, ' ', 0xFFFFFF, 0);
            char sym = consoleArray[indexX + indexY * console_width];
            if(sym != '\0' && sym != '\n' && !clearline){
                buf_write_cell(console_fb, console_width*8, indexX + indexY * console_width, sym, console_color_fg, console_color_bg);
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
        buf_write_cell(console_fb, console_width*8, consoleLine*console_width + index, consoleArray[consoleLine*console_width + index], 0xFFFFFF, 0);
    }
}

void shiftConsoleUp(){
    console_last_line = 0;
    for(uint32_t places = 0; places < console_width; places++){
        for(uint32_t i = 0; i < console_height*console_width-1; i++){
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

int printDecimal(int data, int offset){
    int idx = 0;
    int pow = 1;
    if(data < 0){
        consoleArray[consoleLine*console_width + consoleLinePlace + idx + offset] = '-';
        idx++;
        data *= -1;
    }
    while(pow * 10 <= data)
        pow *= 10;
    while(pow != 0){
        int d = data / pow;
        consoleArray[consoleLine*console_width + consoleLinePlace + idx + offset] = (char)((int)'0' + d);
        data = data - d * pow;
        pow /= 10;
        idx++;
    }
    return idx;
}

int printHex(unsigned int data, int setlength){
    for(int i = 0; i < (setlength ? setlength : 8); i++){
        consoleArray[consoleLine*console_width + consoleLinePlace + (setlength ? setlength : 8) - 1 - i] = quadToHex((data >> 4*i) & 0xF);
    }
    return (setlength ? setlength : 8);
}

int printBinary(unsigned int data, int setlength){
    for(int i = 0; i < (setlength ? setlength : 32); i++){
        if((data >> i) & 1){
            consoleArray[consoleLine*console_width + consoleLinePlace + i] = '1';
        }
        else{
            consoleArray[consoleLine*console_width + consoleLinePlace + i] = '0';
        }
    }
    return 32;
}

int printFloat(double data){
    int i = (int)data;
    int sizePart1 = printDecimal(i, 0);
    data = (data-i)*1000000;
    i = abs((int)data);
    if(fabs(data) - i >= 0.5){
        i++;
    }
    consoleArray[consoleLine*console_width + consoleLinePlace + sizePart1] = '.';
    int sizePart2 = printDecimal(i, sizePart1 + 1);
    return sizePart1 + sizePart2 + 1;
}

void printk(char* msg, ...){
    if(!console_initialized) return;
    uint32_t p = 0;
    char allowed = 1;
    va_list listptd;
    va_start(listptd, msg);
    //printChar(p, consoleLine-consoleStart, msg[p]);
    //memset(consoleArray + consoleStart*fb_terminal_w - consoleLine*fb_terminal_w, 0, fb_terminal_w);
    while(p < (unsigned int)console_width){
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
                    consoleLinePlace += printDecimal(va_arg(listptd, int), 0);
                break;
                case 'f'://float
                case 'F':
                    consoleLinePlace += printFloat(va_arg(listptd, double));
                break;
                case 'c':
                case 'C':
                    //consoleLinePlace += va_arg(listptd, char);
                    consoleArray[consoleLine*console_width + consoleLinePlace] = (char) va_arg(listptd, unsigned int);
                    consoleLinePlace += 1;
                    break;
                case 's':
                case 'S': {
                    char *str = (char *) va_arg(listptd, unsigned int);
                    while(*str != '\0'){
                        consoleArray[consoleLine*console_width + consoleLinePlace] = *str;
                        consoleLinePlace += 1;
                        str++;
                    }
                }
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
                else if(msg[p] == '5'){
                    setlength = 5;
                    goto parseaftermodify;
                }
                else if(msg[p] == '6'){
                    setlength = 6;
                    goto parseaftermodify;
                }
                else if(msg[p] == '7'){
                    setlength = 7;
                    goto parseaftermodify;
                }
                else if(msg[p] == '8'){
                    setlength = 8;
                    goto parseaftermodify;
                }
            }
        }
        else if(msg[p] != '\0' && allowed){
            
            consoleArray[consoleLine*console_width + consoleLinePlace] = msg[p];
            if(msg[p] == '\n'){
                consoleLinePlace = 0;          
                if(consoleLine == (unsigned int) console_height-2){
                    shiftConsoleUp();
                    console_wipebuf();
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

char quadToHex(uint8_t quad){
  switch(quad){
    case 0x00:
      return '0';
      break;
    case 0x01:
    case 0x10:
      return '1';
      break;
    case 0x02:
    case 0x20:
      return '2';
      break;
    case 0x03:
    case 0x30:
      return '3';
      break;
    case 0x04:
    case 0x40:
      return '4';
      break;
    case 0x05:
    case 0x50:
      return '5';
      break;
    case 0x06:
    case 0x60:
      return '6';
      break;
    case 0x07:
    case 0x70:
      return '7';
      break;
    case 0x08:
    case 0x80:
      return '8';
      break;
    case 0x09:
    case 0x90:
      return '9';
      break;
    case 0x0a:
    case 0xa0:
      return 'A';
      break;
    case 0x0b:
    case 0xb0:
      return 'B';
      break;
    case 0x0c:
    case 0xc0:
      return 'C';
      break;
    case 0x0d:
    case 0xd0:
      return 'D';
      break;
    case 0x0e:
    case 0xe0:
      return 'E';
      break;
    case 0x0f:
    case 0xf0:
      return 'F';
      break;
  }
  return 'x';
}

void console_clear(){
    if(!console_initialized) return;
    memset(consoleArray, 0, consoleSize);
    consoleLine = 0;
    consoleStart = 0;
    consoleLinePlace = 0;
    console_last_line = 0;
}

#define ConsoleWindowWidth 80
#define ConsoleWindowHeight 60

struct window* console_window;
uint32_t console_window_buf[8*ConsoleWindowWidth*8*ConsoleWindowHeight*sizeof(uint32_t)];

void console_addWindow(){
    console_clear();
    console_width = ConsoleWindowWidth;
    console_height = ConsoleWindowHeight;
    console_window = create_window(console_width*8, console_height*8, console_window_buf);
    console_window->screen_x = fb_width-((console_width + 2)*8);
    console_fb = console_window->window_buf;
}

void console_wipebuf(){
    memset(console_fb, 0, sizeof(uint32_t) * (8*console_width)*(8*console_height));
}