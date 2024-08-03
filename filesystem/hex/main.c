#include "libc.h"

char *heap;
int heap_idx = 0;
uint32_t *win_buf;
struct WINDOW *window;
int size;
char *file_buf;

int scan;

void *alloc(int size);
void memset(void *mem, char v, int size);
void render_file(char *buf, int line);

#define BYTES_PER_LINE 16
#define marginX 5
#define marginY 2

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

	window = window_open("HexED", 0);
	win_buf = window->backbuffer;
	
	window_update();

	set_schedule(ONFOCUS);

    int line = 0;
	scan = 0;
    while(1){
        render_file(file_buf, line);


        window_update();
        char c = getc();
		if(c == 0x13 && line > 0){
			line--;
            if(scan >= 0){
                scan -= 2*BYTES_PER_LINE;
            }
		}
		else if(c == 0x14){
			line++;
            scan += 2*BYTES_PER_LINE;
		}
		else if(c == 0x11) scan--;
		else if(c == 0x12) scan++;
        if(scan > size * 2) scan = size;
        if(scan < 0) scan = 0;
        if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
            if(c >= '0' && c <= '9') c = c - '0';
            else if(c >= 'a' && c <= 'z')  c = c - 'a' + 0xA;
            else if(c >= 'A' && c <= 'Z') c = c - 'A' + 0xA;
            int idx = scan / 2;
            int offset = scan % 2;//if non zero, editing second half of byte.

            uint8_t byte = file_buf[idx];
            if(offset){
                byte = (byte & 0xF0) | (c & 0xF);
            }
            else{
                byte = (byte & 0x0F) | ((c & 0xF) << 4);
                //byte = byte >> 8 | ((c & 0xF) << 8);
                //byte = 0x12;
            }
            file_buf[idx] = byte;
            fseek(text, idx);
            fputc(text, byte);
            scan++;
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

/*

00 00 00 00 00 00 00 00 

*/



typedef unsigned char uint8_t;

char quadToChar(uint8_t quad){
    switch(quad & 0x0F){
        case 0x0:
            return '0';
        case 0x1:
            return '1';
        case 0x2:
            return '2';
        case 0x3:
            return '3';
        case 0x4:
            return '4';
        case 0x5:
            return '5';
        case 0x6:
            return '6';
        case 0x7:
            return '7';
        case 0x8:
            return '8';
        case 0x9:
            return '9';
        case 0xA:
            return 'A';
        case 0xB:
            return 'B';
        case 0xC:
            return 'C';
        case 0xD:
            return 'D';
        case 0xE:
            return 'E';
        case 0xF:
            return 'F';
    }
}

void print_byte(uint8_t byte, int x, int y){
    char part1 = quadToChar(byte >> 4);
    char part2 = quadToChar(byte & 0xF);

    drawChar(x*8, y*8, part1);
    drawChar((x+1)*8, y*8, part2);
}



void render_file(char *buf, int line){
    int idx = line * BYTES_PER_LINE;
    int x = marginX;
    int y = marginY;

    memset(win_buf, 0, window->width*window->height*sizeof(uint32_t));
    for(int margin = 0; margin < BYTES_PER_LINE; margin++){
        print_byte(margin, (margin*3) + marginX, 0);
    }
    for(int margin = 0; margin < (window->height / 8) - 2 * marginY; margin++){
        print_byte((margin+idx) >> 8, 0, margin + marginY);
        print_byte((margin+idx) & 0x00FF, 2, margin + marginY);
    }
    for(; idx < size && y < (window->height / 8) - marginY; idx++){
        print_byte(buf[idx], x, y);        

        if(idx == scan / 2){
            int offset = 0;
            if(scan % 2){
                offset = 8;
            }
            for(int i = 0; i < 8; i++){
                win_buf[x*8+i+offset + (y*8+7)*window->width] = 0xFF0000;
                win_buf[x*8+i+offset + (y*8-1)*window->width] = 0xFF0000;
            }
        }

        x+=3;
        drawChar((marginX + 3*BYTES_PER_LINE + 1 + ((x-marginX)/3))*8, y*8, buf[idx]);
        if(x == BYTES_PER_LINE * 3 + marginX){
            x = marginX;
            y++;
        }
    }


}