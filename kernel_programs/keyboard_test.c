#include "keyboard_test.h"

void KYBRD_DEBUG_DISPLAY(){
    decodeData(decode, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1], 8, 0);
    decodeHex(decode, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1], 8, 8);
    decodeData(decode, keyboard_ASCIIBuffer[keyboard_ascii_pointer-1], 8, 11);
    decodeHex(decode, keyboard_ASCIIBuffer[keyboard_ascii_pointer-1], 8, 19);
    decodeHex(decode, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1], 8, 27);

    fb_move_cursor(0);
    fb_write(keyboard_ASCIIBuffer, KEYBOARD_BUFFERSIZE);
    fb_move_cursor_xy(0,5);
    fb_write(decode, 8);
    fb_move_cursor_xy(0,6);
    fb_write_start(decode, 40, 0);

    unsigned short y_shift = 3;
    for(int i = 0; i < 2*KEYBOARD_BUFFERSIZE; i++){
        decodeHex(decode, keyboard_KEYBUFFER[i], 8, 3*i+50);
        for(int c = 0 ; c < 3; c++){
            if(c != 2){
                printChar(i+c, 5+y_shift, decode[i+c+50]);
            }
            printChar(i+c, 5+y_shift, decode[i+c+50]);
        }
    }
}