#include "console.h"

struct CONSOLE consoles[MAX_CONSOLE] = {0};

void console_init(){
	print_serial("[CONSOLE] Initialization\n");
	for(int i = 0; i < MAX_CONSOLE; i++){
		consoles[i].window = NULL;
		consoles[i].width = fb_width / CHAR_W;
		consoles[i].height = fb_height / CHAR_H - CHAR_H;
		consoles[i].buf_size = consoles[i].width * consoles[i].height;
		consoles[i].buf = malloc(consoles[i].buf_size);
		consoles[i].active = false;
		consoles[i].cursor = 0;
	}	
}

struct CONSOLE *console_open(struct WINDOW *window){
	struct CONSOLE *console = NULL;
	for(int i = 0; i < MAX_CONSOLE; i++){
		if(!consoles[i].active){
			console = &consoles[i];
			break;
		}
	}
	if(console == NULL) return NULL;
	memset(console->buf, 0, console->buf_size);
	console->active = true;
	console->cursor = 0;
	console->last_cursor = 0;
	console->window = window;
	return console;
}

void console_close(struct CONSOLE *console){
	console->cursor = 0;
	console->active = false;
}

void render_console(struct CONSOLE *console){
	uint32_t count_lines = 1;
	uint32_t cursor = 0;
	//Calculate Number of Lines.
	while(console->buf[cursor]){
		if(console->buf[cursor] == '\n') count_lines++;
		cursor++;
	}
	cursor = 0;
	//If more lines than available, shift up one line.
	if(count_lines >= console->height){
		int count_remove = 0;
		while(console->buf[count_remove] != '\n' && console->buf[count_remove] != '\0'){
			count_remove++;
		}
		//memcpy(console->buf, console->buf + count_remove, count_remove);
		for(uint32_t i = 0; i < console->buf_size - count_remove; i++){
			console->buf[i] = console->buf[count_remove + i];
		}
		console->cursor -= count_remove;
		memset(console->buf + console->cursor, 0, console->buf_size - console->cursor);
	}

	int x = 0;
	int y = 0;
	while(console->buf[cursor] != '\0' && cursor < console->buf_size){
		if(console->buf[cursor] == '\n'){
			y++;
			x = 0;
		}
		else{
			if((uint32_t) x >= console->width){
				x = 0;
				y++;
			}
			if(cursor >= (uint32_t) console->last_cursor) buf_putChar(console->window->backbuffer, x*CHAR_W, y*CHAR_H, console->buf[cursor], 0xFFFFFFFF, 0);
			x++;
		}
		cursor++;
	}
	window_copy_buffer(console->window);
}

void print_console(struct CONSOLE *console, char *msg, ...){
	console->last_cursor = console->cursor;
	while(*msg != '\0'){
		console->buf[console->cursor] = *msg;
		console->cursor++;
		msg++;
	}
	render_console(console);
	print_serial(console->buf);
	print_serial("Cursor is at %d\n", console->cursor);
}