#include "console.h"

struct CONSOLE consoles[MAX_CONSOLE] = {0};
struct CONSOLE *kernel_console = NULL;

void console_init(){
	print_serial("[CONSOLE] Initialization\n");
	for(int i = 0; i < MAX_CONSOLE; i++){
		struct CONSOLE *console = &consoles[i];
		console->window = NULL;
		console->width = fb_width / CHAR_W;
		console->height = fb_height / CHAR_H - CHAR_H;
		console->buf_size = consoles[i].width * consoles[i].height;
		console->buf = malloc(consoles[i].buf_size);
		console->active = false;
		console->cursor = 0;
	}
	kernel_console = NULL;	
}

void console_drawFull(struct CONSOLE *console){
	if(console == NULL) return;

	for(uint32_t indexY = console->LastLine; indexY < console->Line+1; indexY++){
		char clearLine = 0;
		for(uint32_t indexX = 0; indexX < console->width; indexX++){
			char sym = console->buf[indexX + indexY * console->width];
			if(sym != '\0' && sym != '\n' && !clearLine){
				if(console->view_type == CONSOLE_WINDOW){
					buf_write_cell(
						console->window->backbuffer, 
						console->width*8, 
						indexX + indexY * console->width, 
						sym, 
						console->color.fg, 
						console->color.bg
					);
				}
				else if(console->view_type == CONSOLE_VIEWPORT){
					buf_write_cell(
						console->viewport->backbuf, 
						console->width*8, 
						indexX + indexY * console->width, 
						sym, 
						console->color.fg, 
						console->color.bg
					);
				}
			}
			else{
				clearLine = 1;
			}
		}
	}
	console->LastLine = console->Line-1;
}

void console_putLine(struct CONSOLE *console, uint32_t places){
	for(uint32_t index = 0; index < places; index++){
		buf_write_cell(
			console->window->backbuffer,
			console->width*8,
			console->Line*console->width + index,
			console->buf[console->Line*console->width + index],
			console->color.fg, 
			console->color.bg
		);
	}
}

void shiftConsoleUp(struct CONSOLE *console){
	console->LastLine = 0;
	for(uint32_t places = 0; places < console->width; places++){
		for(uint32_t i = 0; i < console->height*console->width-1; i++){
			console->buf[i] = console->buf[i+1];
		}
	}
}

int calculateNumberLength(unsigned int data, int base){
	switch(base){
		case(16):
			if(data > 0x0FFFFFFF){
				return 8;
			}
			else if(data > 0x00FFFFFF){
				return 8;
			}
			else if(data > 0x000FFFFF){
				return 8;
			}
			else if(data > 0x0000FFFF){
				return 8;
			}
			else if(data > 0x00000FFF){
				return 4;
			}
			else if(data > 0x000000FF){
				return 4;
			}
			else if(data > 0x0000000F){
				return 2;
			}
			return 1;
		break;
		case(10):
			return 8;
		break;
		case(2):
		return sizeof(unsigned int);
	}
	return 8;
}

int printDecimal(struct CONSOLE *console, int data, int offset){
	int idx = 0;
	int pow = 1;
	if(data < 0){
		console->buf[console->Line*console->width + console->LinePlace + idx + offset] = '-';
		idx++;
		data *= -1;
	}
	while(pow * 10 <= data)
		pow *= 10;
	while(pow != 0){
		int d = data / pow;
		console->buf[console->Line*console->width + console->LinePlace + idx + offset] = (char)((int)'0' + d);
		data = data - d * pow;
		pow /= 10;
		idx++;
	}
	return idx;
}

int printHex(struct CONSOLE *console, unsigned int data, int setlength){
	for(int i = 0; i < (setlength ? setlength : 8); i++){
		console->buf[console->Line*console->width + console->LinePlace + (setlength ? setlength : 8) - 1 - i] = quadToHex((data >> 4*i) & 0xF);
	}
	return (setlength ? setlength : 8);
}

int printBinary(struct CONSOLE *console, unsigned int data, int setlength){
	for(int i = 0; i < (setlength ? setlength : 32); i++){
		if((data >> i) & 1){
			console->buf[console->Line*console->width + console->LinePlace + i] = '1';
		}
		else{
			console->buf[console->Line*console->width + console->LinePlace + i] = '0';
		}
	}
	return 32;
}

