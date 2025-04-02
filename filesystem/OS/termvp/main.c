#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/task.h>
#include <sys/vp.h>
#include <sys/console.h>
#include <sys/dir.h>
#include <sys/stat.h>

#define WIDTH 400
#define HEIGHT 400

char **args;
void *alloc(int size);
int term_width;
int term_height;

void run_command(char *string);

struct Viewport *vp;
struct CONSOLE *console;
uint32_t *backbuf;
int running;

void draw_cursor(int idx);

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

int main(int argc, char **argv){
	puts("Terminal!\n");
	set_schedule(ALWAYS);
	vp = vp_open(WIDTH, HEIGHT, "GWOS Terminal");
	backbuf = (uint32_t *) malloc(WIDTH * HEIGHT *sizeof(uint32_t));
	vp_set_buffer(vp, backbuf, WIDTH * HEIGHT * sizeof(uint32_t));
	vp_add_event_handler(vp, event_handler);
	term_width = WIDTH / 8;
	term_height = HEIGHT / 8;
	console = console_open_vp(vp);
	puts("Greenwood OS Terminal\n");
	vp_copy(vp);

	char termbuf[50];
	char dirBuf[20];
	memset(termbuf, 0, 50);
	
	int idx = 0;
	
	running = 1;
	while(running){
		char *path = getcwd(dirBuf, sizeof(dirBuf));
		int i;
		for(i = 0; path[i] != 0; i++){
			vp_drawChar(vp, 8*i,(term_height-2)*8, path[i], 0xFFFFFF, 0x0);
		}
		vp_drawChar(vp, 8*i,(term_height-2)*8,'>', 0xFFFFFF, 0x0);
		for(i++; i < term_width; i++){
			vp_drawChar(vp, 8*i,(term_height-2)*8,' ', 0xFFFFFF, 0x0);
		}
		vp_copy(vp);
		char c = vp_getc(vp);
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
				printf("$ %s\n", (uint32_t) termbuf);
				termbuf[idx-1] = 0;
				//exec(termbuf, 0, 0);
				run_command(termbuf);
				memset(termbuf, 0, term_width);
				for(int i = 0; i < term_width; i++){
					termbuf[i] = 0;
				}
				idx = 0;
				for(int i = 0; i < term_width; i++){
					vp_drawChar(vp, 8*i,(term_height-1)*8, termbuf[i], 0xFFFFFF, 0x0);
				}
			}
		}
		for(int i = 0; i < term_width; i++){
			vp_drawChar(vp, 8*i,(term_height-1)*8, termbuf[i], 0xFFFFFF, 0x0);
		}
		draw_cursor(idx);
		vp_copy(vp);
	}
	console_close();
	vp_close(vp);
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
	vp_copy(vp);
	char *command;
	int argc = 1;
	for(int i = 0; cmd[i] != 0 && i < 50; i++){
		if(cmd[i] == ' ') argc++;
	}
	args = (char **) malloc(sizeof(char *) * argc);
	int idx = 0;
	for(int i = 0; i < argc && cmd[idx] != 0; i++){
		int size = 0;
		for(int j = 0; cmd[idx+j] != ' ' && cmd[idx+j] != 0; j++){
			size++;
		}
		args[i] = (char *) malloc(size+1);
		memset(args[i], 0, size+1);
		memcpy(args[i], cmd+idx, size);
		idx = idx + size + 1;
	}
	for(int i = 0; i < fullsize; i++){
		if(cmd[i] == ' ') cmd[i] = 0;
	}
	if(!strcmp(args[0], "cd")){
		chdir(args[1]);
	}
	else if(!strcmp(args[0], "ls")){
		char *path;
		if(argc == 1){
			path = ".";
		}
		else{
			path = args[1];
		}
		struct DirectoryListing dir = getDirectoryListing(path);
		for(int i = 0; i < dir.num_entries; i++){
			puts(dir.entries[i].filename);
		}
	}
	else if(!strcmp(args[0], "mkdir")){
		printf("Making dir %s\n", (uint32_t) args[1]);
		if(argc >= 1){
			mkdir(args[1], 0);
		}
	}
	else if(!strcmp(args[0], "mkfile")){
		printf("Making file %s\n", (uint32_t) args[1]);
		if(argc >= 1){
			creat(args[1]);
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
		//exit(1);
    }
}
