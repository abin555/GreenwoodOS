#include "terminal.h"

int Terminal_OUT_pointer = 0;
unsigned short Terminal_Buffer_Pointer = 0;
unsigned int previousASCII_pointer = 0;
unsigned int previousKEY_pointer = 0;

#ifndef Terminal_Y
int Terminal_Y = 20;
#endif


void terminal_renderer(){
    fb_clear(' ', FB_WHITE, FB_BLACK);
    fb_move_cursor_xy(1,Terminal_Y);
    printChar(0, Terminal_Y, '>');
    fb_write_xy(Terminal_Buffer, TERMINAL_Buffer_Size, Terminal_Buffer_Pointer, 1, Terminal_Y);
}

void terminal_console(){
    Terminal_Buffer[Terminal_Buffer_Pointer] = keyboard_ASCIIBuffer[keyboard_ascii_pointer-1];
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
    Terminal_OUT_pointer+=80;
}

void terminal_interpret(){
    int found_splits = 0;


    
    for(int i = 0; i < TERMINAL_Buffer_Size; i++){
        if(Terminal_Buffer[i] == TERMINAL_SPLIT){
            Terminal_Arguments[found_splits] = i;
            found_splits++;
        }
    }
    Terminal_Arguments[found_splits+1] = -1;
    

    if(STR_Compare(Terminal_Buffer, "PRINT", 0, Terminal_Arguments[0]-1) == 4){
        
        printChar(79, 0, 'P');
        fb_write_cell(Terminal_OUT_pointer, '-', FB_RED, FB_BLACK);
        fb_write_xy(Terminal_Buffer, TERMINAL_Buffer_Size-Terminal_Arguments[0], Terminal_Arguments[0]+1, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=80;
        
       //terminal_output(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_OUT_pointer+1);
    }

    fb_write_xy(Terminal_Buffer, Terminal_Arguments[0], 0, 2, 21);

    decodeData(STR_edit, (Terminal_Arguments[0] << 1), 8, 0);
    fb_write_xy(STR_edit, 8, 0, 0, 22);
    decodeData(STR_edit, (Terminal_Arguments[1] << 1), 8, 0);
    fb_write_xy(STR_edit, 8, 0, 9, 22);

    decodeData(STR_edit, STR_Compare(Terminal_Buffer, "PRINT", 0, Terminal_Arguments[0]-1) << 1, 8, 0);
    fb_write_xy(STR_edit, 8, 0, 18, 22);
}

void terminal_enter(){    


    STR_INSERT(Terminal_Buffer, Terminal_OUT_Buffer, TERMINAL_Buffer_Size, Terminal_OUT_pointer);
    fb_write_xy(Terminal_OUT_Buffer, TERMINAL_Buffer_Size, Terminal_OUT_pointer, Terminal_OUT_pointer, 0);
    Terminal_OUT_pointer += TERMINAL_Buffer_Size + (TERMINAL_Buffer_Size % 80);
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

    if(Terminal_OUT_pointer > (Terminal_Y-1) * 80){
        Terminal_OUT_pointer = 0;
        fb_clear(' ', FB_WHITE, FB_BLACK);
    }
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
                Terminal_Buffer[i] = Terminal_OUT_Buffer[Terminal_OUT_pointer-80+i];
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
    decodeData(STR_edit, (Terminal_Buffer_Pointer << 1), 16, 0);
    fb_write_xy(STR_edit, 16, 0, 0, 23);
}