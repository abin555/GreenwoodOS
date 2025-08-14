#ifndef GREENWOOD_LISP_H
#define GREENWOOD_LISP_H

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

void print_expr(Atom atom);

int lex(const char *str, const char **start, const char **end);
int parse_simple(const char *start, const char *end, Atom *result);
int read_list(const char *start, const char **end, Atom *result);
int read_expr(const char *input, const char **end, Atom *result);
int eval_expr(Atom expr, Atom *env, Atom *result);


#endif