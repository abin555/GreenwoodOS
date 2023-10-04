#include "libc.h"

char *heap;
int heap_idx = 0;
uint32_t *win_buf;
struct WINDOW *window;
int size;
char *file_buf;

void *alloc(int size);
void memset(void *mem, char v, int size);

int main(int argc, char **argv){
	if(argc < 2){
		print("Hex Editor, needs file path!\n");
		return 1;
	}
	print("Opening Hex\n");
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

	window = window_open("HexED");
	win_buf = window->backbuffer;
	
	window_update();

	set_schedule(ONFOCUS);
    while(1){

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