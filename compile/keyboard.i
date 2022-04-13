# 1 "keyboard.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "keyboard.c"
# 1 "./include/keyboard.h" 1



# 1 "./include/io.h" 1



extern void outb(unsigned short port, unsigned char data);

extern unsigned char inb(unsigned short pos);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
# 5 "./include/keyboard.h" 2
# 1 "./include/stdint.h" 1
# 6 "./include/keyboard.h" 2




enum KEYBOARD_ENCODER_IO{
    KEYBOARD_ENC_INPUT_BUF = 0x60,
    KEYBOARD_ENC_CMD_REG = 0x60
};

enum KEYBOARD_CTRL_IO{
    KEYBOARD_CTRL_STATS_REG = 0x64,
    KEYBOARD_CTRL_CMD_REG = 0x64
};

enum KYBRD_CTRL_STATS_MASK {

 KYBRD_CTRL_STATS_MASK_OUT_BUF = 1,
 KYBRD_CTRL_STATS_MASK_IN_BUF = 2,
 KYBRD_CTRL_STATS_MASK_SYSTEM = 4,
 KYBRD_CTRL_STATS_MASK_CMD_DATA = 8,
 KYBRD_CTRL_STATS_MASK_LOCKED = 0x10,
 KYBRD_CTRL_STATS_MASK_AUX_BUF = 0x20,
 KYBRD_CTRL_STATS_MASK_TIMEOUT = 0x40,
 KYBRD_CTRL_STATS_MASK_PARITY = 0x80
};

_Bool KYBRD_CAPS_LOCK;
_Bool KYBRD_SHIFT;

char keyboard_ctrl_read_status ();

void keyboard_ctrl_send_cmd(char cmd);

char keyboard_enc_read_buf();

void keyboard_enc_send_cmd(char cmd);

void keyboard_set_leds(_Bool num, _Bool caps, _Bool scroll);

_Bool keyboard_self_test();

void keyboard_disable();
void keyboard_enable();

int keyboard_keyread();

void keyboard_flag_handler(unsigned char scan_code);
void keyboard_handle_interrupt();

char convertascii(unsigned char scan_code);

unsigned char keyboard_KEYBUFFER[100];
char keyboard_ASCIIBuffer[100];

unsigned int keyboard_KEYBUFFER_POINTER;
unsigned int keyboard_ascii_pointer;

unsigned char prev_Scancode;
unsigned char char_scancode;
# 2 "keyboard.c" 2
# 1 "./include/frame_buffer.h" 1
# 12 "./include/frame_buffer.h"
char *fb;
int fb_cursor;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

void printChar(unsigned int x, unsigned int y, char c);

void fb_clear(char c, unsigned char fg, unsigned char bg);

int fb_write(char *buf, unsigned int len);
int fb_write_start(char *buf, unsigned int len, unsigned int start);
void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y);
# 33 "./include/frame_buffer.h"
void fb_move_cursor(unsigned short pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);
# 3 "keyboard.c" 2
# 1 "./include/ascii_tables.h" 1


# 1 "./include/keyboard.h" 1
# 4 "./include/ascii_tables.h" 2


char kbd_US[256];
char kbd_US_shift[256];
# 4 "keyboard.c" 2

unsigned char prev_Scancode = 0;
unsigned int keyboard_KEYBUFFER_POINTER = 0;
unsigned int keyboard_ascii_pointer = 0;
_Bool _keyboard_disable = 1;
_Bool KYBRD_SHIFT = 0;

char keyboard_ctrl_read_status() {
    return inb(KEYBOARD_CTRL_STATS_REG);
}

void keyboard_ctrl_send_cmd(char cmd){
    while (1){
        if ((keyboard_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0){
            break;
        }
    }
    outb(KEYBOARD_CTRL_CMD_REG, cmd);
}

char keyboard_enc_read_buf() {
    return inb(KEYBOARD_ENC_INPUT_BUF);
}

void keyboard_enc_send_cmd(char cmd){
    while(1){
        if((keyboard_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0){
            break;
        }
    }

    outb(KEYBOARD_ENC_CMD_REG, cmd);
}




void keyboard_set_leds (_Bool num, _Bool caps, _Bool scroll){
    char data = 0;

    data = (scroll) ? (data | 1) : (data & 1);
    data = (num) ? (num | 2) : (num & 2);
 data = (caps) ? (num | 4) : (num & 4);

    keyboard_enc_send_cmd(0xED);
    keyboard_enc_send_cmd(data);
}

_Bool keyboard_self_test(){
    keyboard_ctrl_send_cmd(0xAA);

    while(1){
        if(keyboard_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_OUT_BUF){
            break;
        }
    }

    return (keyboard_enc_read_buf() == 0x55) ? 1 : 0;
}

void keyboard_disable(){
    keyboard_ctrl_send_cmd(0xAD);
    _keyboard_disable = 1;
}

void keyboard_enable(){
    keyboard_ctrl_send_cmd(0xAE);
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
        KYBRD_SHIFT = 1;
        keyboard_set_leds(0, 1, 0);
        break;
        case 0xaa:
        KYBRD_SHIFT = 0;
        keyboard_set_leds(0,0,0);
        break;

        case 0x1D:

        software_interrupt(1);
        break;

        case 0x5B:
        software_interrupt(4);
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
    if(keyboard_ascii_pointer > 100){
        keyboard_ascii_pointer = 0;
    }
    if(keyboard_KEYBUFFER_POINTER > 100){
        keyboard_KEYBUFFER_POINTER = 0;
    }
}
