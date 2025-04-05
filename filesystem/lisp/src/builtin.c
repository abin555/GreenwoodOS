#include "builtin.h"

int builtin_car(Atom args, Atom *result){
    if(nilp(args) || !nilp(cdr(args)))
        return Error_Args;
    else if(car(args).type == Atom_PAIR)
        *result = car(car(args));
    else
        *result = nil;
    return Error_OK;
}

int builtin_cdr(Atom args, Atom *result){
    if(nilp(args) || !nilp(cdr(args)))
        return Error_Args;
    else if(car(args).type == Atom_PAIR)
        *result = cdr(car(args));
    else
        *result = nil;
    return Error_OK;
}

int builtin_cons(Atom args, Atom *result){
    if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;
    *result = cons(car(args), car(cdr(args)));
    return Error_OK;
}

int builtin_add(Atom args, Atom *result){
    Atom a, b;
    if(nilp(args) || nilp(cdr(args))){
        return Error_Args;
    }
    a = car(args);
    b = cdr(args);
    char coerce_bool = 0;
    int r;
    float r_f;
    if(a.type != Atom_INT && a.type != Atom_REAL)
        return Error_Type;
    if(a.type == Atom_INT)
        r = a.value.integer;
    else if(a.type == Atom_REAL){
        coerce_bool = 1;
        r_f = a.value.real;
    }

    while(!nilp(b)){
        if(car(b).type != Atom_INT && car(b).type != Atom_REAL)
            return Error_Type;
        if(car(b).type == Atom_INT && !coerce_bool)
            r += car(b).value.integer;
        else if(car(b).type == Atom_INT && coerce_bool)
            r_f += (float) car(b).value.integer;
        else if(car(b).type == Atom_REAL && coerce_bool)
            r_f += car(b).value.real;
        else if(car(b).type == Atom_REAL && !coerce_bool){
            r_f = (float) r;
            coerce_bool = 1;
            r_f += car(b).value.real;
        }


        b = cdr(b);
    }
    if(!coerce_bool)
        *result = make_int(r);
    else
        *result = make_real(r_f);
    
    return Error_OK;
}
int builtin_sub(Atom args, Atom *result){
    Atom a, b;
    if(nilp(args) || nilp(cdr(args))){
        return Error_Args;
    }
    a = car(args);
    b = cdr(args);
    char coerce_bool = 0;
    int r;
    float r_f;
    if(a.type != Atom_INT && a.type != Atom_REAL)
        return Error_Type;
    if(a.type == Atom_INT)
        r = a.value.integer;
    else if(a.type == Atom_REAL){
        coerce_bool = 1;
        r_f = a.value.real;
    }

    while(!nilp(b)){
        if(car(b).type != Atom_INT && car(b).type != Atom_REAL)
            return Error_Type;
        if(car(b).type == Atom_INT && !coerce_bool)
            r -= car(b).value.integer;
        else if(car(b).type == Atom_INT && coerce_bool)
            r_f -= (float) car(b).value.integer;
        else if(car(b).type == Atom_REAL && coerce_bool)
            r_f -= car(b).value.real;
        else if(car(b).type == Atom_REAL && !coerce_bool){
            r_f = (float) r;
            coerce_bool = 1;
            r_f -= car(b).value.real;
        }


        b = cdr(b);
    }
    if(!coerce_bool)
        *result = make_int(r);
    else
        *result = make_real(r_f);
    
    return Error_OK;
}
int builtin_mult(Atom args, Atom *result){
    Atom a, b;
    if(nilp(args) || nilp(cdr(args))){
        return Error_Args;
    }
    a = car(args);
    b = cdr(args);
    char coerce_bool = 0;
    int r;
    float r_f;
    if(a.type != Atom_INT && a.type != Atom_REAL)
        return Error_Type;
    if(a.type == Atom_INT)
        r = a.value.integer;
    else if(a.type == Atom_REAL){
        coerce_bool = 1;
        r_f = a.value.real;
    }

    while(!nilp(b)){
        if(car(b).type != Atom_INT && car(b).type != Atom_REAL)
            return Error_Type;
        if(car(b).type == Atom_INT && !coerce_bool)
            r *= car(b).value.integer;
        else if(car(b).type == Atom_INT && coerce_bool)
            r_f *= (float) car(b).value.integer;
        else if(car(b).type == Atom_REAL && coerce_bool)
            r_f *= car(b).value.real;
        else if(car(b).type == Atom_REAL && !coerce_bool){
            r_f = (float) r;
            coerce_bool = 1;
            r_f *= car(b).value.real;
        }


        b = cdr(b);
    }
    if(!coerce_bool)
        *result = make_int(r);
    else
        *result = make_real(r_f);
    
    return Error_OK;
}
int builtin_div(Atom args, Atom *result){
    Atom a, b;
    if(nilp(args) || nilp(cdr(args))){
        return Error_Args;
    }
    a = car(args);
    b = cdr(args);
    char coerce_bool = 0;
    int r;
    float r_f;
    if(a.type != Atom_INT && a.type != Atom_REAL)
        return Error_Type;
    if(a.type == Atom_INT)
        r = a.value.integer;
    else if(a.type == Atom_REAL){
        coerce_bool = 1;
        r_f = a.value.real;
    }

    while(!nilp(b)){
        if(car(b).type != Atom_INT && car(b).type != Atom_REAL)
            return Error_Type;

        if(car(b).value.integer == 0) return Error_Type;

        if(car(b).type == Atom_INT && !coerce_bool)
            r /= car(b).value.integer;
        else if(car(b).type == Atom_INT && coerce_bool)
            r_f /= (float) car(b).value.integer;
        else if(car(b).type == Atom_REAL && coerce_bool)
            r_f /= car(b).value.real;
        else if(car(b).type == Atom_REAL && !coerce_bool){
            r_f = (float) r;
            coerce_bool = 1;
            r_f /= car(b).value.real;
        }

        b = cdr(b);
    }
    if(!coerce_bool)
        *result = make_int(r);
    else
        *result = make_real(r_f);
    
    return Error_OK;
}

