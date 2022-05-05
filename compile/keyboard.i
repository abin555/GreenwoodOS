# 1 "keyboard.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "keyboard.c"
# 1 "./include/keyboard.h" 1



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
extern uint32_t * restore_kernel_addr;
extern void PROGA();

extern unsigned int *externalProgram;
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
_Bool KYBRD_CTRL;

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
void keyboard_handle_interrupt(unsigned int interrupt);

char convertascii(unsigned char scan_code);

unsigned char keyboard_KEYBUFFER[0xFF];
char keyboard_ASCIIBuffer[0xFF];

unsigned int keyboard_KEYBUFFER_POINTER;
unsigned int keyboard_ascii_pointer;

unsigned char prev_Scancode;
unsigned char char_scancode;
# 2 "keyboard.c" 2
# 1 "./include/frame_buffer.h" 1



# 1 "./include/types.h" 1
# 21 "./include/types.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long int i64;
# 5 "./include/frame_buffer.h" 2
# 1 "./include/multiboot.h" 1
# 72 "./include/multiboot.h"
typedef unsigned char multiboot_uint8_t;
typedef unsigned short multiboot_uint16_t;
typedef unsigned int multiboot_uint32_t;
typedef unsigned long long multiboot_uint64_t;

struct multiboot_header
{

  multiboot_uint32_t magic;


  multiboot_uint32_t architecture;


  multiboot_uint32_t header_length;


  multiboot_uint32_t checksum;
};

struct multiboot_header_tag
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
};

struct multiboot_header_tag_information_request
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t requests[0];
};

struct multiboot_header_tag_address
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t header_addr;
  multiboot_uint32_t load_addr;
  multiboot_uint32_t load_end_addr;
  multiboot_uint32_t bss_end_addr;
};

struct multiboot_header_tag_entry_address
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t entry_addr;
};

struct multiboot_header_tag_console_flags
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t console_flags;
};

struct multiboot_header_tag_framebuffer
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t width;
  multiboot_uint32_t height;
  multiboot_uint32_t depth;
};

struct multiboot_header_tag_module_align
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
};

struct multiboot_header_tag_relocatable
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t min_addr;
  multiboot_uint32_t max_addr;
  multiboot_uint32_t align;
  multiboot_uint32_t preference;
};

struct multiboot_color
{
  multiboot_uint8_t red;
  multiboot_uint8_t green;
  multiboot_uint8_t blue;
};

struct multiboot_mmap_entry
{
  multiboot_uint64_t addr;
  multiboot_uint64_t len;





  multiboot_uint32_t type;
  multiboot_uint32_t zero;
};
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

struct multiboot_tag
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
};

struct multiboot_tag_string
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  char string[0];
};

struct multiboot_tag_module
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t mod_start;
  multiboot_uint32_t mod_end;
  char cmdline[0];
};

struct multiboot_tag_basic_meminfo
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t mem_lower;
  multiboot_uint32_t mem_upper;
};

struct multiboot_tag_bootdev
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t biosdev;
  multiboot_uint32_t slice;
  multiboot_uint32_t part;
};

struct multiboot_tag_mmap
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t entry_size;
  multiboot_uint32_t entry_version;
  struct multiboot_mmap_entry entries[0];
};

struct multiboot_vbe_info_block
{
  multiboot_uint8_t external_specification[512];
};

struct multiboot_vbe_mode_info_block
{
  multiboot_uint8_t external_specification[256];
};

struct multiboot_tag_vbe
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;

  multiboot_uint16_t vbe_mode;
  multiboot_uint16_t vbe_interface_seg;
  multiboot_uint16_t vbe_interface_off;
  multiboot_uint16_t vbe_interface_len;

  struct multiboot_vbe_info_block vbe_control_info;
  struct multiboot_vbe_mode_info_block vbe_mode_info;
};

struct multiboot_tag_framebuffer_common
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;

  multiboot_uint64_t framebuffer_addr;
  multiboot_uint32_t framebuffer_pitch;
  multiboot_uint32_t framebuffer_width;
  multiboot_uint32_t framebuffer_height;
  multiboot_uint8_t framebuffer_bpp;



  multiboot_uint8_t framebuffer_type;
  multiboot_uint16_t reserved;
};

struct multiboot_tag_framebuffer
{
  struct multiboot_tag_framebuffer_common common;

  union
  {
    struct
    {
      multiboot_uint16_t framebuffer_palette_num_colors;
      struct multiboot_color framebuffer_palette[0];
    };
    struct
    {
      multiboot_uint8_t framebuffer_red_field_position;
      multiboot_uint8_t framebuffer_red_mask_size;
      multiboot_uint8_t framebuffer_green_field_position;
      multiboot_uint8_t framebuffer_green_mask_size;
      multiboot_uint8_t framebuffer_blue_field_position;
      multiboot_uint8_t framebuffer_blue_mask_size;
    };
  };
};

