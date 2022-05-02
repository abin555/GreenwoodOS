#include "terminal.h"

int Terminal_OUT_pointer = 0;
unsigned short Terminal_Buffer_Pointer = 0;
unsigned int previousASCII_pointer = 0;
unsigned int previousKEY_pointer = 0;

#ifndef Terminal_Y
int Terminal_Y = 92;
#endif

void terminal_memory_view(){

}

void terminal_renderer(){
    fb_clear(FB_BLACK);
    fb_move_cursor_xy(1,Terminal_Y);
    printChar(0, Terminal_Y, '>');
    fb_write_xy(Terminal_Buffer, TERMINAL_Buffer_Size, Terminal_Buffer_Pointer, 1, Terminal_Y);
}

void terminal_console(){
    if(keyboard_ascii_pointer-1 != 0xFFFFFFFF){
        Terminal_Buffer[Terminal_Buffer_Pointer] = keyboard_ASCIIBuffer[keyboard_ascii_pointer-1];
    }
    else{
        Terminal_Buffer[Terminal_Buffer_Pointer] = keyboard_ASCIIBuffer[KEYBOARD_BUFFERSIZE-1];
    }
    printChar(Terminal_Buffer_Pointer+1, Terminal_Y, Terminal_Buffer[Terminal_Buffer_Pointer]);
    
    fb_move_cursor_xy(Terminal_Buffer_Pointer+2, Terminal_Y);

    Terminal_Buffer_Pointer++;
    fb_set_color(FB_GREEN, FB_BLACK);
    printChar(0, Terminal_Y, '>');
    fb_set_color(FB_WHITE, FB_BLACK);
}

void terminal_output(char *Buffer, int start, int end){
    fb_write_cell(Terminal_OUT_pointer, '-', FB_RED, FB_BLACK);
    fb_write_xy(Buffer, end-start, start, end, 0);
    Terminal_OUT_pointer+=fb_terminal_w;
}

int terminal_compare(char *Buffer, int start, int end, int len){
    if(STR_Compare(Terminal_Buffer, Buffer, start, end) == len){
        return 1;
    }
    else{
        return 0;
    }
}

