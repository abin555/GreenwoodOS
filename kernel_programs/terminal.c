#include "terminal.h"
#define Terminal_Y 20

int Terminal_OUT_pointer = 0;
int Terminal_Buffer_Pointer = 0;
unsigned int previousKYBD_pointer = 0;

void terminal_renderer(){
    fb_clear(' ', FB_WHITE, FB_BLACK);
    fb_write_xy(Terminal_OUT_Buffer, TERMINAL_Buffer_Size*40, Terminal_OUT_pointer, 0, 0);
    fb_move_cursor_xy(1,Terminal_Y);
    printChar(0, Terminal_Y, '>');
    fb_write_xy(Terminal_Buffer, TERMINAL_Buffer_Size, Terminal_Buffer_Pointer, 1, Terminal_Y);
}

void terminal_console(){
    Terminal_Buffer[Terminal_Buffer_Pointer] = keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1];
    
    printChar(0, Terminal_Y, '>');
    
    printChar(Terminal_Buffer_Pointer+1, Terminal_Y, Terminal_Buffer[Terminal_Buffer_Pointer]);
    
    Terminal_Buffer_Pointer+=1;
    fb_move_cursor_xy(Terminal_Buffer_Pointer+1, Terminal_Y);
}

void terminal_handler(){
    
    STR_INSERT("Testing Buffer System", Terminal_OUT_Buffer, 20, 0);
    STR_INSERT("Command Buffer", Terminal_Buffer, 16, 0);
    if(keyboard_KEYBUFFER_POINTER-1 != previousKYBD_pointer){
        //printChar(keyboard_KEYBUFFER_POINTER-1, Terminal_Y, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1]);
        terminal_console();
        previousKYBD_pointer = keyboard_KEYBUFFER_POINTER-1;
    }
    /*
    Terminal_OUT_Buffer[0] = 'A';
    terminal_renderer();*/
}