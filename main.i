# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"
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
# 2 "main.c" 2
# 1 "./include/keyboard.h" 1



# 1 "./include/io.h" 1
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

unsigned char keyboard_KEYBUFFER[0xFF];
char keyboard_ASCIIBuffer[0xFF];

unsigned int keyboard_KEYBUFFER_POINTER;
unsigned int keyboard_ascii_pointer;

unsigned char prev_Scancode;
unsigned char char_scancode;
# 3 "main.c" 2
# 1 "./include/interrupts.h" 1



unsigned char SYS_MODE;
# 1 "./include/system_calls.h" 1
# 6 "./include/interrupts.h" 2

struct IDT
{
 unsigned short size;
 unsigned int address;
} __attribute__((packed));

struct IDTDescriptor {


 unsigned short offset_low;
 unsigned short segment_selector;


 unsigned char reserved;
 unsigned char type_and_attr;
 unsigned short offset_high;
} __attribute__((packed));

struct cpu_state {

 unsigned int eax;
 unsigned int ebx;
 unsigned int ecx;
 unsigned int edx;
 unsigned int ebp;
 unsigned int esi;
 unsigned int edi;
} __attribute__((packed));

struct stack_state {

 unsigned int error_code;
 unsigned int eip;
 unsigned int cs;
 unsigned int eflags;
} __attribute__((packed));

void interrupt_install_idt();
extern void int_handler_33();
extern void int_handler_34();
extern void int_handler_35();
extern void int_handler_128();

void load_idt(unsigned int idt_address);

void KERNEL_INTERRUPT();
void SYS_CALL(
 struct cpu_state cpu;
);


void interrupt_handler(
    struct cpu_state cpu,
    unsigned int interrupt,
    struct stack_state stack);
# 4 "main.c" 2
# 1 "./include/frame_buffer.h" 1
# 12 "./include/frame_buffer.h"
char *fb;
unsigned char *Buffer;
int fb_cursor;
unsigned char FG;
unsigned char BG;

void screen_init();

void fb_putPixel(int x, int y, unsigned int COLOR);

void fb_set_color(unsigned char fg, unsigned char bg);

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

void printChar(unsigned int x, unsigned int y, char c);

void fb_clear(char c, unsigned char fg, unsigned char bg);

int fb_write(char *buf, unsigned int len);
int fb_write_start(char *buf, unsigned int len, unsigned int start);
void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y);
# 42 "./include/frame_buffer.h"
void fb_move_cursor(unsigned short pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);
# 5 "main.c" 2
# 1 "./include/serial.h" 1




void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void serial_write(unsigned int com, char b);
int init_serial();
# 6 "main.c" 2
# 1 "./include/string.h" 1



# 1 "./include/ascii_tables.h" 1


# 1 "./include/keyboard.h" 1
# 4 "./include/ascii_tables.h" 2


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
# 7 "main.c" 2
# 1 "./include/terminal.h" 1



# 1 "./include/frame_buffer.h" 1
# 5 "./include/terminal.h" 2


# 1 "./include/string.h" 1
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
# 8 "main.c" 2
# 1 "./include/keyboard_test.h" 1
# 10 "./include/keyboard_test.h"
char decode[500];

void KYBRD_DEBUG_DISPLAY();
# 9 "main.c" 2

extern void load_gdt();

struct gdt {
  unsigned int address;
  unsigned short size;
} __attribute__((packed));


int sum_of_three(int arg1, int arg2, int arg3){
 return arg1 + arg2 + arg3;
}

void PROGRAMA();

int main(){
  load_gdt();
  interrupt_install_idt();
  screen_init();

  unsigned int i = 0;
  while(1){
    fb_putPixel(i*2, 0, 0xFFFFFF-i);
    i++;
  }
  return 0;
  fb_clear(' ', 15, 0);

  while(1){
    switch(SYS_MODE){
      case 1:
        terminal_handler();
        break;
      case 4:
        KYBRD_DEBUG_DISPLAY();
        break;
      default:
        KYBRD_DEBUG_DISPLAY();

    }
  }
  return 0;
}
