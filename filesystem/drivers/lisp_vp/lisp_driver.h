#ifndef LISP_DRIVER_H
#define LISP_DRIVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    Error_OK = 0,
    Error_Syntax,
    Error_Unbound,
    Error_Args,
    Error_Type
} Error;

struct Pair;
struct Atom;

typedef int (*Builtin)(struct Atom args, struct Atom *result);

struct Atom{    
    enum {
        Atom_NIL,
        Atom_PAIR,
        Atom_SYMBOL,
        Atom_INT,
        Atom_REAL,
        Atom_STRING,
        Atom_BUILTIN,
        Atom_CLOSURE,
        Atom_MACRO,
        Atom_PTR
    } type;

    union {
        struct Pair *pair;
        const char *symbol;
        int integer;
        Builtin builtin;
        char *string;
        float real;
        void *ptr;
    } value;
};

struct Pair{
    struct Atom atom[2];
};

typedef struct Atom Atom;

static const Atom nil = { Atom_NIL };

#define car(p) ((p).value.pair->atom[0])
#define cdr(p) ((p).value.pair->atom[1])
#define nilp(atom) ((atom).type == Atom_NIL)

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

extern struct LISP_DRIVER driver;

#endif