#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/vp.h>
#include <sys/console.h>
#include <sys/task.h>
#include <sys/sysfs.h>

#include "greenwood-lisp.h"
#include "builtin.h"

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

Atom env;

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
    Atom (*make_ptr)(void *ptr);
    Atom (*copy_list)(Atom list);
    int (*make_closure)(Atom env, Atom args, Atom body, Atom *result);
    int (*apply)(Atom fn, Atom args, Atom *result);
    int (*read_expr)(const char *input, const char **end, Atom *result);
    int (*eval_expr)(Atom expr, Atom env, Atom *result);
};

struct LISP_DRIVER driver;

int init_driver(){
    printf("Lisp Driver Init\n");
    FILE *metaFile = fopen("/-/fsCTRL", "r");
    if(metaFile == NULL){
        printf("Cannot open file!\n");
        return 1;
    }
    struct SysFS_Meta meta;
    fread(&meta, sizeof(meta), 1, metaFile);
    fclose(metaFile);

    driver.env = &env;
    driver.env_set = env_set;
    driver.env_get = env_get;
    driver.listp = listp;
    driver.cons = cons;
    driver.make_int = make_int;
    driver.make_sym = make_sym;
    driver.make_string = make_string;
    driver.make_builtin = make_builtin;
    driver.make_real = make_real;
    driver.make_ptr = make_ptr;
    driver.copy_list = copy_list;
    driver.make_closure = make_closure;
    driver.apply = apply;
    driver.read_expr = read_expr;
    driver.eval_expr = eval_expr;

    void *cdev = meta.createCdev(
        (char *) &driver,
        sizeof(driver),
        CDEV_READ
    );
    void *lisp_env = meta.mkcdev("env", cdev);
    void *dir = meta.mkdir("lisp");
    
    meta.addChild(meta.root, dir);
    meta.addChild(dir, lisp_env);
    return 0;
}

int main(int argc, int argv[]){
    printf("Opening Greenwood LISP\n");
    printf("Greenwood LISP Version 0.2\n");


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

    if(init_driver()){
        printf("Error setting up driver!\n");
        return 1;
    }
    
    printf("LISP Driver Setup!\n");

    Atom expr, result;
    const char *p = "(* 4 5)";
    Error err;

    printf("Lib @ 0x%x\n", defaultlib);
    p = "(defmacro (defun name args expr) (cons \'define (cons name (cons (cons \'lambda (cons args (cons expr NIL))) NIL))))";
    read_expr(p, &p, &expr);
    eval_expr(expr, env, &result);
    load_file(env, "/A/drivers/lispeng/lib.gwl");
    /*
    for(int i = 0; i < 15; i++){
        p = defaultlib[i];
        printf("Parsing: 0x%x \"%s\"\n", p, p);
        err = read_expr(p, &p, &expr);
        if(err){
            printf("Error while reading! %d\n", err);
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
    */
    printf("Moving into the background!\n");
    set_schedule(NEVER);
    yield();
    while(1){
    }
    return 0;
}
