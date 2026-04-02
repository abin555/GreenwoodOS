#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/vp.h>
#include <sys/console.h>
#include <sys/task.h>
#include <sys/io.h>

#include "client.h"


#define WIDTH 496
#define HEIGHT 418

void draw_cursor(int i);
//struct Viewport *vp;
uint32_t *backbuf;
//struct CONSOLE *console;
int term_width;
int term_height;

struct LISP_DRIVER {
    Atom *env;

    int (*env_set)(Atom, Atom, Atom);
    int (*env_get)(Atom env, Atom symbol, Atom *result);
    int (*listp)(Atom expr);
    Atom (*cons)(Atom car_val, Atom cdr_val);
    Atom (*make_int)(int x);
    Atom (*make_sym)(const char *s);
    Atom (*make_string)(char *s);
    Atom (*make_builtin)(Builtin fn);
    Atom (*make_real)(float real);
    Atom (*copy_list)(Atom list);
    int (*make_closure)(Atom env, Atom args, Atom body, Atom *result);
    int (*apply)(Atom fn, Atom args, Atom *result);
    int (*read_expr)(const char *input, const char **end, Atom *result);
    int (*eval_expr)(Atom expr, Atom env, Atom *result);
};

struct LISP_DRIVER driver;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);
int running;

//static char input[2048];

int main(int argc, char **argv){
    FILE *lisp_driver_file = fopen("/-/lisp/env", "r");
    if(lisp_driver_file == NULL){
        printf("Error: lisp engine not running!\n");
        return 1;
    }
    fread(&driver, sizeof(driver), 1, lisp_driver_file);
    fclose(lisp_driver_file);

    printf("Opening Greenwood LISP Client\n");

    Atom expr, result;
    const char *p = "(* 4 5)";
    Error err;

    running = 1;
    if(argc == 2 && argv[1] != NULL){
        load_file(driver.env, argv[1]);
    }
    if(running == 0){
        return 0;
    }

    /*
    vp = vp_open(WIDTH, HEIGHT, "Greenwood LISP Client");
    backbuf = (uint32_t *) malloc(WIDTH * HEIGHT *sizeof(uint32_t));
    memset(backbuf, 0, WIDTH * HEIGHT *sizeof(uint32_t));
	vp_set_buffer(vp, backbuf, WIDTH * HEIGHT * sizeof(uint32_t));
    vp_add_event_handler(vp, event_handler);
    vp->loc.h = 400;
    console = console_open_vp(vp);
    vp->loc.h = 418;

	term_width = WIDTH / 8;
	term_height = HEIGHT / 8;
    */

    printf("Greenwood LISP Version 0.3\n");

    int idx = 0;
    char termbuf[100];
    memset(termbuf, 0, sizeof(termbuf));
    char readbuf[1];

    while (running) {
        int n = read(0, readbuf, 1);
        if (n != 1) {
            yield();
            continue;
        }

        char c = readbuf[0];

        if (c == '\n') {
            printf("\n");
            //termbuf[idx] = '\0';
            p = termbuf;
            err = read_expr(p, &p, &expr);

            if (!err)
                err = eval_expr(expr, driver.env, &result);

            switch (err) {
                case Error_OK:
                    print_expr(result);
                    printf("\n");
                    break;
                case Error_Syntax:
                    printf("Syntax Error\n");
                    break;
                case Error_Unbound:
                    printf("Symbol not bound\n");
                    break;
                case Error_Args:
                    printf("Wrong number of arguments\n");
                    break;
                case Error_Type:
                    printf("Wrong Type\n");
                    break;
            }

            idx = 0;
            memset(termbuf, 0, sizeof(termbuf));

        } else if (c == 8 || c == 127) {
            if (idx > 0) {
                idx--;
                termbuf[idx] = '\0';
                fwrite("\b", 1, 1, stdout);
            }

        } else {
            if (idx < (int)sizeof(termbuf) - 1) {
                fwrite(&c, 1, 1, stdout);
                termbuf[idx++] = c;
            }
        }

        yield();
    }
    return 0;
}
