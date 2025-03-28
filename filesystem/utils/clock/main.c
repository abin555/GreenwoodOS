#include <stdio.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <sys/task.h>
#include <sys/io.h>
#include <string.h>

#define WIDTH 8*10
#define HEIGHT 3*8

#define BUF_SIZE WIDTH * HEIGHT * sizeof(uint32_t)

struct RealTimeClock {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;
	unsigned int msec;
};

struct Viewport *win;
uint32_t *backbuffer;
int running;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

struct RealTimeClock rtc;

int drawDecimal(int data, int x, int y);

int main(int argc, char **argv){

	backbuffer = (uint32_t *) malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
    memset(backbuffer, 0, sizeof(uint32_t) * WIDTH * HEIGHT);

    int clock_fd = open("/-/dev/RTC", O_READ);
    if(clock_fd == -1){
        printf("Unable to open clock RTC file!\n");
        return 1;
    }

	win = vp_open(WIDTH, HEIGHT, "CLOCK");
	vp_add_event_handler(win, event_handler);
	vp_set_buffer(win, backbuffer, BUF_SIZE);


	running = 1;
	while(running){
        read(clock_fd, &rtc, sizeof(rtc));
		drawDecimal(rtc.hour, 1*8, 8);
		vp_drawChar(win, 3*8, 8, ':', 0xFFFFFF, 0);
		drawDecimal(rtc.minute, 4*8, 8);
		vp_drawChar(win, 6*8, 8, ':', 0xFFFFFF, 0);
		drawDecimal(rtc.second, 7*8, 8);
		vp_copy(win);
        yield();
	}
	vp_close(win);
    close(clock_fd);
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

int drawDecimal(int data, int x, int y){
    int idx = 0;
    int pow = 1;

    if(data < 0){
		vp_drawChar(win, x, y, '-', 0xFFFFFF, 0x0);
		x+=8;
        idx++;
        data *= -1;
    }

	if(data < 10){
		vp_drawChar(win, x, y, '0', 0xFFFFFF, 0x0);
		x+=8;
	}

    while(pow * 10 <= data)
        pow *= 10;
    while(pow != 0){
        if(pow == 0) return 0;
        int d = data / pow;
        vp_drawChar(win, x, y, '0' + d, 0xFFFFFF, 0x0);
		x+=8;
        data = data - d * pow;
        pow /= 10;
        if(pow == 0) return 0;
        idx++;
    }
    return idx;
}