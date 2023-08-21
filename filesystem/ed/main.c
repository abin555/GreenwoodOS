#include "libc.h"

char *heap;
int heap_idx = 0;
uint32_t *win_buf;
struct WINDOW *window;
int size;
char *file_buf;

void *alloc(int size);

void render_file();

int main(int argc, char **argv){
	if(argc < 2){
		print("ed, needs file path!\n");
		return 1;
	}
	print("Opening ed\n");
	heap = (char *) 0x4000;
	print_arg("Opening file %s\n", (uint32_t) argv[1]);
	struct FILE *text = fopen(argv[1]);
	if(text == NULL){
		print("File does not exist!\n");
		return 1;
	}
	size = fsize(text);
	print_arg("File size is %d\n", size);
	file_buf = alloc(size);
	fcopy(text, file_buf, size);

	window = window_open("Ed");
	win_buf = window->backbuffer;
	
	window_update();

	set_schedule(ONFOCUS);

	int line = 0;
	while(1){
		render_file(file_buf, line);
		window_update();
		char c = getc();
		if(c == 'w' && line > 0){
			line--;
		}
		else if(c == 's'){
			line++;
		}
	}
	

}

void *alloc(int size){
	void *address = (void *) &heap[heap_idx];
	heap_idx += size;
	return address;
}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
	}
}

void render_file(char *buf, int line){
	int idx = 0;
	int lines_skipped = 0;
	for(idx = 0; idx < size && lines_skipped < line; idx++){
		if(buf[idx] == '\n'){
			lines_skipped++;
		}
	}
	int x = 0;
	int y = 0;
	memset(win_buf, 0, window->width*window->height*sizeof(uint32_t));
	for(; idx < size && y < window->height / 8; idx++){
		if(buf[idx] == '\n'){
			x = 0;
			y++;
			continue;
		}
		if(x >= window->width / 8){
			x = 0;
			y++;
		}
		drawChar(x*8, y*8, buf[idx]);
		x++;
	}
}