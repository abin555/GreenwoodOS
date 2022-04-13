#include "keyboard.h"
#include "frame_buffer.h"
#include "ascii_tables.h"

unsigned char prev_Scancode = 0;
unsigned int keyboard_KEYBUFFER_POINTER = 0;
unsigned int keyboard_ascii_pointer = 0;
bool _keyboard_disable = 1;
bool KYBRD_SHIFT = false;

uint8_t keyboard_ctrl_read_status() {
    return inb(KEYBOARD_CTRL_STATS_REG);
}

void keyboard_ctrl_send_cmd(uint8_t cmd){
    while (1){
        if ((keyboard_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0){
            break;
        }
    }
    outb(KEYBOARD_CTRL_CMD_REG, cmd);
}

uint8_t keyboard_enc_read_buf() {
    return inb(KEYBOARD_ENC_INPUT_BUF);
}

void keyboard_enc_send_cmd(uint8_t cmd){
    while(1){
        if((keyboard_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0){
            break;
        }
    }

    outb(KEYBOARD_ENC_CMD_REG, cmd);
}

#define KEYBOARD_ENC_CMD_SET_LED 0xED
#define KYBRD_CTRL_CMD_SELF_TEST 0xAA

void keyboard_set_leds (bool num, bool caps, bool scroll){
    uint8_t data = 0;

    data = (scroll) ? (data | 1) : (data & 1);
    data = (num) ? (num | 2) : (num & 2);
	data = (caps) ? (num | 4) : (num & 4);

    keyboard_enc_send_cmd(KEYBOARD_ENC_CMD_SET_LED);
    keyboard_enc_send_cmd(data);
}

bool keyboard_self_test(){
    keyboard_ctrl_send_cmd(KYBRD_CTRL_CMD_SELF_TEST);

    while(1){
        if(keyboard_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_OUT_BUF){
            break;
        }
    }

    return (keyboard_enc_read_buf() == 0x55) ? true : false;
}

void keyboard_disable(){
    keyboard_ctrl_send_cmd(0xAD);//Disable Keyboard Command = 0xAD
    _keyboard_disable = 1;
}

void keyboard_enable(){
    keyboard_ctrl_send_cmd(0xAE);//Enable Keyboard Command = 0xAE
    _keyboard_disable = 0;
}

int keyboard_keyread(){
    keyboard_enc_send_cmd(0xF4);
    while(1){
        if(keyboard_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_OUT_BUF){
            break;
        }
    }
    return keyboard_enc_read_buf();
}

void keyboard_flag_handler(unsigned char scan_code){
    switch(scan_code){
        case 0x2a:
        KYBRD_SHIFT = true;
        keyboard_set_leds(false, true, false);
        break;
        case 0xaa:
        KYBRD_SHIFT = false;
        keyboard_set_leds(false,false,false);
        break;
        
        case 0x1D:
        //software_int();
        software_interrupt(1);
        break;

        case 0x5B:
        software_interrupt(4);
        break;

        case 0xD3:
        for(int i = 0; i < 2*KEYBOARD_BUFFERSIZE; i++){
            keyboard_KEYBUFFER[i] = 0xaa;
            keyboard_ASCIIBuffer[i] = 'X';
        }
        break;
    }
}

char convertascii(unsigned char scan_code){
    if(kbd_US[scan_code] != 0){
        if(KYBRD_SHIFT){
            return kbd_US_shift[scan_code];
        }
        else{
            return kbd_US[scan_code];
        }
    }
    return 0;
}

void keyboard_handle_interrupt(){
    unsigned char scan_code;

    scan_code = keyboard_enc_read_buf();
    if(scan_code){
        if(kbd_US[scan_code] != 0){
            if(KYBRD_SHIFT){
                keyboard_ASCIIBuffer[keyboard_ascii_pointer] = kbd_US_shift[scan_code];
            }
            else{
                keyboard_ASCIIBuffer[keyboard_ascii_pointer] = kbd_US[scan_code];
            }
            keyboard_ascii_pointer++;
        }
        else{
            keyboard_flag_handler(scan_code);
        }
        keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER] = scan_code;
        keyboard_KEYBUFFER_POINTER++;
    }
    if(keyboard_ascii_pointer > KEYBOARD_BUFFERSIZE){
        keyboard_ascii_pointer = 0;
    }
    if(keyboard_KEYBUFFER_POINTER > KEYBOARD_BUFFERSIZE){
        keyboard_KEYBUFFER_POINTER = 0;
    }
}
