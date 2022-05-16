# 1 "kernel_programs/terminal.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "kernel_programs/terminal.c"
# 1 "./include/terminal.h" 1



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
# 5 "./include/terminal.h" 2




# 1 "./include/pong.h" 1






void pong();
# 10 "./include/terminal.h" 2
# 1 "./include/grapher.h" 1



# 1 "./include/gfx.h" 1






void gfx_line(u32 x1, u32 y1, u32 x2, u32 y2, u32 color);
void gfx_hline(u32 x1, u32 x2, u32 y, u32 color);
void gfx_vline(u32 y1, u32 y2, u32 x, u32 color);
# 5 "./include/grapher.h" 2

# 1 "./include/system_calls.h" 1
# 7 "./include/grapher.h" 2
# 16 "./include/grapher.h"
struct DATA_Settings{
    int left_bound;
    int right_bound;
    int top_bound;
    int bottom_bound;
    int xscale;
    int yscale;
    char settings;
    double step;
} settings_data;

struct interface_struct{
    unsigned int last_ASCII_P;
    unsigned int select_region;

} grapher_interface;

struct formula{
    int type;
    unsigned int ex_pointer;
    char expression[80];
} formulas[4];

int previousAscii_Pointer;
int previousKey_Pointer;

int axis_center_x;
int axis_center_y;

void draw_settings_pane();
void draw_axis();
void draw_graph();
void clear_region();
void draw_regions();
void grapher_entry();
void grapher_key_handler(char key);
void grapher_draw_formulas();
void plot_point(float x, float y);
float sqrt(float x);
# 11 "./include/terminal.h" 2
# 19 "./include/terminal.h"
char Terminal_Buffer[75];
char Terminal_OUT_Buffer[75*40];

char Terminal_Arguments[75];

void terminal_memory_view();

int terminal_compare(char *buffer, int start, int end, int len);

void terminal_interpret();
void terminal_output(char *Buffer, int start, int end);

void terminal_enter();

void terminal_renderer();
void terminal_console();
void terminal_handler();

void terminal_init();
# 2 "kernel_programs/terminal.c" 2

int Terminal_OUT_pointer = 0;
unsigned short Terminal_Buffer_Pointer = 0;
unsigned int previousASCII_pointer = 0;
unsigned int previousKEY_pointer = 0;


int Terminal_Y = 92;


void terminal_memory_view(){

}

void terminal_renderer(){
    fb_clear(0);
    fb_move_cursor_xy(1,Terminal_Y);
    printChar(0, Terminal_Y, '>');
    fb_write_xy(Terminal_Buffer, 75, Terminal_Buffer_Pointer, 1, Terminal_Y);
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
    fb_set_color(0x00FF00, 0);
    printChar(0, Terminal_Y, '>');
    fb_set_color(0xFFFFFF, 0);
}

