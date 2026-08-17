#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/io.h>
#include <sys/vp.h>
#include <sys/task.h>
#include <sys/memory.h>
#include <stdbool.h>
#include <sys/mouse.h>


struct AudioInterface {
    void (*set_volume)(uint8_t volume);
    void (*stop)();
    void *(*wav_read)(uint8_t * wav_memory, uint32_t len);
    uint32_t (*wav_play)(void *wav, uint32_t offset);
};
struct AudioInterface audio;

struct DesktopConfig {
    int vp_root_x;
    int vp_root_y;
    int screen_w;
    int screen_h;
};

int running = 1;
int stop = 0;
int click = 0;

int volume = 50;
int mouseX, mouseY;

enum {
    HOVER_NONE,
    HOVER_TOP,
    HOVER_BOTTOM
} hoverArea = HOVER_NONE;

void vol_up(){
    volume++;
    if(volume > 100) volume = 100;
    audio.set_volume(volume);
}

void vol_down(){
    volume--;
    if(volume < 0) volume = 0;
    audio.set_volume(volume);
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_FOCUSED || event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
    }
    else if(event == VP_UNFOCUSED || event == VP_MINIMIZE){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        stop = 1;
        running = 0;
        set_schedule(ALWAYS);
    }
    else if(event == VP_CLICK){
        click = 1;
        printf("VP CLICK\n");
        if(hoverArea == HOVER_TOP){
            vol_up();
        }
        else if (hoverArea == HOVER_BOTTOM){
            vol_down();
        }
    }
}

int drawDecimal(struct Viewport *win, int data, int x, int y){
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

struct IVec2 getRelativeMouse(struct MouseStatus *mouseStatus, struct Viewport * vp){
    struct IVec2 pos;
    pos.x = mouseStatus->pos.x - vp->loc.x;
    pos.y = mouseStatus->pos.y - vp->loc.y - 8;
    return pos;
}

#define WIDTH 3 * 8
#define HEIGHT 5 * 8

#define COL_A 0x000000
#define COL_B 0xaaaaaa

int main(int argc, char **argv){
    freopen("/-/dev/serial", "w", stdout);
    click = 0;

    int audio_fd = open("/-/sys/audio", O_READ);
    if(audio_fd == -1) return 1;

    int nread;

    nread = read(audio_fd, &audio, sizeof(audio));
    if(nread != sizeof(audio)){
        close(audio_fd);
        return 1;
    }
    close(audio_fd);

    int screen_fd = open("/-/sys/screen", O_READ);
    struct DesktopConfig conf;
    read(screen_fd, &conf, sizeof(struct DesktopConfig));
    printf("Screen is %dx%d\n", conf.screen_w, conf.screen_h);
    close(screen_fd);

    int mouse_fd = open("/-/sys/mouse", O_READ);
    struct MouseStatus mouse;
    read(mouse_fd, &mouse, sizeof(mouse));

    struct Viewport *vp = vp_open(WIDTH, HEIGHT, "Audio Control");
    vp->click_events_enabled = true;
    vp->options |= VP_OPT_NODECORATOR;

    vp->loc.x = 0;
    vp->loc.y = conf.screen_h - vp->loc.h - 8;

    uint32_t *backbuf = malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
    vp_set_buffer(vp, backbuf, sizeof(uint32_t) * WIDTH * HEIGHT);
    set_schedule(ALWAYS);
    running = 1;
    vp_add_event_handler(vp, event_handler);

    audio.set_volume(volume);

    while(running){
        lseek(mouse_fd, 0, 0);
        read(mouse_fd, &mouse, sizeof(mouse));
        struct IVec2 relMouse = getRelativeMouse(&mouse, vp);
    
        if(relMouse.y > 0 && relMouse.y <= vp->loc.h && relMouse.x > 0 && relMouse.x <= vp->loc.w){
            if(relMouse.y < 8) hoverArea = HOVER_TOP;
            else if(relMouse.y > 24) hoverArea = HOVER_BOTTOM;
            else hoverArea = HOVER_NONE;
        }
        else{
            hoverArea = HOVER_NONE;
        }

        vp_drawChar(
            vp, 4, 0, '/',
            hoverArea == HOVER_TOP ? COL_B : COL_A,
            hoverArea == HOVER_TOP ? COL_A : COL_B
        );
        vp_drawChar(
            vp, 12, 0, '\\',
            hoverArea == HOVER_TOP ? COL_B : COL_A,
            hoverArea == HOVER_TOP ? COL_A : COL_B
        );
        drawDecimal(vp, volume, 24, 12);

        vp_drawChar(
            vp, 4, 24, '\\',
            hoverArea == HOVER_BOTTOM ? COL_B : COL_A,
            hoverArea == HOVER_BOTTOM ? COL_A : COL_B
        );
        vp_drawChar(
            vp, 12, 24, '/',
            hoverArea == HOVER_BOTTOM ? COL_B : COL_A,
            hoverArea == HOVER_BOTTOM ? COL_A : COL_B
        );

        if(vp->ascii == 'w'){
            vol_up();
        }
        if(vp->ascii == 's'){
            vol_down();
        }
        vp->ascii = 0;
        
        vp_copy(vp);
        yield();
    }

    vp_close(vp);
}