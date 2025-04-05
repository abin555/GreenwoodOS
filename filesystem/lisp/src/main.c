#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/vp.h>
#include <sys/console.h>
#include <sys/task.h>

#include "greenwood-lisp.h"
#include "builtin.h"


#define WIDTH 400
#define HEIGHT 400

void draw_cursor(int i);
struct Viewport *vp;
uint32_t *backbuf;
struct CONSOLE *console;
int term_width;
int term_height;

char *defaultlib[] = {
    "(defmacro (defun name args expr) (cons \'define (cons name (cons (cons \'lambda (cons args (cons expr NIL))) NIL))))",
    "(define square (lambda (x) (* x x)))",
    "(define abs (lambda (x) (- 0 x)))",
    "(defun fib (n a b) (if (= n 0) nil (cons a (fib (- n 1) b (+ a b)))))",
    "(define foldl (lambda (proc init list) (if list (foldl proc (proc init (car list)) (cdr list)) init)))",
    "(define foldr (lambda (proc init list) (if list (proc (car list) (foldr proc init (cdr list))) init)))",
    "(define list (lambda items (foldr cons nil items)))",
    "(defmacro (setq name val) (list 'define name val))",
    "(define reverse (lambda (list) (foldl (lambda (a x) (cons x a) nil list))))",
    "(defun unary-map (proc list) (foldr (lambda (x rest) (cons (proc x) rest)) nil list))",
    "(defun append (a b) (foldr cons b a))",
    "(defun caar (x) (car (car x)))",
    "(defun cadr (x) (car (cdr x)))",
    "(defmacro (quasiquote x) (if (pair? x) (if (eq? (car x) 'unquote) (cadr x) (if (eq? (caar x) 'unquote-splicing) (list 'append (cadr (car x)) (list 'quasiquote (cdr x))) (list 'cons (list 'quasiquote (car x)) (list 'quasiquote (cdr x))))) (list 'quote x)))",
    "(defmacro (let defs . body) `((lambda ,(map car defs) ,@body) ,@(map cadr defs)))"
};
void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);
int running;

//static char input[2048];

int main(int argc, int argv[]){
    printf("Opening Greenwood LISP\n");
    vp = vp_open(400, 400, "Greenwood LISP");
    backbuf = (uint32_t *) malloc(WIDTH * HEIGHT *sizeof(uint32_t));
	vp_set_buffer(vp, backbuf, WIDTH * HEIGHT * sizeof(uint32_t));
    vp_add_event_handler(vp, event_handler);
    console = console_open_vp(vp);

	term_width = WIDTH / 8;
	term_height = HEIGHT / 8;

    printf("Greenwood LISP Version 0.1\n");

    Atom env;
    env = env_create(nil);
    
    env_set(env, make_sym("CAR"), make_builtin(builtin_car));
    env_set(env, make_sym("CDR"), make_builtin(builtin_cdr));
    env_set(env, make_sym("CONS"), make_builtin(builtin_cons));
    env_set(env, make_sym("+"), make_builtin(builtin_add));
    env_set(env, make_sym("-"), make_builtin(builtin_sub));
    env_set(env, make_sym("*"), make_builtin(builtin_mult));
    env_set(env, make_sym("/"), make_builtin(builtin_div));
    env_set(env, make_sym("T"), make_sym("T"));
    env_set(env, make_sym("="), make_builtin(builtin_numeq));
    env_set(env, make_sym("<"), make_builtin(builtin_less));
    env_set(env, make_sym("TYPE?"), make_builtin(builtin_type));
    env_set(env, make_sym("TYPE_STR"), make_builtin(builtin_typeToStr));
    env_set(env, make_sym("STR"), make_builtin(builtin_strConv));
    env_set(env, make_sym("NTH"), make_builtin(builtin_nth));
    env_set(env, make_sym("APPLY"), make_builtin(builtin_apply));
    env_set(env, make_sym("EQ?"), make_builtin(builtin_eq));
    env_set(env, make_sym("PAIR?"), make_builtin(builtin_pair));
    env_set(env, make_sym("REAL"), make_builtin(builtin_real));
    
    
    Atom expr, result;
    const char *p = "(* 4 5)";
    Error err;

    //load_file(env, "/A/LISP/LIB.GWL");
    for(int i = 0; i < 15; i++){
        p = defaultlib[i];
        err = read_expr(p, &p, &expr);
        if(err){
            printf("Error while reading!\n");
            continue;
        }
        err = eval_expr(expr, env, &result);
        if(err){
            printf("Error in expression:\n\t");
            print_expr(expr);
            printf("\n");
        }
        else{
            print_expr(result);
            printf("\n");
        }
    }

    int idx = 0;
    char *termbuf = malloc(term_width);
    memset(termbuf, 0, term_width);
    set_schedule(ONFOCUS);

    running = 1;
    while(running){
        vp_copy(vp);
		char c = vp_getc(vp);
        draw_cursor(idx);
		if(c == '\0') continue;
		if(c == 8){
			idx--;
			termbuf[idx] = 0;
		}
		else{
			termbuf[idx++] = c;
			if(idx > term_width) idx = 0;
			
		}
		for(int i = 0; i < term_width; i++){
			vp_drawChar(vp, 8*i,(term_height-1)*8, termbuf[i], 0xFFFFFF, 0x0);
		}

        if(c == 10){
            p = termbuf;
            err = read_expr(p, &p, &expr);
            puts(termbuf);
            if(!err)
                err = eval_expr(expr, env, &result);
            switch(err){
                case Error_OK:
                    print_expr(result);
                    printf("\n");
                    break;
                case Error_Syntax:
                    printf("Synax Error\n");
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
            memset(termbuf, 0, term_width);
        }

        draw_cursor(idx);
        //window_update();
        vp_copy(vp);
    }
    console_close();
    vp_close(vp);
    free(backbuf);
    return 0;
}

void draw_cursor(int i){
	for(int x = 0; x < 8; x++){
		backbuf[WIDTH * ((term_height) * 8 - 2) + (i*8) + x] = 0xFFFFFF;
	}
}
void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_FOCUSED || event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
		set_schedule(ALWAYS);
    }
}