void terminal_output(char *Buffer, int start, int end){
    fb_write_cell(Terminal_OUT_pointer, '-', 0xFF0000, 0);
    fb_write_xy(Buffer, end-start, start, end, 0);
    Terminal_OUT_pointer+=fb_terminal_w;
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

    for(int i = 0; i < 75; i++){
        if(Terminal_Buffer[i] == ' ' || Terminal_Buffer[i] == 0){
            Terminal_Arguments[found_splits] = i;
            found_splits++;
        }
        if(Terminal_Buffer[i] == 0){}
    }
    Terminal_Arguments[found_splits+1] = -1;


    if(terminal_compare("print", 0, Terminal_Arguments[0], 5)){
        fb_write_cell(Terminal_OUT_pointer, '-', 0xFF0000, 0);
        fb_write_xy(Terminal_Buffer, 75 -Terminal_Arguments[0], Terminal_Arguments[0]+1, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=fb_terminal_w;


    }
    if(terminal_compare("peek", 0, Terminal_Arguments[0], 4)){
        printChar(79, 0, 'M');
        fb_write_cell(Terminal_OUT_pointer, '-', 0xFF0000, 0);



        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        unsigned int data = ReadMem(addr);
        decodeHex(STR_edit, data, 32, 0);
        fb_write_xy(STR_edit, 9, 0, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=fb_terminal_w;
    }
    if(terminal_compare("poke", 0, Terminal_Arguments[0], 4)){
        fb_write_cell(Terminal_OUT_pointer, '-', 0xFF0000, 0);
        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        unsigned int val = encodeHex(Terminal_Buffer, Terminal_Arguments[1]+1, Terminal_Arguments[2]);
        WriteMem(addr, val);
        decodeHex(STR_edit, addr, 32, 0);
        decodeHex(STR_edit, val, 32, 9);
        STR_edit[9] = ' ';
        fb_write_xy(STR_edit, 18, 0, Terminal_OUT_pointer+1, 0);
        Terminal_OUT_pointer+=fb_terminal_w;
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
        for(int i = 0; i < 1000; i++){
            WriteMem((int)externalProgram+4*i, 0x00000000);
        }
    }
    if(terminal_compare("check", 0, Terminal_Arguments[0], 5)){
        decodeHex(STR_edit, (int) externalProgram, 32, 0);
        fb_write_xy(STR_edit, 9, 0, Terminal_OUT_pointer+1, 0);
        fb_write_cell(Terminal_OUT_pointer, '-', 0xFF0000, 0);
        Terminal_OUT_pointer+=fb_terminal_w;
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
    if(terminal_compare("clear", 0, Terminal_Arguments[0], 5)){
        fb_clear(0);
        Terminal_OUT_pointer = 0;
    }
    if(terminal_compare("pong", 0, Terminal_Arguments[0], 4)){
        pong();
    }
    if(terminal_compare("grapher", 0, Terminal_Arguments[0], 7)){
        grapher_entry();
    }
    if(terminal_compare("reboot", 0, Terminal_Arguments[0], 6)){
        kreboot();
    }
    if(terminal_compare("malloc", 0, Terminal_Arguments[0], 6)){
        unsigned int size = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        void* addr = malloc(size);
        decodeHex(STR_edit, (unsigned int) addr, 32, 0);

        fb_write_cell(Terminal_OUT_pointer, '-', 0xFF0000, 0);
        fb_write_xy(STR_edit, 8, 1, Terminal_OUT_pointer+1, 0);

        decodeHex(STR_edit, memory_used, 32, 0);
        fb_write_xy(STR_edit, 8, 1, Terminal_OUT_pointer+10, 0);

        Terminal_OUT_pointer+=fb_terminal_w;
    }
    if(terminal_compare("msize", 0, Terminal_Arguments[0], 5)){
        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        decodeHex(STR_edit, mgetSize((void *)addr), 32, 0);
        fb_write_cell(Terminal_OUT_pointer, '-', 0xFF0000, 0);
        fb_write_xy(STR_edit, 8, 1, Terminal_OUT_pointer+1, 0);

        Terminal_OUT_pointer+=fb_terminal_w;
    }
    if(terminal_compare("free", 0, Terminal_Arguments[0], 4)){
        unsigned int addr = encodeHex(Terminal_Buffer, Terminal_Arguments[0]+1, Terminal_Arguments[1]);
        free((void *) addr);

        fb_write_cell(Terminal_OUT_pointer, '-', 0xFF0000, 0);

        decodeHex(STR_edit, memory_used, 32, 0);
        fb_write_xy(STR_edit, 8, 1, Terminal_OUT_pointer+1, 0);

        Terminal_OUT_pointer+=fb_terminal_w;
    }
}

void terminal_enter(){


    STR_INSERT(Terminal_Buffer, Terminal_OUT_Buffer, 75, Terminal_OUT_pointer);
    fb_write_xy(Terminal_OUT_Buffer, 75, Terminal_OUT_pointer, Terminal_OUT_pointer, 0);
    Terminal_OUT_pointer += fb_terminal_w;
    Terminal_Buffer_Pointer = 0;



    terminal_interpret();
    for(int i = 0; i < 75; i++){
        Terminal_Buffer[i] = 0;
        printChar(i+1, Terminal_Y, ' ');
    }
    fb_move_cursor_xy(1, Terminal_Y);
    fb_set_color(0x00FF00, 0);
    printChar(0, Terminal_Y, '>');
    fb_set_color(0xFFFFFF, 0);

    if(Terminal_OUT_pointer > (Terminal_Y-1) * fb_terminal_w){
        Terminal_OUT_pointer = 0;
        fb_clear(0);
    }
}

void terminal_handler(){





    if(keyboard_ascii_pointer != previousASCII_pointer && Terminal_Buffer_Pointer < 75){

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
            if(Terminal_Buffer_Pointer < 75){
                Terminal_Buffer_Pointer++;
                fb_move_cursor_xy(Terminal_Buffer_Pointer, Terminal_Y);
            }
            break;
            case 0xC8:
            for(int i = 0; i < 75; i++){
                Terminal_Buffer[i] = Terminal_OUT_Buffer[Terminal_OUT_pointer-fb_terminal_w+i];
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

void terminal_init(){
    Terminal_Y = fb_height/8 - 8;
}
