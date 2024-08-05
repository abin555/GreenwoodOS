#include "window.h"
#include "allocator.h"
#include "drive.h"

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
	MEM_reserveRegionBlock(blk, window_region_size, WINDOW_BACKBUFFER_VIRT_BASE, FRAMEBUFFER);
	window_buf_size = (fb_width * fb_height * sizeof(uint32_t)) - window_bar_size;
	for(int i = 0; i < MAX_WINDOWS; i++){
		windows[i].backbuffer = (uint32_t *) (WINDOW_BACKBUFFER_VIRT_BASE + (i*window_buf_size));
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
	if(fexists(WINDOW_MOUSE_FILE)){
		print_serial("[WINDOW] Loading cursor image\n");
		struct FILE *cursor = fopen(WINDOW_MOUSE_FILE);
		int size = fsize(cursor);
		cursor_file = malloc(size);
		fcopy(cursor, (char *) cursor_file, size);
		fclose(cursor);
		tga_header_t *header = ((tga_header_t *) cursor_file);
		cursor_width = header->w;
		cursor_height = header->h;
		cursor_bitmap = (uint32_t *) (cursor_file + sizeof(tga_header_t) + header->magic1);
	}
	else{
		print_serial("[WINDOW] Cursor image unavailable!\n");
	}

	timer_attach(2, window_timer_callback);
	
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
            fb_frontbuffer[(y+ly)*fb_width + (x+lx)] = color;
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
	memfcpy(fb_frontbuffer, window->backbuffer, window_buf_size);

	struct IVec2 mousePos = mouse_getPos();
	window_draw_cursor(mousePos.x, mousePos.y);
	//fb_putChar(mousePos.x, mousePos.y, 'M', 0xFFFFFFFF, 0);
}

void window_timer_callback(){

	if(!windows[window_selected].copyOnPromptOnly && windows[window_selected].active){
		window_copy_buffer(&windows[window_selected]);
	}
	else{
		struct IVec2 mousePos = mouse_getPos();
		window_draw_cursor(mousePos.x, mousePos.y);
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