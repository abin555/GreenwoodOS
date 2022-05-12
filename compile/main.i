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
# 2 "main.c" 2
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
# 3 "main.c" 2
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
# 4 "main.c" 2
# 1 "./include/frame_buffer.h" 1



# 1 "./include/types.h" 1
# 5 "./include/frame_buffer.h" 2
# 1 "./include/multiboot.h" 1
# 6 "./include/frame_buffer.h" 2
# 1 "./include/memory.h" 1





# 1 "./include/frame_buffer.h" 1
# 7 "./include/memory.h" 2
# 1 "./include/string.h" 1



# 1 "./include/ascii_tables.h" 1


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

void fb_move_cursor(unsigned int pos);
void fb_move_cursor_xy(unsigned int x, unsigned int y);

void fb_copyBuffer();
void fb_clearBackBuffer(u32 color);
# 5 "main.c" 2
# 1 "./include/keyboard.h" 1
# 6 "main.c" 2
# 1 "./include/terminal.h" 1
# 9 "./include/terminal.h"
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
# 7 "main.c" 2
# 1 "./include/interrupts.h" 1



unsigned char SYS_MODE;


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
 unsigned int esp;
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
# 8 "main.c" 2
# 1 "./include/keyboard_test.h" 1
# 10 "./include/keyboard_test.h"
char decode[500];

void KYBRD_DEBUG_DISPLAY();
# 9 "main.c" 2
# 1 "./include/gfx.h" 1
# 10 "main.c" 2
# 1 "./include/pong.h" 1
# 11 "main.c" 2
# 1 "./include/memory.h" 1
# 12 "main.c" 2
# 1 "./include/PCI.h" 1
# 10 "./include/PCI.h"
struct __pci_driver;

typedef struct {
 unsigned int vendor;
 unsigned int device;
 unsigned int func;
 struct __pci_driver *driver;
} pci_device;

typedef struct {
 unsigned int vendor;
 unsigned int device;
 unsigned int func;
} pci_device_id;

typedef struct __pci_driver {
 pci_device_id *table;
 char *name;
 char (*init_one)(pci_device *);
 char (*init_driver)(void);
 char (*exit_driver)(void);
} pci_driver;

void add_pci_device();

unsigned short pci_read_word(unsigned short bus, unsigned short slot, unsigned short func, unsigned short offset);
unsigned short getVendorID(unsigned short bus, unsigned short device, unsigned short function);
unsigned short getDeviceID(unsigned short bus, unsigned short device, unsigned short function);

void pci_init();
void pci_probe();
# 13 "main.c" 2


extern void load_gdt();

u32 vga_width;
u32 vga_height;
u64* screen;

void flyingDot(){
  int x = 50;
  int y = 50;
  int dx = 3;
  int dy = 5;
  int color = 0xFF;
  while(1){
    for(int clear = 0; clear < 0xFF; clear++){

      x+=dx;
      y+=dy;
      fb_setPixel(x,y,0xFFFFFF);
      if(x >= (int) fb_width-10 || x <= 10){
        dx *= -1;
        if(dx < 0){
          dx--;
        }
        if(dx>0){
          dx++;
        }
      }
      if(y >= (int) fb_height-10 || y <= 10){
        dy *= -1;
        if(dy < 0){
          dy--;
        }
        if(dy>0){
          dy++;
        }
      }

      if(dx>50||dx<-50){dx=1;}
      if(dy>50||dy<-50){dy=1;}
      if(x>(int)fb_width || x<0){x=fb_width/2;}
      if(y>(int)fb_height || y<0){y=fb_height/2;}
      for(u32 t = 0; t < 0xFFFF; t++){}
    }
    for(int bx = 0; bx<500; bx++){
      for(int by = 0; by<500; by++){
        fb_setPixel(bx, by, ((color & 0xFF) << 16) | ((color & 0xFF) << 8) | color);
        color--;
        if(color == 0){
          color = 0xFF;
        }

      }
    }
  }
}

void kmain_loop(){
  while(1){
    terminal_handler();
  }
}

extern unsigned int kernel_end;

int kmain(unsigned long magic, unsigned long magic_addr){
  struct multiboot_tag *tag;

  if(magic != 0x36d76289){
    return 0xFF;
  }

  for(
    tag = (struct multiboot_tag *) (magic_addr + 8);
    tag->type != 0;
    tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))
  ){
    switch(tag->type){
      case 8:
        {
            struct multiboot_tag_framebuffer *tagfb
              = (struct multiboot_tag_framebuffer *) tag;
            init_fb(tagfb);
        }
    }
  }


  load_gdt();
  interrupt_install_idt();
  fb_set_color(0xFFFFFF,0);
  restore_kernel_addr = (u32 *) &kmain_loop;

  mem_init(0x10000000);
  pci_init();

  terminal_init();


  kmain_loop();
  return 0;
}
