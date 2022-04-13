# 1 "ascii_tables.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "ascii_tables.c"
# 1 "./include/ascii_tables.h" 1


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
# 4 "./include/ascii_tables.h" 2


char kbd_US[256];
char kbd_US_shift[256];
# 2 "ascii_tables.c" 2

char kbd_US [256] =
{
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
  0,
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
  '*',
    0,
  ' ',
    0,
    0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0,
    0,
    0,
  '-',
    0,
    0,
    0,
  '+',
    0,
    0,
    0,
    0,
    0,
    0, 0, 0,
    0,
    0,
    0,
};
char kbd_US_shift [256] =
{
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
  0,
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
    0,
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
  '*',
    0,
  ' ',
    0,
    0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0,
    0,
    0,
  '-',
    0,
    0,
    0,
  '+',
    0,
    0,
    0,
    0,
    0,
    0, 0, 0,
    0,
    0,
    0,
};
