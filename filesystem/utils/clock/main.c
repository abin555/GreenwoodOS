#include <stdio.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <sys/task.h>
#include <sys/io.h>
#include <string.h>
#include <math.h>
#include "gfx.h"

#define WIDTH 8*10
#define HEIGHT 3*8

#define AWIDTH 140
#define AHEIGHT 140

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

void taskBarClock(int clock_fd);

void digitalClock(int clock_fd){
    backbuffer = (uint32_t *) malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
    memset(backbuffer, 0, sizeof(uint32_t) * WIDTH * HEIGHT);

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
}

void drawAngledLine(sGFXctx *ctx, float angle, int centerX, int centerY, float length){
  int xReal;
  int yReal;

  float x = cos(angle);
  float y = sin(angle);
  
  xReal = x*(length / 2) + centerX;
  yReal = y*(length / 2) + centerY;
  GFX_line(ctx, 0xFFFFFF, centerX, centerY, xReal, yReal);
}

int numberScreenWidth(int number){
    int width = 0;
    while(number > 1){
        width++;
        number / 10;
    }
    return width * 8;
}

#define DEG_TO_RAD(deg) (deg * M_PI / 180.0f)

void analogClock(int clock_fd){
    freopen("/-/dev/serial", "w", stdout);
    size_t backbuf_size = sizeof(uint32_t) * AWIDTH * AHEIGHT;
    backbuffer = (uint32_t *) malloc(backbuf_size);
    memset(backbuffer, 0, backbuf_size);

	win = vp_open(AWIDTH, AHEIGHT, "CLOCK"); 
	vp_add_event_handler(win, event_handler);
	vp_set_buffer(win, backbuffer, backbuf_size);
    win->transparent = 1;
    win->options |= VP_OPT_NODECORATOR;

    int cx, cy;

    cx = AWIDTH / 2;
    cy = AHEIGHT / 2;

    sGFXctx gctx;
    GFX_fromVP(&gctx, win);

    printf("Doing analog clock!\n");

    float hr_digit_angle = 360.0f / 12.0f;

    running = 1;
    while(running){      
        memset(backbuffer, 0, backbuf_size);
        GFX_line(&gctx, 0xFFFFFF, 0, 0, win->loc.w, 0);
        GFX_line(&gctx, 0xFFFFFF, 0, 0, 0, win->loc.h);
        GFX_line(&gctx, 0xFFFFFF, win->loc.w-1, 0, win->loc.w-1, win->loc.h-1);
        GFX_line(&gctx, 0xFFFFFF, 0, win->loc.h-1, win->loc.w-1, win->loc.h-1);

        for(int hr = 0; hr < 12; hr++){
            float x, y;
            x = cos(DEG_TO_RAD(hr_digit_angle * (hr+1)) - DEG_TO_RAD(90.0f));
            y = sin(DEG_TO_RAD(hr_digit_angle * (hr+1)) - DEG_TO_RAD(90.0f));
            int pos_x = x * ((AWIDTH - 24) / 2) + cx;
            int pos_y = y * ((AHEIGHT - 24) / 2) + cy;
            pos_x -= 8;
            pos_y -= 4;
            drawDecimal(hr+1, (int) pos_x, (int) pos_y);
        }
        

        read(clock_fd, &rtc, sizeof(rtc));

        float second_angle = ((float) rtc.second / 60.0f) * 2 * M_PI - DEG_TO_RAD(90.0f);
        float minute_angle = ((float) rtc.minute / 60.0f) * 2 * M_PI - DEG_TO_RAD(90.0f);
        float hour_angle = ((float) rtc.hour / 60.0f) * 2 * M_PI - DEG_TO_RAD(90.0f);

        drawAngledLine(&gctx, second_angle, cx, cy, AWIDTH - 58);
        drawAngledLine(&gctx, minute_angle, cx, cy, AWIDTH - 48);
        drawAngledLine(&gctx, hour_angle, cx, cy, (AWIDTH / 2) - 24);
        GFX_circle(&gctx, 0xFFFFFF, cx, cy, (AWIDTH-10) / 2);

        vp_copy(win);
        yield();
    }
}

int main(int argc, char **argv){
    freopen("/-/dev/serial", "w", stdout);

    int clock_fd = open("/-/dev/RTC", O_READ);
    if(clock_fd == -1){
        printf("Unable to open clock RTC file!\n");
        return 1;
    }

    if(argc == 2){
        printf("Clock via taskbar!\n");
        taskBarClock(clock_fd);
    }
    else{
        //digitalClock(clock_fd);
        analogClock(clock_fd);
    }	

    close(clock_fd);
    return 0;
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

int bufDecimal(int data, char *buf){
    int idx = 0;
    int pow = 1;
    int i = 0;

    if(data < 0){
		buf[i] = '-';
		i++;
        idx++;
        data *= -1;
    }

	if(data < 10){
		buf[i] = '0';
        i++;
	}

    while(pow * 10 <= data)
        pow *= 10;
    while(pow != 0){
        if(pow == 0) return 0;
        int d = data / pow;
        buf[i] = '0' + d;
        i++;
        data = data - d * pow;
        pow /= 10;
        if(pow == 0) return 0;
        idx++;
    }
    return idx;
}

void taskBarClock(int clock_fd){
    char wbuf[14];
    set_schedule(ONFOCUS);
    FILE *taskbar = fopen("/-/sys/winbar", "w");
    if(taskbar == NULL) return;

    while(1){
        read(clock_fd, &rtc, sizeof(rtc));

        memset(wbuf, 0, sizeof(wbuf));
        bufDecimal(rtc.hour, wbuf);
        wbuf[2] = ':';
        bufDecimal(rtc.minute, wbuf+3);
        wbuf[5] = ':';
        bufDecimal(rtc.second, wbuf+6);
        wbuf[8] = ' ';
        bufDecimal(rtc.month, wbuf+9);
        wbuf[11] = '/';
        bufDecimal(rtc.day, wbuf+12);

        fseek(taskbar, 0, SEEK_SET);
        fwrite(wbuf, sizeof(wbuf), 1, taskbar);


        yield();
    }
}