struct multiboot_tag_elf_sections
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t num;
  multiboot_uint32_t entsize;
  multiboot_uint32_t shndx;
  char sections[0];
};

struct multiboot_tag_apm
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint16_t version;
  multiboot_uint16_t cseg;
  multiboot_uint32_t offset;
  multiboot_uint16_t cseg_16;
  multiboot_uint16_t dseg;
  multiboot_uint16_t flags;
  multiboot_uint16_t cseg_len;
  multiboot_uint16_t cseg_16_len;
  multiboot_uint16_t dseg_len;
};

struct multiboot_tag_efi32
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t pointer;
};

struct multiboot_tag_efi64
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint64_t pointer;
};

struct multiboot_tag_smbios
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t major;
  multiboot_uint8_t minor;
  multiboot_uint8_t reserved[6];
  multiboot_uint8_t tables[0];
};

struct multiboot_tag_old_acpi
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t rsdp[0];
};

struct multiboot_tag_new_acpi
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t rsdp[0];
};

struct multiboot_tag_network
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t dhcpack[0];
};

struct multiboot_tag_efi_mmap
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t descr_size;
  multiboot_uint32_t descr_vers;
  multiboot_uint8_t efi_mmap[0];
};

struct multiboot_tag_efi32_ih
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t pointer;
};

struct multiboot_tag_efi64_ih
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint64_t pointer;
};

struct multiboot_tag_load_base_addr
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t load_base_addr;
};
# 6 "./include/frame_buffer.h" 2
# 1 "./include/memory.h" 1





void memcpy(u64* source, u64* target, u64 len);
# 7 "./include/frame_buffer.h" 2
# 16 "./include/frame_buffer.h"
u32 fb_width;
u32 fb_height;
u64* fb;
u32 fb_backBuffer[1920*1080];
int fb_terminal_w;
int fb_terminal_h;

unsigned int FG;
unsigned int BG;

void fb_setPixel(u32 x, u32 y, u32 color);

void init_fb(struct multiboot_tag_framebuffer *tagfb);

void fb_write_cell(u32 index, char c, u32 fb, u32 bg);

void printChar(unsigned int x, unsigned int y, char c);
void printChar_Scaled(unsigned int x, unsigned int y, char c, int scale);

void pixelScaled(unsigned int x, unsigned int y, int scale, u32 color);

void fb_set_color(unsigned int fg, unsigned int bg);

void fb_clear(unsigned int color);

int fb_write(char *buf, unsigned int len);
int fb_write_start(char *buf, unsigned int len, unsigned int start);
void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y);

void fb_move_cursor(unsigned int pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);

void fb_copyBuffer();
void fb_clearBackBuffer(u32 color);
# 3 "keyboard.c" 2
# 1 "./include/ascii_tables.h" 1


# 1 "./include/keyboard.h" 1
# 4 "./include/ascii_tables.h" 2


char kbd_US[256];
char kbd_US_shift[256];
# 4 "keyboard.c" 2
# 1 "./include/string.h" 1



# 1 "./include/ascii_tables.h" 1
# 5 "./include/string.h" 2

char STR_edit[128];

int STR_Compare(char *elem1, char *elem2, int start, int end);

void STR_INSERT(char *in_str, char *out_str, int len, int write_index);

void decodeData(char *Buffer, int in, int len, int start);

void decodeHex(char *Buffer, int in, int len, int start);

unsigned int encodeHex(char *Buffer, int start, int end);

char quadToHex(char quad);
char hexToQuad(char hex);
# 5 "keyboard.c" 2

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

        case 0x5B:

        break;

        case 0xD3:

        break;
        case 0x1D:
        KYBRD_CTRL = 1;
        break;
        case 0x9D:
        KYBRD_CTRL = 0;
        break;
        case 0x2E:
        restore_kernel();
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

void keyboard_handle_interrupt(unsigned int interrupt){
    unsigned char scan_code;
    scan_code = keyboard_enc_read_buf();
    pic_acknowledge(interrupt);
    if(scan_code){
        decodeHex(STR_edit, scan_code, 8, 0);
        fb_write_xy(STR_edit, 3, 0, 0, 0);
        if(kbd_US[scan_code] != 0 && !KYBRD_CTRL){
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
    if(keyboard_ascii_pointer >= 0xFF){
        keyboard_ascii_pointer = 0;
    }
    if(keyboard_KEYBUFFER_POINTER >= 0xFF){
        keyboard_KEYBUFFER_POINTER = 0;
    }
}
