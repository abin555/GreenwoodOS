#include "window.h"
#include "allocator.h"
#include "vfs.h"

int window_selected;
struct WINDOW windows[MAX_WINDOWS] = {0};
uint32_t window_bar_size;
uint32_t window_buf_size;

void window_timer_callback();

struct Bitmap cursor_bitmap_s;

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

	cursor_bitmap_s = loadBitmap(WINDOW_MOUSE_FILE);
	if(cursor_bitmap_s.bitmap == NULL){
		print_serial("[WINDOW] Cursor image unavailable!\n");
	}

	timer_attach(100, window_timer_callback);
	
}

void window_draw_cursor(int x, int y){
	if(cursor_bitmap_s.bitmap == NULL){
		fb_putChar(x, y, 'M', 0xFFFFFFFF, 0);
		return;
	}
	drawBitmap(x, y, &cursor_bitmap_s, &windows[window_selected]);
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
	//print_serial("[WINDOW] Copy\n");
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