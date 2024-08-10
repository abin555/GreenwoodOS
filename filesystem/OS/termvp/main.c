#include "libc.h"

char heap[400] = {0};
int heap_idx = 0;

#define WIDTH 400
#define HEIGHT 400


char **args;
void *alloc(int size);
int term_width;
int term_height;

void run_command(char *string);

struct ViewportFunctions *vp_funcs;
struct Viewport *vp;
struct CONSOLE *console;
uint32_t *backbuf;
int running;

void draw_cursor(int idx);
void memset(void *dest, char v, int size);

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

int main(int argc, char **argv){
	print("Terminal!\n");
	set_schedule(ALWAYS);
	vp_funcs = viewport_get_funcs();
	vp = vp_funcs->open(WIDTH, HEIGHT, "GWOS Terminal");
	backbuf = (uint32_t *) requestRegion(WIDTH * HEIGHT *sizeof(uint32_t));
	vp_funcs->set_buffer(vp, backbuf, WIDTH * HEIGHT * sizeof(uint32_t));
	vp_funcs->add_event_handler(vp, event_handler);
	term_width = WIDTH / 8;
	term_height = HEIGHT / 8;
	console = console_open_vp(vp);
	print("Greenwood OS Terminal\n");
	vp_funcs->copy(vp);

	char termbuf[50];
	memset(termbuf, 0, 50);
	
	int idx = 0;
	
	running = 1;
	while(running){
		char *path = getDirectory();
		int i;
		for(i = 0; path[i] != 0; i++){
			vp_funcs->drawChar(vp, 8*i,(term_height-2)*8, path[i], 0xFFFFFF, 0x0);
		}
		vp_funcs->drawChar(vp, 8*i,(term_height-2)*8,'>', 0xFFFFFF, 0x0);
		for(i++; i < term_width; i++){
			vp_funcs->drawChar(vp, 8*i,(term_height-2)*8,' ', 0xFFFFFF, 0x0);
		}
		vp_funcs->copy(vp);
		char c = vp_funcs->getc(vp);
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
					vp_funcs->drawChar(vp, 8*i,(term_height-1)*8, termbuf[i], 0xFFFFFF, 0x0);
				}
			}
		}
		for(int i = 0; i < term_width; i++){
			vp_funcs->drawChar(vp, 8*i,(term_height-1)*8, termbuf[i], 0xFFFFFF, 0x0);
		}
		draw_cursor(idx);
		vp_funcs->copy(vp);
	}
	console_close();
	vp_funcs->close(vp);
	freeRegion(backbuf, WIDTH * HEIGHT * sizeof(uint32_t));
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
	vp_funcs->copy(vp);
	char *command;
	int argc = 1;
	for(int i = 0; cmd[i] != 0 && i < 50; i++){
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
	uint32_t *buf = vp->backbuf;
	for(int x = 0; x < 8; x++){
		buf[WIDTH * ((term_height) * 8 - 1) + (i*8) + x] = 0xFF7FFF;
	}
}
void memset(void *dest, char v, int size){
	for(int i = 0; i < size; i++){
		((char *)dest)[i] = v;
	}
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_FOCUSED || event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
		set_schedule(ALWAYS);
		exit(1);
    }
}
