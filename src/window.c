#include "window.h"
#include "allocator.h"
#include "vfs.h"

int window_selected;
struct WINDOW windows[MAX_WINDOWS] = {0};
uint32_t window_bar_size;
uint32_t window_buf_size;

void window_timer_callback();

typedef struct {
  unsigned char magic1;             // must be zero
  unsigned char colormap;           // must be zero
  unsigned char encoding;           // must be 2
  unsigned short cmaporig, cmaplen; // must be zero
  unsigned char cmapent;            // must be zero
  unsigned short x;                 // must be zero
  unsigned short y;                 // image's height
  unsigned short w;                 // image's height
  unsigned short h;                 // image's width
  unsigned char bpp;                // must be 32
  unsigned char pixeltype;          // must be 40
} __attribute__((packed)) tga_header_t;
uint8_t *cursor_file;
uint32_t *cursor_bitmap;
int cursor_width;
int cursor_height;

void window_init(){
	print_serial("[WINDOW] Initialization\n");
	window_bar_size = (fb_width * CHAR_H) * sizeof(uint32_t);
	uint32_t window_region_size = MAX_WINDOWS * fb_width * fb_height * sizeof(uint32_t);
	int blk = MEM_findRegionIdx(window_region_size);
	uint32_t address = MEM_reserveRegionBlock(blk, window_region_size, 0, FRAMEBUFFER);
	window_buf_size = (fb_width * fb_height * sizeof(uint32_t)) - window_bar_size;
	for(int i = 0; i < MAX_WINDOWS; i++){
		windows[i].backbuffer = (uint32_t *) (address + (i*(window_buf_size+window_bar_size)));
		memset(windows[i].backbuffer, 0x11, window_buf_size);
		//windows[i].name = 0;
		memset(windows[i].name, 0, sizeof(windows[i].name));
		windows[i].width = fb_width;
		windows[i].height = fb_height-8;
		windows[i].active = false;
		windows[i].copyOnPromptOnly = false;
	}

	cursor_file = NULL;
	cursor_bitmap = NULL;
	int cursor_fd = vfs_open(WINDOW_MOUSE_FILE, VFS_FLAG_READ);
	if(cursor_fd != -1){
		print_serial("[WINDOW] Loading cursor image\n");
		int size = vfs_seek(cursor_fd, 0, 2);
		vfs_seek(cursor_fd, 0, 0);
		cursor_file = malloc(size);
		vfs_read(cursor_fd, (char *) cursor_file, size);
		vfs_close(cursor_fd);
		tga_header_t *header = ((tga_header_t *) cursor_file);
		cursor_width = header->w;
		cursor_height = header->h;
		cursor_bitmap = (uint32_t *) (cursor_file + sizeof(tga_header_t) + header->magic1);
	}
	else{
		print_serial("[WINDOW] Cursor image unavailable!\n");
	}

	timer_attach(100, window_timer_callback);
	
}

void window_draw_cursor(int x, int y){
	if(cursor_bitmap == NULL){
		fb_putChar(x, y, 'M', 0xFFFFFFFF, 0);
		return;
	}
	for(int ly = 0; ly < cursor_height; ly++){
        for(int lx = 0; lx < cursor_width; lx++){
			uint32_t color = cursor_bitmap[lx+ly*cursor_width];
			if(!(color & 0xFF000000)) continue;
            windows[window_selected].backbuffer[(y+ly)*fb_width + (x+lx)] = color;
        }
    }
}

void window_render_bar(){
	int y = fb_height - CHAR_H;
	for(uint32_t i = 0; i < (fb_width / CHAR_W); i++){
		fb_putChar(CHAR_W*i, y, ' ', 0x0000AA, 0x0000AA);
	}
	for(int i = 0; i < MAX_WINDOWS; i++){
		if(!windows[i].active) continue;
		if(i == window_selected){
			fb_putChar((CHAR_W+2)*i, y, '1'+i, 0x0000AA, 0xFFFFFFFF);
		}
		else{
			fb_putChar((CHAR_W+2)*i, y, '1'+i, 0xFFFFFFFF, 0x0000AA);
		}
	}
	fb_print(CHAR_W*(MAX_WINDOWS + 2), y, windows[window_selected].name);
}

void window_copy_buffer(struct WINDOW *window){
	struct IVec2 mousePos = mouse_getPos();
	window_draw_cursor(mousePos.x, mousePos.y);
	memfcpy(fb_frontbuffer, window->backbuffer, window_buf_size);
}

void window_timer_callback(){

	if(!windows[window_selected].copyOnPromptOnly && windows[window_selected].active){
		window_copy_buffer(&windows[window_selected]);
	}

	window_render_bar();
}

struct WINDOW *window_open(char *name, bool copyPromptOnly){
	struct WINDOW *window = NULL;
	int i;
	for(i = 0; i < MAX_WINDOWS; i++){
		if(!windows[i].active){
			window = &windows[i];
			break;
		}
	}
	if(window == NULL) return NULL;
	window->copyOnPromptOnly = copyPromptOnly;
	window->active = 1;
	//window->name = name;
	for(unsigned int i = 0; i < sizeof(window->name) && name[i] != 0; i++){
		window->name[i] = name[i];
	}
	window_selected = i;
	window_render_bar();
	print_serial("[WINDOW] Opened Window \"%s\"\n", window->name);
	return window;
}

void window_close(struct WINDOW *window){
	window->active = false;
	memset(window->backbuffer, 0x11, window_buf_size);
	memset(window->name, 0, sizeof(windows->name));
	window_render_bar();
}