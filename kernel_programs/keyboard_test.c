#include "keyboard_test.h"

void KYBRD_DEBUG_DISPLAY(){
    decodeData(decode, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1], 8, 0);
    decodeHex(decode, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1], 8, 8);
    decodeData(decode, keyboard_ASCIIBuffer[keyboard_ascii_pointer-1], 8, 11);
    decodeHex(decode, keyboard_ASCIIBuffer[keyboard_ascii_pointer-1], 8, 19);
    decodeHex(decode, keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1], 8, 27);

    fb_move_cursor(0);
    fb_write(keyboard_ASCIIBuffer, 100);
    fb_move_cursor(160);
    fb_write(decode, 8);
    fb_move_cursor_xy(0,3);
    fb_write_start(decode, 40, 0);
}