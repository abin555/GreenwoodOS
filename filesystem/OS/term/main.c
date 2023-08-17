#include "libc.h"

char heap[400] = {0};
int heap_idx = 0;



char **args;
void *alloc(int size);
int term_width;
int term_height;

void run_command(char *string);

struct WINDOW *window;
struct CONSOLE *console;

void draw_cursor(int idx);

int main(int argc, char **argv){
	char *window_name = "TERMINAL";
	window = window_open(window_name);
	set_schedule(2);
	term_width = window->width / 8;
	term_height = window->height / 8;
	print("Terminal!\n");
	console = console_open();
	print("Greenwood OS Terminal\n");
	window_update();
	char *termbuf = alloc(term_width);
	int idx = 0;
	while(1){
		char c = getc();
		if(c == '0') continue;
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
		if(c == 10){
			print(termbuf);
			termbuf[idx-1] = 0;
			//exec(termbuf, 0, 0);
			run_command(termbuf);
			//memset(termbuf, 0, term_width);
			for(int i = 0; i < term_width; i++){
				termbuf[i] = 0;
			}
			idx = 0;
		}
		draw_cursor(idx);
		window_update();
	}
}

void *alloc(int size){
	void *address = (void *) &heap[heap_idx];
	heap_idx += size;
	return address;
}

void mcpy(void *dest, void *src, int size){
	for(int i = 0; i < size; i++){
		((char *) dest)[i] = ((char *) src)[i]; 
	}
}

void run_command(char *cmd){
	int fullsize = 0;
	while(cmd[fullsize] != 0){
		fullsize++;
	}
	fullsize++;
	print_arg("Fullsize is %d\n", fullsize);
	char *command;
	int argc = 1;
	for(int i = 0; cmd[i] != 0; i++){
		if(cmd[i] == ' ') argc++;
	}
	args = (char **) kmalloc(sizeof(char *) * argc);
	int idx = 0;
	for(int i = 0; i < argc && cmd[idx] != 0; i++){
		int size = 0;
		for(int j = 0; cmd[j] != ' ' && cmd[j] != 0; j++){
			size++;
		}
		print_arg("Arg %d Size is ", i);
		print_arg("%d ", size);
		args[i] = (char *) kmalloc(size+1);
		mcpy(args[i], cmd+idx, size);
		print_arg("%s\n", (uint32_t) args[i]);
		idx = idx + size + 1;
	}
	for(int i = 0; i < fullsize; i++){
		if(cmd[i] == ' ') cmd[i] = 0;
	}
	exec(args[0], argc, args);
}

void draw_cursor(int i){
	uint32_t *buf = window->backbuffer;
	for(int x = 0; x < 8; x++){
		buf[window->width * ((term_height - 1) * 8 - 2) + (i*8) + x] = 0xFFFFFF;
	}
}