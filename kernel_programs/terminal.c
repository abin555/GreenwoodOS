#include "terminal.h"
#define Terminal_Y 20

int Terminal_OUT_pointer = 0;
unsigned short Terminal_Buffer_Pointer = 0;
unsigned int previousASCII_pointer = 0;
unsigned int previousKEY_pointer = 0;

void terminal_renderer(){
    fb_clear(' ', FB_WHITE, FB_BLACK);
    fb_move_cursor_xy(1,Terminal_Y);
    printChar(0, Terminal_Y, '>');
    fb_write_xy(Terminal_Buffer, TERMINAL_Buffer_Size, Terminal_Buffer_Pointer, 1, Terminal_Y);
}

void terminal_console(){
    Terminal_Buffer[Terminal_Buffer_Pointer] = keyboard_ASCIIBuffer[keyboard_ascii_pointer-1];
    printChar(Terminal_Buffer_Pointer+1, Terminal_Y, Terminal_Buffer[Terminal_Buffer_Pointer]);
    Terminal_Buffer_Pointer++;
    
    fb_move_cursor_xy(Terminal_Buffer_Pointer+1, Terminal_Y);

    printChar(0, Terminal_Y, '>');
}

void terminal_enter(){
    STR_INSERT(Terminal_Buffer, Terminal_OUT_Buffer, TERMINAL_Buffer_Size, Terminal_OUT_pointer);
    fb_write_xy(Terminal_OUT_Buffer, TERMINAL_Buffer_Size*19, Terminal_OUT_pointer, 0, 0);
    Terminal_OUT_pointer += 80;
    Terminal_Buffer_Pointer = 0;
    for(int i = 0; i < TERMINAL_Buffer_Size; i++){
        Terminal_Buffer[i] = 0;
        printChar(i+1, Terminal_Y, ' ');
    }
    fb_move_cursor_xy(1, Terminal_Y);
}

void terminal_handler(){
    /*
    STR_INSERT("Testing Buffer System", Terminal_OUT_Buffer, 20, 0);
    STR_INSERT("Command Buffer", Terminal_Buffer, 16, 0);
    */
    if(keyboard_ascii_pointer-1 != previousASCII_pointer && Terminal_Buffer_Pointer < TERMINAL_Buffer_Size){
        //printChar(keyboard_KEYBUFFER_POINTER-1, Terminal_Y, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1]);
        terminal_console();
        previousASCII_pointer = keyboard_ascii_pointer-1;
    }
    else if(keyboard_KEYBUFFER_POINTER != previousKEY_pointer){
        switch(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1]){
            case 0x0E:
            if(Terminal_Buffer != 0){
                if(Terminal_Buffer_Pointer > 1){
                    Terminal_Buffer_Pointer -= 1;
                }
                fb_move_cursor_xy(Terminal_Buffer_Pointer, Terminal_Y);
                Terminal_Buffer[Terminal_Buffer_Pointer] = ' ';
                printChar(Terminal_Buffer_Pointer, Terminal_Y, ' ');

                
            }
            break;
            case 0x9C:
            terminal_enter();
            break;
        }
        previousKEY_pointer = keyboard_KEYBUFFER_POINTER;
    }
    else if(Terminal_Buffer_Pointer > 0b100000000000000){
        Terminal_Buffer_Pointer = 0;
    }
    decodeData(STR_edit, (Terminal_Buffer_Pointer << 1), 16, 0);
    fb_write_xy(STR_edit, 16, 0, 0, 22);
    //Terminal_OUT_Buffer[0] = 'A';
    //terminal_renderer();
}