int builtin_numeq(Atom args, Atom *result){
    Atom a, b;
    if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))){
        return Error_Args;
    }
    a = car(args);
    b = car(cdr(args));

    if((a.type != Atom_INT && a.type != Atom_REAL) || (b.type != Atom_INT && b.type != Atom_REAL))
        return Error_Type;
    
    *result = ((a.type == Atom_INT ? a.value.integer : a.value.real) == (b.type == Atom_INT ? b.value.integer : b.value.real)) ? make_sym("T") : nil;
    return Error_OK;
}

int builtin_less(Atom args, Atom *result){
    Atom a, b;
    if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))){
        return Error_Args;
    }
    a = car(args);
    b = car(cdr(args));

    if((a.type != Atom_INT && a.type != Atom_REAL) || (b.type != Atom_INT && b.type != Atom_REAL))
        return Error_Type;
    *result = ((a.type == Atom_INT ? a.value.integer : a.value.real) < (b.type == Atom_INT ? b.value.integer : b.value.real)) ? make_sym("T") : nil;
    return Error_OK;
}

int builtin_type(Atom args, Atom *result){
    if(nilp(args) || nilp(car(args)) || !nilp(cdr(args))){
        return Error_Args;
    }
    //print_int("Getting Type: %d\n", car(args).type);
    *result = make_int(car(args).type);
    return Error_OK;    
}

int builtin_typeToStr(Atom args, Atom *result){
    if(nilp(args) || nilp(car(args)) || !nilp(cdr(args))){
        return Error_Args;
    }
    if(car(args).type != Atom_INT){
        return Error_Type;
    }
    int type = car(args).value.integer;
    char *type_str;
    switch(type){
        case Atom_NIL:
            type_str = "NIL";
            break;
        case Atom_PAIR:
            type_str = "PAIR";
            break;
        case Atom_SYMBOL:
            type_str = "SYMBOL";
            break;
        case Atom_INT:
            type_str = "INTEGER";
            break;
        case Atom_REAL:
            type_str = "REAL";
            break;
        case Atom_STRING:
            type_str = "STRING";
            break;
        case Atom_BUILTIN:
            type_str = "BUILTIN";
            break;
        case Atom_CLOSURE:
            type_str = "CLOSURE";
            break;
        case Atom_MACRO:
            type_str = "MACRO";
            break;
    }   
    *result = make_string(type_str);
    return Error_OK;
}

