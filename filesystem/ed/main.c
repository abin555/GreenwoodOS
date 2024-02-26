#include "libc.h"

char *heap;
uint32_t *win_buf;
struct WINDOW *window;

int size;
char *file_buf;

int scan;

void *alloc(int size);
void memset(void *mem, char v, int size);
void render_file();
void render_mode();

char *mode_strs[] = {
	"--CMD--",
	"--REPLACE--",
	"--INSERT--"
};

int ed_mode = 0;

char cmd_buf[10] = ":WAT\0";

int main(int argc, char **argv){
	if(argc < 2){
		print("ed, needs file path!\n");
		return 1;
	}
	print("Opening ed\n");
	heap = (char *) 0x8000;
	print_arg("Opening file %s\n", (uint32_t) argv[1]);
	struct FILE *text = fopen(argv[1]);
	if(text == NULL){
		print("File does not exist!\n");
		return 1;
	}
	size = fsize(text);
	print_arg("File size is %d\n", size);
	file_buf = heap;
	fcopy(text, file_buf, size);

	window = window_open("Ed");
	win_buf = window->backbuffer;
	
	window_update();

	set_schedule(ONFOCUS);

	int line = 0;
	scan = 0;
	int cmd_scan = 0;
	ed_mode = 0;

	int chars_added = 0;
	while(1){
		render_file(file_buf, line);
		render_mode();
		if(ed_mode == 0){
			drawChar(cmd_scan*8, window->height-15, '_');
			for(int i = 0; i < sizeof(cmd_buf); i++){
				drawChar(i*8, window->height-16, cmd_buf[i]);
			}
		}
		window_update();
		char c = getc();
		//print_arg("Key %2x\n", c);
		if(c == 0x33){
			ed_mode = 0;
			memset(cmd_buf, 0, sizeof(cmd_buf));
			cmd_scan = 0;
			continue;
		}
		if(ed_mode == 0){
			if(c == 0x11 && cmd_scan > 0) cmd_scan--;
			else if(c == 0x12) cmd_scan++;
			else if(c >= 32 && c <= 126){
				cmd_buf[cmd_scan] = c;
				cmd_scan++;
			}
			else if(c == 0xa){
				//print(cmd_buf);
				if(cmd_buf[0] == 'R'){
					ed_mode = 1;
				}
				if(cmd_buf[0] == 'I'){
					ed_mode = 2;
				}
				else if(cmd_buf[0] == 'w'){
					fextend(text, chars_added);
					chars_added = 0;
					//text->head = 0;
					for(int i = 0; i < size; i++){
						//text->head = i;
						fseek(text, i);
						fputc(text, file_buf[i]);
					}
					fseek(text, 0);
					//text->head = 0;
					fcopy(text, file_buf, size);
				}
				else if(cmd_buf[0] == 'q'){
					break;
				}
				cmd_scan = 0;
				memset(cmd_buf, 0, sizeof(cmd_buf));
			}
		}
		else if(ed_mode == 1){
			
			if(c == 0x13){
				if(line == 0) continue;
				line--;
				while(file_buf[scan] != '\n'){
					scan--;
				}
				scan--;
			}
			else if(c == 0x14){
				line++;
				while(file_buf[scan] != '\n'){
					scan++;
				}
				scan++;
			}
			else if(c == 0x11) scan--;
			else if(c == 0x12) scan++;
			else{
				file_buf[scan] = c;
				//fseek(text, scan);
				//fputc(text, c);
				scan++;
			}
		}
		else if(ed_mode == 2){
			if(c == 0x13){
				if(line == 0) continue;
				line--;
				while(file_buf[scan] != '\n'){
					scan--;
				}
				scan--;
			}
			else if(c == 0x14){
				line++;
				while(file_buf[scan] != '\n'){
					scan++;
				}
				scan++;
			}
			else if(c == 0x11) scan--;
			else if(c == 0x12) scan++;
			else if(c == 0x08){
				size--;
				if(chars_added != 0) chars_added--;
				for(int i = scan-1; i < size; i++){
					file_buf[i] = file_buf[i+1];
				}
				scan--;
			}
			else{
				//fextend(text, 10);
				chars_added++;
				size++;
				for(int i = size; i > scan; i--){
					file_buf[i] = file_buf[i-1];
				}
				file_buf[scan] = c;
				scan++;
			}
		}
	}
	fclose(text);	

}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
	}
}

void render_mode(){
	int x = 0;
	char *mode_str = mode_strs[ed_mode];
	while(*mode_str){
		drawChar((x++)*8, window->height - 8, *mode_str++);
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
	for(; idx < size && y < window->height / 8 - 2; idx++){
		if(buf[idx] == '\n'){
			x = 0;
			y++;
			if(idx == scan){
				for(int i = 0; i < 8; i++){
					win_buf[x*8+i + (y*8+7)*window->width] = 0xFF0000;
				}
			}
			continue;
		}
		else if(buf[idx] == '\t'){
			x+=2;
			continue;
		}
		if(x >= window->width / 8){
			x = 0;
			y++;
		}
		drawChar(x*8, y*8, buf[idx]);

		if(idx == scan){
			for(int i = 0; i < 8; i++){
				win_buf[x*8+i + (y*8+7)*window->width] = 0xFF0000;
			}
		}

		x++;
	}
}