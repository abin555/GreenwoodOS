#ifndef LISP_DRIVER_H
#define LISP_DRIVER_H

#include "client.h"

struct LISP_DRIVER {
    Atom *env;

    int (*env_set)(Atom, Atom, Atom);
    int (*env_get)(Atom env, Atom symbol, Atom *result);
    int (*env_unset)(Atom, Atom);
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

extern struct LISP_DRIVER driver;

#endif