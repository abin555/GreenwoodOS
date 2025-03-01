#include "libc.h"

struct AudioInterface {
    void (*set_volume)(uint8_t volume);
    void (*stop)();
    void *(*wav_read)(uint8_t * wav_memory, uint32_t len);
    uint32_t (*wav_play)(void *wav, uint32_t offset);
};

int running = 1;
int stop = 0;

int click = 0;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_FOCUSED || event == VP_MAXIMIZE){
        //set_schedule(ALWAYS);
    }
    else if(event == VP_UNFOCUSED || event == VP_MINIMIZE){
        //set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        dprint("Exit!\n");
        stop = 1;
        running = 0;
        set_schedule(ALWAYS);
    }
    else if(event == VP_CLICK){
        click = 1;
    }
}

int main(int argc, char **argv){
    struct AudioInterface audio;
    struct ViewportFunctions vp_funcs;
    click = 0;
    if(argc != 2){
        return 1;
    }
    if(argv[1] == NULL) return 1;

    int audio_fd = open("/-/sys/audio", O_READ);
    if(audio_fd == -1) return 1;
    int vp_fd = open("/-/sys/viewport", O_READ);
    if(vp_fd == -1) return 1;

    int nread;

    nread = read(audio_fd, &audio, sizeof(audio));
    if(nread != sizeof(audio)){
        close(audio_fd);
        close(vp_fd);
        return 1;
    }

    nread = read(vp_fd, &vp_funcs, sizeof(vp_funcs));
    if(nread != sizeof(vp_funcs)){
        close(audio_fd);
        close(vp_fd);
        return 1;
    }

    close(audio_fd);
    close(vp_fd);

    struct Viewport *vp = vp_funcs.open(120, 300, "Music Player");
    set_schedule(ALWAYS);
    running = 1;
    vp_funcs.add_event_handler(vp, event_handler);
    void *file_buf;
    int size;
    int music_fd = open(argv[1], O_READ);
    if(music_fd != -1){
        size = lseek(music_fd, 0, 2) / 2;
        lseek(music_fd, 0, 0);
        file_buf = (void*) requestRegion(size);
        read(music_fd, file_buf, size);
        audio.set_volume(100);
        audio.wav_play(audio.wav_read(file_buf, size), 0);
    }
    else{
        return 1;
    }
    running = 1;
    stop = 0;

    set_schedule(NEVER);
    yield();
    dprint("Hopefully not yet!\n");

    vp_funcs.close(vp);
    audio.stop();
    freeRegion(file_buf, size);
    write(open("/-/dev/serial", O_WRITE), "Music Closed\n", 14);
}
