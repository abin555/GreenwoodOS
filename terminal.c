#include "terminal.h"

char Terminal_Buffer_Pointer = 0;
unsigned short Terminal_OUT_pointer = 0;

void terminal_renderer(){
    fb_clear();
    fb_print_buf(Terminal_OUT_Buffer, TERMINAL_Buffer_Size*40, Terminal_OUT_pointer, 0, 0);
    printChar(0, 40, '>');
    fb_print_buf(Terminal_Buffer, TERMINAL_Buffer_Size, Terminal_Buffer_Pointer, 1, 40);
}

void terminal_handler(){
    Terminal_OUT_Buffer[0] = "Testing my buffer system";
    Terminal_Buffer[0] = "New Command?";
    terminal_renderer();
    while(1){}
}