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



# 1 "./include/gcc_stdint.h" 1
# 34 "./include/gcc_stdint.h"
typedef signed char int8_t;


typedef short int int16_t;


typedef int int32_t;


typedef long long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef unsigned int uint32_t;


typedef long long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef long long unsigned int uint_least64_t;



typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long long unsigned int uint_fast64_t;




typedef int intptr_t;


typedef unsigned int uintptr_t;




typedef long long int intmax_t;
typedef long long unsigned int uintmax_t;
# 5 "./include/io.h" 2

extern void outb(unsigned short port, unsigned char data);

extern unsigned char inb(unsigned short pos);

void WriteMem(uint32_t Address, uint32_t Value);
uint32_t ReadMem(uint32_t Address);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char *INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
extern void PROGA();

extern unsigned int *externalProgram;
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

unsigned char keyboard_KEYBUFFER[0xFF];
char keyboard_ASCIIBuffer[0xFF];

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

int STR_Compare(char *elem1, char *elem2, int start, int end);

void STR_INSERT(char *in_str, char *out_str, int len, int write_index);

void decodeData(char *Buffer, int in, int len, int start);

void decodeHex(char *Buffer, int in, int len, int start);

unsigned int encodeHex(char *Buffer, int start, int end);

char quadToHex(char quad);
char hexToQuad(char hex);
# 8 "./include/terminal.h" 2
# 16 "./include/terminal.h"
char Terminal_Buffer[80];
char Terminal_OUT_Buffer[80*40];

char Terminal_Arguments[80];

int terminal_compare(char *buffer, int start, int end, int len);

void terminal_interpret();
void terminal_output(char *Buffer, int start, int end);

void terminal_enter();

void terminal_renderer();
void terminal_console();
void terminal_handler();
# 2 "kernel_programs/terminal.c" 2

int Terminal_OUT_pointer = 0;
unsigned short Terminal_Buffer_Pointer = 0;
unsigned int previousASCII_pointer = 0;
unsigned int previousKEY_pointer = 0;


int Terminal_Y = 24;



void terminal_renderer(){
    fb_clear(' ', 15, 0);
    fb_move_cursor_xy(1,Terminal_Y);
    printChar(0, Terminal_Y, '>');
    fb_write_xy(Terminal_Buffer, 80, Terminal_Buffer_Pointer, 1, Terminal_Y);
}

void terminal_console(){
    if(keyboard_ascii_pointer-1 != 0xFFFFFFFF){
        Terminal_Buffer[Terminal_Buffer_Pointer] = keyboard_ASCIIBuffer[keyboard_ascii_pointer-1];
    }
    else{
        Terminal_Buffer[Terminal_Buffer_Pointer] = keyboard_ASCIIBuffer[0xFF -1];
    }
    printChar(Terminal_Buffer_Pointer+1, Terminal_Y, Terminal_Buffer[Terminal_Buffer_Pointer]);

    fb_move_cursor_xy(Terminal_Buffer_Pointer+2, Terminal_Y);

    Terminal_Buffer_Pointer++;
    fb_set_color(2, 0);
    printChar(0, Terminal_Y, '>');
    fb_set_color(15, 0);
}

void terminal_output(char *Buffer, int start, int end){
    fb_write_cell(Terminal_OUT_pointer, '-', 4, 0);
    fb_write_xy(Buffer, end-start, start, end, 0);
    Terminal_OUT_pointer+=80;
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

    for(int i = 0; i < 80; i++){
        if(Terminal_Buffer[i] == ' ' || Terminal_Buffer[i] == 0){
            Terminal_Arguments[found_splits] = i;
            found_splits++;
        }
        if(Terminal_Buffer[i] == 0){}
    }
    Terminal_Arguments[found_splits+1] = -1;


    if(terminal_compare("print", 0, Terminal_Arguments[0], 5)){

        printChar(79, 0, 'P');
        fb_write_cell(Terminal_OUT_pointer, '-', 4, 0);
        fb_write_xy(Terminal_Buffer, 80 -Terminal_Arguments[0], Terminal_Arguments[0]+1, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=80;


    }
    if(terminal_compare("peek", 0, Terminal_Arguments[0], 4)){
        printChar(79, 0, 'M');
        fb_write_cell(Terminal_OUT_pointer, '-', 4, 0);



        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        unsigned int data = ReadMem(addr);
        decodeHex(STR_edit, data, 32, 0);
        fb_write_xy(STR_edit, 9, 0, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=80;
    }
    if(terminal_compare("poke", 0, Terminal_Arguments[0], 4)){
        fb_write_cell(Terminal_OUT_pointer, '-', 4, 0);
        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        unsigned int val = encodeHex(Terminal_Buffer, Terminal_Arguments[1]+1, Terminal_Arguments[2]);
        WriteMem(addr, val);
        decodeHex(STR_edit, addr, 32, 0);
        decodeHex(STR_edit, val, 32, 9);
        STR_edit[9] = ' ';
        fb_write_xy(STR_edit, 18, 0, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=80;
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
        fb_write_cell(Terminal_OUT_pointer, '-', 4, 0);
        Terminal_OUT_pointer+=80;
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
}

void terminal_enter(){


    STR_INSERT(Terminal_Buffer, Terminal_OUT_Buffer, 80, Terminal_OUT_pointer);
    fb_write_xy(Terminal_OUT_Buffer, 80, Terminal_OUT_pointer, Terminal_OUT_pointer, 0);
    Terminal_OUT_pointer += 80 + (80 % 80);
    Terminal_Buffer_Pointer = 0;



    terminal_interpret();
    for(int i = 0; i < 80; i++){
        Terminal_Buffer[i] = 0;
        printChar(i+1, Terminal_Y, ' ');
    }
    fb_move_cursor_xy(1, Terminal_Y);
    fb_set_color(2, 0);
    printChar(0, Terminal_Y, '>');
    fb_set_color(15, 0);

    if(Terminal_OUT_pointer > (Terminal_Y-1) * 80){
        Terminal_OUT_pointer = 0;
        fb_clear(' ', 15, 0);
    }
}

void terminal_handler(){





    if(keyboard_ascii_pointer != previousASCII_pointer && Terminal_Buffer_Pointer < 80){

        terminal_console();
        previousASCII_pointer = keyboard_ascii_pointer;
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
            if(Terminal_Buffer_Pointer < 80){
                Terminal_Buffer_Pointer++;
                fb_move_cursor_xy(Terminal_Buffer_Pointer, Terminal_Y);
            }
            break;
            case 0xC8:
            for(int i = 0; i < 80; i++){
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
}
