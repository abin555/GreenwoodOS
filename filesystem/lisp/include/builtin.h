#ifndef BUILTIN_H
#define BUILTIN_H
#include "greenwood-lisp.h"

int builtin_car(Atom args, Atom *result);
int builtin_cdr(Atom args, Atom *result);
int builtin_cons(Atom args, Atom *result);
int builtin_add(Atom args, Atom *result);
int builtin_sub(Atom args, Atom *result);
int builtin_mult(Atom args, Atom *result);
int builtin_div(Atom args, Atom *result);
int builtin_numeq(Atom args, Atom *result);
int builtin_less(Atom args, Atom *result);
int builtin_type(Atom args, Atom *result);
int builtin_typeToStr(Atom args, Atom *result);
int builtin_strConv(Atom args, Atom *result);
int builtin_nth(Atom args, Atom *result);
int builtin_apply(Atom args, Atom *result);
int builtin_eq(Atom args, Atom *result);
int builtin_pair(Atom args, Atom *result);
int builtin_real(Atom args, Atom *result);
int builtin_vp_open(Atom args, Atom *result);

#endif
