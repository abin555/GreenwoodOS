#include "libc.h"

char heap[400] = {0};
int heap_idx = 0;

void *alloc(int size);
int term_width;
int term_height;

int main(int argc, char **argv){
	char *window_name = "TERMINAL";
	struct WINDOW *window = window_open(window_name);
	set_schedule(2);
	term_width = window->width / 8;
	term_height = window->height / 8;
	print("Terminal!\n");
	console_open();
	print("\n\nTerminal Console\n");
	char string[] = "This is a terminal!";
	for(int i = 0; i < sizeof(string); i++){
		drawChar(i*8, 0, string[i]);
	}
	window_update();
	char *termbuf = alloc(term_width);
	int idx = 0;
	while(1){
		char c = getc();
		if(c == 8){
			idx--;
			termbuf[idx] = 0;
		}
		else{
			termbuf[idx++] = c;
			if(idx > term_width) idx = 0;
			
		}
		for(int i = 0; i < term_width; i++){
			drawChar(8*i,(term_height-2)*8, termbuf[i]);
		}
		if(c == '='){
			termbuf[idx-1] = 0;
			exec(termbuf, 0, 0);
			//memset(termbuf, 0, term_width);
			for(int i = 0; i < term_width; i++){
				termbuf[i] = 0;
			}
			idx = 0;
		}
		window_update();
	}
}

void *alloc(int size){
	void *address = (void *) &heap[heap_idx];
	heap_idx += size;
	return address;
}