void terminal_interpret(){
    int found_splits = 0;

    for(int i = 0; i < TERMINAL_Buffer_Size; i++){
        if(Terminal_Buffer[i] == TERMINAL_SPLIT || Terminal_Buffer[i] == 0){
            Terminal_Arguments[found_splits] = i;
            found_splits++;
        }
        if(Terminal_Buffer[i] == 0){}
    }
    Terminal_Arguments[found_splits+1] = -1;
    

    if(terminal_compare("print", 0, Terminal_Arguments[0], 5)){
        
        printChar(79, 0, 'P');
        fb_write_cell(Terminal_OUT_pointer, '-', FB_RED, FB_BLACK);
        fb_write_xy(Terminal_Buffer, TERMINAL_Buffer_Size-Terminal_Arguments[0], Terminal_Arguments[0]+1, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=fb_terminal_w;
        
       //terminal_output(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_OUT_pointer+1);
    }
    if(terminal_compare("peek", 0, Terminal_Arguments[0], 4)){
        printChar(79, 0, 'M');
        fb_write_cell(Terminal_OUT_pointer, '-', FB_RED, FB_BLACK);

        //int hex = hexToQuad(Terminal_Buffer[Terminal_Arguments[1]-1]);
        
        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        unsigned int data = ReadMem(addr);
        decodeHex(STR_edit, data, 32, 0);
        fb_write_xy(STR_edit, 9, 0, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=fb_terminal_w;
    }
    if(terminal_compare("poke", 0, Terminal_Arguments[0], 4)){
        fb_write_cell(Terminal_OUT_pointer, '-', FB_RED, FB_BLACK);
        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        unsigned int val = encodeHex(Terminal_Buffer, Terminal_Arguments[1]+1, Terminal_Arguments[2]);
        WriteMem(addr, val);
        decodeHex(STR_edit, addr, 32, 0);
        decodeHex(STR_edit, val, 32, 9);
        STR_edit[9] = ' ';
        fb_write_xy(STR_edit, 18, 0, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=fb_terminal_w;
    }
    if(terminal_compare("load", 0, Terminal_Arguments[0], 4)){
        WriteMem((int) externalProgram, 0x000001b8);
        WriteMem((int) externalProgram + 0x05, 0x0000efbb);
        WriteMem((int) externalProgram + 0x0a, 0x000054b9);
        WriteMem((int) externalProgram + 0x0f, 0x000080cd);
        WriteMem((int) externalProgram + 0x11, 0x000000c3);
    }
    if(terminal_compare("exec", 0, Terminal_Arguments[0], 4)){
        PROGA();
    }
    if(terminal_compare("wipe", 0, Terminal_Arguments[0], 4)){
        for(int i = 0; i < 50; i++){
            WriteMem((int)externalProgram+4*i, 0x00000000);
        }
    }
    if(terminal_compare("check", 0, Terminal_Arguments[0], 5)){
        decodeHex(STR_edit, (int) externalProgram, 32, 0);
        fb_write_xy(STR_edit, 9, 0, Terminal_OUT_pointer+1, 0);
        fb_write_cell(Terminal_OUT_pointer, '-', FB_RED, FB_BLACK);
        Terminal_OUT_pointer+=fb_terminal_w;
    }
    if(terminal_compare("swap_PROG", 0, Terminal_Arguments[0], 9)){
        if((int) externalProgram == 0x01000000){
            externalProgram = (unsigned int *) 0x02000000;
        }
        else if((int) externalProgram == 0x02000000){
            externalProgram = (unsigned int *) 0x01000000;
        }
    }
    if(terminal_compare("set_PROG", 0, Terminal_Arguments[0], 8)){
        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        externalProgram = (unsigned int *) addr;
    }
    if(terminal_compare("clear", 0, Terminal_Arguments[0], 5)){
        fb_clear(0);
        Terminal_OUT_pointer = 0;
    }
    if(terminal_compare("pong", 0, Terminal_Arguments[0], 4)){
        pong();
    }
}

void terminal_enter(){    


    STR_INSERT(Terminal_Buffer, Terminal_OUT_Buffer, TERMINAL_Buffer_Size, Terminal_OUT_pointer);
    fb_write_xy(Terminal_OUT_Buffer, TERMINAL_Buffer_Size, Terminal_OUT_pointer, Terminal_OUT_pointer, 0);
    Terminal_OUT_pointer += TERMINAL_Buffer_Size + (TERMINAL_Buffer_Size % fb_terminal_w);
    Terminal_Buffer_Pointer = 0;


    
    terminal_interpret();
    for(int i = 0; i < TERMINAL_Buffer_Size; i++){
        Terminal_Buffer[i] = 0;
        printChar(i+1, Terminal_Y, ' ');
    }
    fb_move_cursor_xy(1, Terminal_Y);
    fb_set_color(FB_GREEN, FB_BLACK);
    printChar(0, Terminal_Y, '>');
    fb_set_color(FB_WHITE, FB_BLACK);

    if(Terminal_OUT_pointer > (Terminal_Y-1) * fb_terminal_w){
        Terminal_OUT_pointer = 0;
        fb_clear(FB_BLACK);
    }
}

void terminal_handler(){
    /*
    STR_INSERT("Testing Buffer System", Terminal_OUT_Buffer, 20, 0);
    STR_INSERT("Command Buffer", Terminal_Buffer, 16, 0);
    */

    if(keyboard_ascii_pointer != previousASCII_pointer && Terminal_Buffer_Pointer < TERMINAL_Buffer_Size){
        //printChar(keyboard_KEYBUFFER_POINTER-1, Terminal_Y, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1]);
        terminal_console();
        previousASCII_pointer = keyboard_ascii_pointer;
    }
    else if(keyboard_KEYBUFFER_POINTER != previousKEY_pointer){
        //printChar(0,0, 'T');
        switch(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1]){
            case 0x0E:
            if(Terminal_Buffer_Pointer != 0){
                fb_move_cursor_xy(Terminal_Buffer_Pointer, Terminal_Y);
                Terminal_Buffer[Terminal_Buffer_Pointer-1] = 0;
                printChar(Terminal_Buffer_Pointer, Terminal_Y, ' ');
                if(Terminal_Buffer_Pointer > 0){
                    Terminal_Buffer_Pointer -= 1;
                }
            }
            break;
            case 0x9C:
            terminal_enter();
            break;
            case 0xCB:
            if(Terminal_Buffer_Pointer != 0){
                fb_move_cursor_xy(Terminal_Buffer_Pointer, Terminal_Y);
                Terminal_Buffer_Pointer--;
            }
            break;
            case 0xCD:
            if(Terminal_Buffer_Pointer < TERMINAL_Buffer_Size){
                Terminal_Buffer_Pointer++;
                fb_move_cursor_xy(Terminal_Buffer_Pointer, Terminal_Y);
            }
            break;
            case 0xC8:
            for(int i = 0; i < TERMINAL_Buffer_Size; i++){
                Terminal_Buffer[i] = Terminal_OUT_Buffer[Terminal_OUT_pointer-fb_terminal_w+i];
                printChar(i+1, Terminal_Y, Terminal_Buffer[i]);
                if(Terminal_Buffer[i] == 0){
                    fb_move_cursor_xy(i+1, Terminal_Y);
                    Terminal_Buffer_Pointer=i;
                    break;
                }
            }
            break;
        }
        previousKEY_pointer = keyboard_KEYBUFFER_POINTER;
    }
}