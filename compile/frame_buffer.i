# 1 "frame_buffer.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "frame_buffer.c"
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
# 6 "./include/memory.h" 2
# 1 "./include/frame_buffer.h" 1
# 7 "./include/memory.h" 2
# 1 "./include/string.h" 1



# 1 "./include/ascii_tables.h" 1


# 1 "./include/keyboard.h" 1



# 1 "./include/io.h" 1





extern void outb(unsigned short port, unsigned char data);
extern unsigned char inb(unsigned short pos);

extern void outportl(uint16_t port, uint32_t value);
extern uint32_t inportl(uint16_t port);

void WriteMem(uint32_t Address, uint32_t Value);
uint32_t ReadMem(uint32_t Address);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char *INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
extern void kreboot();
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
# 4 "./include/ascii_tables.h" 2


char kbd_US[256];
char kbd_US_shift[256];
# 5 "./include/string.h" 2

char STR_edit[128];

int STR_Compare(char *elem1, char *elem2, int start, int end);

void STR_INSERT(char *in_str, char *out_str, int len, int write_index);

void decodeData(char *Buffer, int in, int len, int start);

void decodeHex(char *Buffer, unsigned int in, int len, int start);
void decodeInt(char *Buffer, int in, int len, int start);

unsigned int encodeHex(char *Buffer, int start, int end);

char quadToHex(char quad);
char hexToQuad(char hex);
void strcpy(char *source, char *destination, unsigned int len);
# 8 "./include/memory.h" 2

unsigned int memory_used;
unsigned int heap_begin;
unsigned int heap_end;

typedef struct{
    unsigned int size;
    char status;
} alloc_t;

void memcpy(u64* source, u64* target, u64 len);
void* memset(void * place, int val, unsigned int size);

void* malloc(unsigned int size);
void free(void *mem);
void mem_init(unsigned int kernelEnd);
unsigned int mgetSize(void *mem);

void printHeap();
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
void fb_write_xy_scaled(char *Buffer, int len, int start, unsigned int x, unsigned int y, unsigned int scale);

void fb_move_cursor(unsigned int pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);

void fb_copyBuffer();
void fb_clearBackBuffer(u32 color);
# 2 "frame_buffer.c" 2
# 1 "./include/font.h" 1





static const u8 FONT[128][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
# 3 "frame_buffer.c" 2



int fb_cursor = 0;

void fb_setPixel(u32 x, u32 y, u32 color){
    if (fb_width <= x || fb_height <= y)
        return;

    fb[ fb_width * y + x] = color;
}

void init_fb(struct multiboot_tag_framebuffer *tagfb){
    fb = (u64 *) (unsigned long) tagfb->common.framebuffer_addr;
    fb_height = tagfb->common.framebuffer_height;
    fb_width = tagfb->common.framebuffer_width;
    fb_terminal_w = fb_width / 8;
    fb_terminal_h = fb_height / 8;
}

void fb_write_cell(u32 index, char c, u32 fg, u32 bg){
    u32 x = (index % fb_terminal_w)*8;
    u32 y = (index / fb_terminal_w)*8;
    for(int layer = 0; layer < 8; layer++){
        for(int pixel = 0; pixel < 8; pixel++){
            fb[fb_width * (y+layer) + x+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}
void printChar(unsigned int x, unsigned int y, char c){
    for(int layer = 0; layer < 8; layer++){
        for(int pixel = 0; pixel < 8; pixel++){
            fb[fb_width * ((y*8)+layer) + (x*8)+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? FG : BG;
        }
    }
}

void printChar_Scaled(unsigned int x, unsigned int y, char c, int scale){
    for(int layer = 0; layer < 8; layer++){
        for(int pixel = 0; pixel < 8; pixel++){

            pixelScaled((x+pixel+scale)*scale,(y+layer+scale)*scale,scale,((FONT[(int)c][layer] >> pixel) & 1) ? FG : BG);
        }
    }
}

void pixelScaled(unsigned int x, unsigned int y, int scale, u32 color){
    for(int scalex = 0; scalex<scale; scalex++){
        for(int scaley = 0; scaley<scale; scaley++){
            fb[fb_width*(y+scaley)+x+scalex] = color;
        }
    }
}

void fb_set_color(unsigned int fg, unsigned int bg){
    FG = fg;
    BG = bg;
}

void fb_clear(unsigned int color){
    for(u32 i = 0; i < fb_width*fb_height; i++){
        fb[i] = color;
    }
}

int fb_write(char *buf, unsigned int len)
{
    for (unsigned int index = 0; index < len; index++)
    {
        fb_write_cell(fb_cursor+index, buf[index], FG, BG);
    }


    return 0;
}

int fb_write_start(char *buf, unsigned int len, unsigned int start){
    for (unsigned int index = 0; index < len; index++)
    {
        fb_write_cell(fb_cursor+index, buf[index+start], FG, BG);
    }


    return 0;
}

void fb_write_xy(char *Buffer, int len, int start, unsigned int x, unsigned int y){
    for(int index = 0; index < len; index++){


        fb_write_cell((y*fb_terminal_w)+x+index, Buffer[index+start], FG, BG);
    }
}

void fb_write_xy_scaled(char *Buffer, int len, int start, unsigned int x, unsigned int y, unsigned int scale){
    for(int index = 0; index < len; index++){
        printChar_Scaled(x+index*(4*scale), y, Buffer[index+start], scale);
    }
}

void fb_move_cursor(unsigned int pos){
    fb_cursor = pos;
}
void fb_move_cursor_xy(unsigned int x, unsigned int y){
    fb_cursor = fb_terminal_w*y+x;
}

void fb_copyBuffer(){

    for(u32 index = 0; index<fb_width*fb_height; index++){
        fb[index] = fb_backBuffer[index];
    }
}

void fb_clearBackBuffer(u32 color){
    for(u32 index = 0; index<fb_width*fb_height; index++){
        fb_backBuffer[index] = color;
    }
}
