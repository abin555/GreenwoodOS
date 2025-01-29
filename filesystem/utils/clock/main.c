#include "libc.h"

#define WIDTH 8*10
#define HEIGHT 3*8

#define BUF_SIZE WIDTH * HEIGHT * sizeof(uint32_t)

struct Viewport *win;
uint32_t *backbuffer;
int running;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

uint8_t (*FONT)[128][8];

void putChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
int drawDecimal(int data, int x, int y);

int main(int argc, char **argv){
	struct ViewportFunctions *vp_funcs = viewport_get_funcs();
	struct RealTimeClock *rtc = get_rtc();
    FONT = getKernelFeature(FEAT_FONT).addr;

	if(rtc == NULL || vp_funcs == NULL){
		print("CLOCK FATAL ERROR: RTC OR VP IS NULL!\n");
        return 0;
	}

	backbuffer = (uint32_t *) 0x6000;

	for(int i = 0; i < WIDTH * HEIGHT; i++){
		backbuffer[i] = 0;
	}

	win = vp_funcs->open(WIDTH, HEIGHT, "CLOCK");
	vp_funcs->add_event_handler(win, event_handler);
	vp_funcs->set_buffer(win, backbuffer, BUF_SIZE);


	running = 1;
	while(running){
		drawDecimal(rtc->hour, 1*8, 8);
		putChar(3*8, 8, ':', 0xFFFFFF, 0);
		drawDecimal(rtc->minute, 4*8, 8);
		putChar(6*8, 8, ':', 0xFFFFFF, 0);
		drawDecimal(rtc->second, 7*8, 8);
		vp_funcs->copy(win);
	}
	vp_funcs->close(win);
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
    }   
}

void putChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg){
	for(int layer = 0; layer < 8; layer++){
		int yoff = WIDTH*(y+layer);
        for(int pixel = 0; pixel < 8; pixel++){
            backbuffer[yoff + x+pixel] = (((*FONT)[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}

int drawDecimal(int data, int x, int y){
    int idx = 0;
    int pow = 1;

    if(data < 0){
		putChar(x, y, '-', 0xFFFFFF, 0x0);
		x+=8;
        idx++;
        data *= -1;
    }

	if(data < 10){
		putChar(x, y, '0', 0xFFFFFF, 0x0);
		x+=8;
	}

    while(pow * 10 <= data)
        pow *= 10;
    while(pow != 0){
        if(pow == 0) return 0;
        int d = data / pow;
        putChar(x, y, '0' + d, 0xFFFFFF, 0x0);
		x+=8;
        data = data - d * pow;
        pow /= 10;
        if(pow == 0) return 0;
        idx++;
    }
    return idx;
}