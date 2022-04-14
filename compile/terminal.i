# 1 "kernel_programs/terminal.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "kernel_programs/terminal.c"
# 1 "./include/terminal.h" 1



# 1 "./include/frame_buffer.h" 1
# 12 "./include/frame_buffer.h"
char *fb;
int fb_cursor;
unsigned char FG;
unsigned char BG;

void fb_set_color(unsigned char fg, unsigned char bg);

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

void printChar(unsigned int x, unsigned int y, char c);

void fb_clear(char c, unsigned char fg, unsigned char bg);

int fb_write(char *buf, unsigned int len);
int fb_write_start(char *buf, unsigned int len, unsigned int start);
void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y);
# 37 "./include/frame_buffer.h"
void fb_move_cursor(unsigned short pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);
# 5 "./include/terminal.h" 2
# 1 "./include/io.h" 1



extern void outb(unsigned short port, unsigned char data);

extern unsigned char inb(unsigned short pos);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
# 6 "./include/terminal.h" 2
# 1 "./include/keyboard.h" 1




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
# 7 "./include/terminal.h" 2
# 1 "./include/string.h" 1



# 1 "./include/ascii_tables.h" 1





char kbd_US[256];
char kbd_US_shift[256];
# 5 "./include/string.h" 2

char STR_edit[128];

void STR_INSERT(char *in_str, char *out_str, int len, int write_index);

void decodeData(char *Buffer, int in, int len, int start);

void decodeHex(char *Buffer, int in, int len, int start);
# 8 "./include/terminal.h" 2





char Terminal_Buffer[80];
char Terminal_OUT_Buffer[80*40];

char Terminal_Arguments[80];

void terminal_interpret();

void write_terminal(char *Buffer, int len, int start, int x, int y);
void terminal_enter();

void terminal_renderer();
void terminal_console();
void terminal_handler();
# 2 "kernel_programs/terminal.c" 2


int Terminal_OUT_pointer = 0;
unsigned short Terminal_Buffer_Pointer = 0;
unsigned int previousASCII_pointer = 0;
unsigned int previousKEY_pointer = 0;

void terminal_renderer(){
    fb_clear(' ', 15, 0);
    fb_move_cursor_xy(1,20);
    printChar(0, 20, '>');
    fb_write_xy(Terminal_Buffer, 80, Terminal_Buffer_Pointer, 1, 20);
}

void terminal_console(){
    Terminal_Buffer[Terminal_Buffer_Pointer] = keyboard_ASCIIBuffer[keyboard_ascii_pointer-1];
    printChar(Terminal_Buffer_Pointer+1, 20, Terminal_Buffer[Terminal_Buffer_Pointer]);

    fb_move_cursor_xy(Terminal_Buffer_Pointer+2, 20);

    Terminal_Buffer_Pointer++;
    fb_set_color(2, 0);
    printChar(0, 20, '>');
    fb_set_color(15, 0);
}

void terminal_interpret(){
    int found_splits = 0;
    for(int i = 0; i < 80; i++){
        Terminal_Arguments[i] = 0;
        if(Terminal_Buffer[i] == ' '){
            Terminal_Arguments[found_splits] = i;
            found_splits++;
        }
    }

    fb_write_xy(Terminal_Buffer, Terminal_Arguments[0], 0, 2, 21);
}

void terminal_enter(){
    terminal_interpret();


    STR_INSERT(Terminal_Buffer, Terminal_OUT_Buffer, 80, Terminal_OUT_pointer);
    fb_write_xy(Terminal_OUT_Buffer, 80*19, Terminal_OUT_pointer, 0, 0);
    Terminal_OUT_pointer += 80 + (80 % 80);
    Terminal_Buffer_Pointer = 0;

    for(int i = 0; i < 80; i++){
        Terminal_Buffer[i] = 0;
        printChar(i+1, 20, ' ');
    }
    fb_move_cursor_xy(1, 20);
    fb_set_color(2, 0);
    printChar(0, 20, '>');
    fb_set_color(15, 0);
}

void terminal_handler(){




    if(keyboard_ascii_pointer-1 != previousASCII_pointer && Terminal_Buffer_Pointer < 80){

        terminal_console();
        previousASCII_pointer = keyboard_ascii_pointer-1;
    }
    else if(keyboard_KEYBUFFER_POINTER != previousKEY_pointer){
        switch(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1]){
            case 0x0E:
            if(Terminal_Buffer_Pointer != 0){
                fb_move_cursor_xy(Terminal_Buffer_Pointer, 20);
                Terminal_Buffer[Terminal_Buffer_Pointer-1] = 0;
                printChar(Terminal_Buffer_Pointer, 20, ' ');
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
                fb_move_cursor_xy(Terminal_Buffer_Pointer, 20);
                Terminal_Buffer_Pointer--;
            }
            break;
            case 0xCD:
            if(Terminal_Buffer_Pointer < 80){
                Terminal_Buffer_Pointer++;
                fb_move_cursor_xy(Terminal_Buffer_Pointer, 20);
            }
            break;
            case 0xC8:
            for(int i = 0; i < 80; i++){
                Terminal_Buffer[i] = Terminal_OUT_Buffer[Terminal_OUT_pointer-80+i];
                printChar(i+1, 20, Terminal_Buffer[i]);
                if(Terminal_Buffer[i] == 0){
                    fb_move_cursor_xy(i+1, 20);
                    Terminal_Buffer_Pointer=i;
                    break;
                }
            }
            break;
        }
        previousKEY_pointer = keyboard_KEYBUFFER_POINTER;
    }
    else if(Terminal_Buffer_Pointer > 0b100000000000000){
        Terminal_Buffer_Pointer = 0;
    }
    decodeData(STR_edit, (Terminal_Buffer_Pointer << 1), 16, 0);
    fb_write_xy(STR_edit, 16, 0, 0, 23);


}
