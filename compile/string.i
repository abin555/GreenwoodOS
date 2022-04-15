# 1 "string.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "string.c"
# 1 "./include/string.h" 1



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

unsigned char keyboard_KEYBUFFER[20];
char keyboard_ASCIIBuffer[20];

unsigned int keyboard_KEYBUFFER_POINTER;
unsigned int keyboard_ascii_pointer;

unsigned char prev_Scancode;
unsigned char char_scancode;
# 4 "./include/ascii_tables.h" 2


char kbd_US[256];
char kbd_US_shift[256];
# 5 "./include/string.h" 2

char STR_edit[128];

int STR_Compare(char *elem1, char *elem2, int start, int end);

void STR_INSERT(char *in_str, char *out_str, int len, int write_index);

void decodeData(char *Buffer, int in, int len, int start);

void decodeHex(char *Buffer, int in, int len, int start);
# 2 "string.c" 2

int STR_Compare(char *elem1, char *elem2, int start, int end){
  int equal = 0;
  if(sizeof elem2 == end - start){
    for(int i = start; i < end; i++){
      if(elem1[i] == elem2[i]){
        equal++;
      }
    }
  }
  return equal;
}

void STR_INSERT(char *in_str, char *out_str, int len, int write_index){
    for(int index = 0; index < len; index++){
        out_str[index+write_index] = in_str[index];
    }
}

void decodeData(char *Buffer, int in, int len, int start){
  for(int x = 1; x < len; x++){
    if((in >> x) & 1){
      Buffer[len-x+start] = '1';
    }
    else{
      Buffer[len-x+start] = '0';
    }
  }
}

char quadtoHex(char quad){
  switch(quad){
    case 0x00:
      return '0';
      break;
    case 0x01:
      return '1';
      break;
    case 0x02:
      return '2';
      break;
    case 0x03:
      return '3';
      break;
    case 0x04:
      return '4';
      break;
    case 0x05:
      return '5';
      break;
    case 0x06:
      return '6';
      break;
    case 0x07:
      return '7';
      break;
    case 0x08:
      return '8';
      break;
    case 0x09:
      return '9';
      break;
    case 0x0a:
      return 'A';
      break;
    case 0x0b:
      return 'B';
      break;
    case 0x0c:
      return 'C';
      break;
    case 0x0d:
      return 'D';
      break;
    case 0x0e:
      return 'E';
      break;
    case 0x0f:
      return 'F';
      break;
  }
  return 'x';
}

void decodeHex(char *Buffer, int in, int len, int start){
  for(int i = 0; i < len/4; i++){
    Buffer[start+(len/4 - i)] = quadtoHex((in >> 4*i) & 0x0F);
  }
}
