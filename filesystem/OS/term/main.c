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
void memset(void *dest, char v, int size);

int main(int argc, char **argv){
	char *window_name = "TERMINAL";
	window = window_open(window_name, 0);
	set_schedule(2);
	term_width = window->width / 8;
	term_height = window->height / 8;
	print("Terminal!\n");
	console = console_open();
	print("Greenwood OS Terminal\n");
	window_update();
	char *termbuf = alloc(term_width);
	memset(termbuf, 0, term_width);

	int idx = 0;
	while(1){
		char *path = getDirectory();
		int i;
		for(i = 0; path[i] != 0; i++){
			drawChar(8*i,(term_height-2)*8, path[i]);
		}
		drawChar(8*i,(term_height-2)*8,'>');
		for(i++; i < term_width; i++){
			drawChar(8*i,(term_height-2)*8,' ');
		}
		window_update();
		char c = getc();
		if(c == 0x11) idx-=2;
		else if(c == 0x12) idx++;
		else{
			if(c == 0) continue;
			if(c == 8){
				idx--;
				termbuf[idx] = 0;
			}
			else{
				termbuf[idx++] = c;
				if(idx > term_width) idx = 0;
				
			}
			if(c == 10){
				print_arg("$ %s\n", (uint32_t) termbuf);
				termbuf[idx-1] = 0;
				//exec(termbuf, 0, 0);
				run_command(termbuf);
				memset(termbuf, 0, term_width);
				for(int i = 0; i < term_width; i++){
					termbuf[i] = 0;
				}
				idx = 0;
				for(int i = 0; i < term_width; i++){
					drawChar(8*i,(term_height-1)*8, termbuf[i]);
				}
			}
		}
		for(int i = 0; i < term_width; i++){
			drawChar(8*i,(term_height-1)*8, termbuf[i]);
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
	if(fullsize == 0){
		return;
	}
	fullsize++;
	window_update();
	char *command;
	int argc = 1;
	for(int i = 0; cmd[i] != 0; i++){
		if(cmd[i] == ' ') argc++;
	}
	args = (char **) kmalloc(sizeof(char *) * argc);
	int idx = 0;
	for(int i = 0; i < argc && cmd[idx] != 0; i++){
		int size = 0;
		for(int j = 0; cmd[idx+j] != ' ' && cmd[idx+j] != 0; j++){
			size++;
		}
		args[i] = (char *) kmalloc(size+1);
		memset(args[i], 0, size+1);
		mcpy(args[i], cmd+idx, size);
		idx = idx + size + 1;
	}
	for(int i = 0; i < fullsize; i++){
		if(cmd[i] == ' ') cmd[i] = 0;
	}
	if(!strcmp(args[0], "cd")){
		changeDirectory(args[1]);
	}
	else if(!strcmp(args[0], "ls")){
		if(argc == 1){
			printDirectoryContents(".");
		}
		else{
			printDirectoryContents(args[1]);
		}
	}
	else if(!strcmp(args[0], "mkdir")){
		print_arg("Making dir %s\n", (uint32_t) args[1]);
		if(argc >= 1){
			fmkdir(args[1]);
		}
	}
	else if(!strcmp(args[0], "mkfile")){
		print_arg("Making file %s\n", (uint32_t) args[1]);
		if(argc >= 1){
			fmkfile(args[1], 100);
		}
	}
	else{
		exec(args[0], argc, args);
	}
}

void draw_cursor(int i){
	uint32_t *buf = window->backbuffer;
	for(int x = 0; x < 8; x++){
		buf[window->width * ((term_height) * 8 - 1) + (i*8) + x] = 0xFF7FFF;
	}
}
void memset(void *dest, char v, int size){
	for(int i = 0; i < size; i++){
		((char *)dest)[i] = v;
	}
}