float fabs(float x){
	if(x < 0) return -1 * x;
	return x;
}
int abs(int x){
	if(x < 0) return -1 * x;
	return x;
}

int printFloat(struct CONSOLE *console, uint32_t data){
	print_serial("Printing float %x\n", data);
	float dataf = (float) data;
	int top = (int) dataf;
	float lower = dataf - top;
	int size1 = printDecimal(console, top, 0);
	console->LinePlace += size1;
	int iLower = (int) (lower * 1000);
	console->buf[console->Line*console->width + console->LinePlace + size1] = '.';
	console->LinePlace++;
	print_serial("%d.%d\n", top, iLower);
	int size2 = printDecimal(console, iLower, 0);
	console->LinePlace += size2;
	return 0;

}

struct CONSOLE *console_open(struct WINDOW *window){
	struct CONSOLE *console = NULL;
	for(int i = 0; i < MAX_CONSOLE; i++){
		if(!consoles[i].active){
			console = &consoles[i];
			break;
		}
	}
	if(console == NULL || window == NULL) return NULL;
	memset(console->buf, 0, console->buf_size);
	console->view_type = CONSOLE_WINDOW;
	console->active = true;
	console->cursor = 0;
	console->last_cursor = 0;
	console->window = window;
	console->viewport = NULL;
	console->width = console->window->width / CHAR_W;
	console->height = console->window->height / CHAR_H - CHAR_H;

	console->Line = 0;
	console->Start = 0;
	console->LinePlace = 0;
	console->LastLine = 0;
	console->color.fg = 0xFFFFFF;
	console->color.bg = 0;
	return console;
}

struct CONSOLE *console_open_vp(struct Viewport *vp){
	struct CONSOLE *console = NULL;
	for(int i = 0; i < MAX_CONSOLE; i++){
		if(!consoles[i].active){
			console = &consoles[i];
			break;
		}
	}
	if(console == NULL || vp == NULL || vp->backbuf == NULL) return NULL;
	memset(console->buf, 0, console->buf_size);
	console->view_type = CONSOLE_VIEWPORT;
	console->active = true;
	console->cursor = 0;
	console->last_cursor = 0;
	console->window = NULL;
	console->viewport = vp;
	memset(console->viewport->backbuf, 0, console->viewport->buf_size);
	console->width = console->viewport->loc.w / CHAR_W;
	console->height = console->viewport->loc.h / CHAR_H - CHAR_H;

	console->Line = 0;
	console->Start = 0;
	console->LinePlace = 0;
	console->LastLine = 0;
	console->color.fg = 0xFFFFFF;
	console->color.bg = 0;

	print_serial("[CONSOLE] Open Viewport console for %s %dx%d\n", console->viewport->title, console->width, console->height);
	return console;
}

void console_close(struct CONSOLE *console){
	console->cursor = 0;
	console->active = false;
}

void console_clear(struct CONSOLE *console){
	if(console == NULL) return;
	memset(console->buf, 0, console->buf_size);
	console->Line = 0;
	console->Start = 0;
	console->LinePlace = 0;
	console->LastLine = 0;
}

