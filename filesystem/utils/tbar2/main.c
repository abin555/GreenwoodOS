#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <sys/io.h>
#include <sys/task.h>
#include <string.h>
#include <sys/mouse.h>
#include <sys/memory.h>
#include "tbar.h"
#include "lexer.h"

struct DesktopConfig {
    int vp_root_x;
    int vp_root_y;
    int screen_w;
    int screen_h;
};

uint32_t width;
uint32_t height;
uint32_t bufsize;
uint32_t *backbuf;

// Scans the token stream for "name" "exec" "args" "icon" ENDL groups and
// adds each as a taskbar entry.
void parse(struct Taskbar *taskbar, struct TokenStream *s){
    int i = 0;
    while(i < s->ntokens){
        int have_full_entry =
            i <= s->ntokens - 5 &&
            s->tokens[i + 0].type == STR &&
            s->tokens[i + 1].type == STR &&
            s->tokens[i + 2].type == STR &&
            s->tokens[i + 3].type == STR &&
            s->tokens[i + 4].type == ENDL;

        if(have_full_entry){
            if(taskbar->numEntries >= TBAR_MAX_ENTRIES){
                printf("[TBAR] Config defines more than %d entries, ignoring the rest\n", TBAR_MAX_ENTRIES);
                break;
            }
            tbar_entry_init(
                &taskbar->entries[taskbar->numEntries++],
                s->tokens[i + 0].str,
                s->tokens[i + 1].str,
                s->tokens[i + 2].str,
                s->tokens[i + 3].str
            );
            i += 5; // Skip past the entry we just consumed instead of rescanning it.
        } else {
            i++;
        }
    }
}

// Reads and parses a taskbar config file, populating tbar. Failures are
// logged and simply leave the taskbar with fewer (or zero) entries.
void load_taskbar_config(struct Taskbar *tbar, const char *path){
    FILE *conf = fopen(path, "r");
    if(conf == NULL){
        printf("[TBAR] Could not open config file '%s'\n", path);
        return;
    }

    fseek(conf, 0, SEEK_END);
    long size = ftell(conf);
    fseek(conf, 0, SEEK_SET);

    if(size <= 0){
        printf("[TBAR] Config file '%s' is empty\n", path);
        fclose(conf);
        return;
    }

    char *buf = malloc(size + 1);
    if(buf == NULL){
        printf("[TBAR] Out of memory reading config\n");
        fclose(conf);
        return;
    }
    memset(buf, 0, size + 1);
    fread(buf, size, 1, conf);
    fclose(conf);

    printf("[TBAR] Read %d byte config\n", size);
    struct TokenStream s = lex(buf, (int)size);
    if(s.valid){
        parse(tbar, &s);
    } else {
        printf("[TBAR] Config file failed to parse, ignoring\n");
    }

    freeTokenStream(&s);
    free(buf);
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_CLICK){
        // Wake the task so the main loop can process the click on its next run.
        set_schedule(ALWAYS);
    }
    if(event == VP_MINIMIZE){
        // Bug fix: the original code set vp->minimized = 0 while still
        // shrinking loc.w/h to the minimized size, leaving the flag and the
        // geometry out of sync. A taskbar shouldn't be minimizable at all,
        // so we simply ignore the request.
    }
}

// Sets up the desktop root position, opens the taskbar viewport centered at
// the bottom of the screen, and restores the original desktop config.
struct Viewport *create_taskbar_viewport(uint32_t vp_width, uint32_t vp_height){
    int screen_fd = open("/-/sys/screen", O_READ | O_WRITE);
    if(screen_fd < 0){
        printf("[TBAR] Could not open /-/sys/screen\n");
        return NULL;
    }

    struct DesktopConfig conf;
    read(screen_fd, &conf, sizeof(conf));
    printf("[TBAR] Screen is %dx%d\n", conf.screen_w, conf.screen_h);

    struct DesktopConfig mutDesktopConfig = conf;
    mutDesktopConfig.vp_root_x = (conf.screen_w / 2) - (vp_width / 2);
    mutDesktopConfig.vp_root_y = conf.screen_h - (TBAR_HEIGHT + 10);
    lseek(screen_fd, 0, SEEK_SET);
    write(screen_fd, &mutDesktopConfig, sizeof(mutDesktopConfig));

    struct Viewport *vp = vp_open(vp_width, vp_height, "Taskbar");

    lseek(screen_fd, 0, SEEK_SET);
    write(screen_fd, &conf, sizeof(conf)); // Restore the original root position.
    close(screen_fd);

    return vp;
}

int main(int argc, char **argv){
    freopen("/-/dev/serial", "w+", stdout);
    struct Taskbar *tbar = tbar_init();
    if(tbar == NULL){
        printf("[TBAR] Failed to initialize taskbar, exiting\n");
        return 1;
    }

    if(argc == 2){
        load_taskbar_config(tbar, argv[1]);
    }

    width = TBAR_MAXICONS * TBAR_HEIGHT;
    height = TBAR_HEIGHT;

    struct Viewport *tbar_vp = create_taskbar_viewport(width, height);
    if(tbar_vp == NULL){
        printf("[TBAR] Failed to create taskbar viewport, exiting\n");
        return 1;
    }

    vp_set_options(tbar_vp, VP_OPT_NODECORATOR);
    bufsize = sizeof(uint32_t) * width * height; // Bug fix: was sizeof(uint32_t*), which
                                                  // over-allocated on 64-bit systems.
    backbuf = malloc(bufsize);
    if(backbuf == NULL){
        printf("[TBAR] Out of memory allocating backbuffer\n");
        return 1;
    }
    vp_set_buffer(tbar_vp, backbuf, bufsize);

    tbar_draw(tbar, backbuf, width, height, -1);
    vp_copy(tbar_vp);

    tbar_vp->click_events_enabled = 1;
    vp_add_event_handler(tbar_vp, event_handler);

    FILE *mouse = fopen("/-/sys/mouse", "r");
    if(mouse == NULL){
        printf("[TBAR] Could not open /-/sys/mouse, exiting\n");
        return 1;
    }
    struct MouseStatus mouseStatus;

    set_schedule(NEVER);
    yield();

    while(1){
        fseek(mouse, 0, SEEK_SET);
        fread(&mouseStatus, sizeof(mouseStatus), 1, mouse);

        int relx = mouseStatus.pos.x - tbar_vp->loc.x;
        int rely = mouseStatus.pos.y - tbar_vp->loc.y;

        // Bug fix: the original code checked only `rely > 0` (true almost
        // any time the mouse is below the viewport's top edge, with no
        // upper bound) and computed the icon index as
        // `(relx / TBAR_HEIGHT) - 1`, an off-by-one that could go negative.
        // Neither the x range nor the resulting index were ever checked
        // against the real bounds, so a stray click near the taskbar could
        // read outside the entries array.
        if(relx >= 0 && (uint32_t)relx < width && rely >= 0 && (uint32_t)rely < height){
            int idx = tbar_index_from_x(relx);
            if(idx >= 0 && idx < tbar->numEntries){
                printf("[TBAR] Launching %s\n", tbar->entries[idx].name);

                // Flash the icon as "active" before handing off control.
                tbar_draw(tbar, backbuf, width, height, idx);
                vp_copy(tbar_vp);

                exec(tbar->entries[idx].exec_filename, 0, NULL);
            }
        }

        set_schedule(NEVER);
        yield();
    }
    return 0;
}