int builtin_strConv(Atom args, Atom *result){
    if(nilp(args) || nilp(car(args)) || !nilp(cdr(args))){
        return Error_Args;
    }
    /*
    char *str = NULL;;

    if(car(args).type == Atom_INT){
        str = malloc(101);
        sprintf(str, "%d", car(args).value.integer);
    }
    else if(car(args).type == Atom_REAL){
        str = malloc(101);
        sprintf(str, "%f", car(args).value.real);
    }
    else if(car(args).type == Atom_SYMBOL){
        str = malloc(101);
        str = strdup(car(args).value.symbol);
    }
    else{
        return Error_Type;
    }
    *result = make_string(str);
    return Error_OK;
    */
    
    *result = make_int(0x00F);
    return Error_OK;
}

int builtin_nth(Atom args, Atom *result){
    if(nilp(args) || nilp(car(args)) || nilp(cdr(args)) || nilp(car(cdr(args))) || !nilp(cdr(cdr(args)))){
        return Error_Args;
    }
    Atom nth = car(args);
    Atom count = car(cdr(args));
    if(nth.type != Atom_PAIR || count.type != Atom_INT){
        return Error_Type;
    }

    int i = 0;
    int n = count.value.integer;
    while(!nilp(car(nth)) && i != n){
        nth = cdr(nth);
        i++;
        if(nilp(nth) || nth.type != Atom_PAIR){
            return Error_Syntax;
        }
    }
    *result = car(nth);
    return Error_OK;    
}

int builtin_apply(Atom args, Atom *result){
    Atom fn;
    if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))){
        return Error_Args;
    }
    fn = car(args);
    args = car(cdr(args));
    if(!listp(args))
        return Error_Syntax;
    
    return apply(fn, args, result);
}

int builtin_eq(Atom args, Atom *result){
    Atom a, b;
    int eq;
    if(nilp(args) || (nilp(cdr(args))) || !nilp(cdr(cdr(args)))){
        return Error_Args;
    }

    a = car(args);
    b = car(cdr(args));

    if(a.type == b.type){
        switch(a.type){
            case Atom_NIL:
                eq = 1;
                break;
            case Atom_PAIR:
            case Atom_CLOSURE:
            case Atom_MACRO:
                eq = (a.value.pair == b.value.pair);
                break;
            case Atom_SYMBOL:
                eq = (a.value.symbol == b.value.symbol);
                break;
            case Atom_INT:
                eq = (a.value.integer == b.value.integer);
                break;
            case Atom_REAL:
                eq = (a.value.real == b.value.real);
                break;
            case Atom_BUILTIN:
                eq = (a.value.builtin == b.value.builtin);
                break;
            case Atom_STRING:
                eq = !strcmp(a.value.string, b.value.string);
                break;
        }
    }
    else {
        eq = 0;
    }

    *result = eq ? make_sym("T") : nil;
    return Error_OK;
}

int builtin_pair(Atom args, Atom *result){
    if(nilp(args) || !nilp(cdr(args)))
        return Error_Args;
    *result = (car(args).type == Atom_PAIR) ? make_sym("T") : nil;
    return Error_OK;
}

int builtin_real(Atom args, Atom *result){
    if(nilp(args) || !nilp(cdr(args)))
        return Error_Args;

    if(car(args).type != Atom_INT && car(args).type != Atom_REAL){
        return Error_Type;
    }
    if(car(args).type == Atom_INT){
        *result = make_real((float) car(args).value.integer);
        return Error_OK;
    }
    if(car(args).type == Atom_REAL){
        *result = make_real((float) car(args).value.real);
        return Error_OK;
    }
}