void print_console(struct CONSOLE *console, char *msg, ...){
	if(console == NULL) return;
	if(msg == NULL){
		print_serial("MSG is null!\n");
		return;
	}
	uint32_t p = 0;
	char allowed = 1;
	va_list listptd;
	va_start(listptd, msg);
	//printChar(p, consoleLine-consoleStart, msg[p]);
	//memset(console->buf + consoleStart*fb_terminal_w - consoleLine*fb_terminal_w, 0, fb_terminal_w);
	while(p < (unsigned int)console->width){
		int setlength = 0;
		if(msg[p] == '%'){
			parseaftermodify:;
			p++;
			switch(msg[p]){
				case 'h'://Hexadecimal
				case 'H':
				case 'x':
				case 'X':
					console->LinePlace += printHex(console, va_arg(listptd, unsigned int), setlength);
				break;
				case 'b'://Binary
				case 'B':
					console->LinePlace += printBinary(console, va_arg(listptd, unsigned int), setlength);
				break;
				case 'd'://Decimal
				case 'D':
					console->LinePlace += printDecimal(console, va_arg(listptd, int), 0);
				break;
				case 'f'://float
				case 'F':
					console->LinePlace += printFloat(console, va_arg(listptd, unsigned int));
				break;
				case 'c':
				case 'C':
					//console->LinePlace += va_arg(listptd, char);
					console->buf[console->Line*console->width + console->LinePlace] = (char) va_arg(listptd, unsigned int);
					console->LinePlace += 1;
					break;
				case 's':
				case 'S': {
					char *str = (char *) va_arg(listptd, unsigned int);
					while(*str != '\0'){
						console->buf[console->Line*console->width + console->LinePlace] = *str;
						console->LinePlace += 1;
						str++;
					}
				}
				break;
				default:
				if(msg[p] == '1' && msg[p+1] == '6'){
					setlength = 16;
					p++;
					goto parseaftermodify;
				}
				else if(msg[p] == '3' && msg[p+1] == '2'){
					setlength = 32;
					p++;
					goto parseaftermodify;
				}
				else if(msg[p] == '1'){
					setlength = 1;
					goto parseaftermodify;
				}
				else if(msg[p] == '2'){
					setlength = 2;
					goto parseaftermodify;
				}
				else if(msg[p] == '3'){
					setlength = 3;
					goto parseaftermodify;
				}
				else if(msg[p] == '4'){
					setlength = 4;
					goto parseaftermodify;
				}
				else if(msg[p] == '5'){
					setlength = 5;
					goto parseaftermodify;
				}
				else if(msg[p] == '6'){
					setlength = 6;
					goto parseaftermodify;
				}
				else if(msg[p] == '7'){
					setlength = 7;
					goto parseaftermodify;
				}
				else if(msg[p] == '8'){
					setlength = 8;
					goto parseaftermodify;
				}
			}
		}
		else if(msg[p] != '\0' && allowed){
			
			console->buf[console->Line*console->width + console->LinePlace] = msg[p];
			if(msg[p] == '\n'){
				console->LinePlace = 0;          
				if(console->Line == (unsigned int) console->height-2){
					if(console->view_type == CONSOLE_WINDOW){
						memset(console->window->backbuffer, 0, console->window->width * console->window->height * sizeof(uint32_t));
					}
					else if(console->view_type == CONSOLE_VIEWPORT){
						memset(console->viewport->backbuf, 0, console->viewport->buf_size);
					}
					shiftConsoleUp(console);					
					//memset(console->buf, 0, console->window->width*console->window->height);
					//console->Line = 0;
					//console->LastLine = 0;
				}
				else{
					console->Line++;  
				}
			}
			else{
				console->LinePlace++;
			}
		}
		else if(msg[p] == '\0'){
			//console->buf[consoleLine*fb_terminal_w + cp] = msg[p];
			allowed = 0;
			break;
		}
		else{
			//console->buf[consoleLine*fb_terminal_w + cp] = 0;
		}
		p++;
	}

	console_drawFull(console);
	//console_putLine(p);
	//fb_setChar(0, consoleLine, '*', 0xFFFFFF, 0);
	va_end(listptd);
	/*
	for(int loop1 = 0; loop1 < 0xFFFF; loop1++){
		for(int loop2 = 0; loop2 < 0x6FF; loop2++){

		}
	}*/
	if(console->view_type == CONSOLE_VIEWPORT){
		viewport_copy_buffer(console->viewport);
	}
}

char quadToHex(uint8_t quad){
	switch(quad){
	case 0x00:
		return '0';
		break;
	case 0x01:
	case 0x10:
		return '1';
		break;
	case 0x02:
	case 0x20:
		return '2';
		break;
	case 0x03:
	case 0x30:
		return '3';
		break;
	case 0x04:
	case 0x40:
		return '4';
		break;
	case 0x05:
	case 0x50:
		return '5';
		break;
	case 0x06:
	case 0x60:
		return '6';
		break;
	case 0x07:
	case 0x70:
		return '7';
		break;
	case 0x08:
	case 0x80:
		return '8';
		break;
	case 0x09:
	case 0x90:
		return '9';
		break;
	case 0x0a:
	case 0xa0:
		return 'A';
		break;
	case 0x0b:
	case 0xb0:
		return 'B';
		break;
	case 0x0c:
	case 0xc0:
		return 'C';
		break;
	case 0x0d:
	case 0xd0:
		return 'D';
		break;
	case 0x0e:
	case 0xe0:
		return 'E';
		break;
	case 0x0f:
	case 0xf0:
		return 'F';
		break;
	}
	return 'x';
}