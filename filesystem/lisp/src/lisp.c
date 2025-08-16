#include <ctype.h>
#include "client.h"
#include "lisp_driver.h"

int lex(const char *str, const char **start, const char **end){
    const char *ws = " \t\n";
    const char *delim = "() \t\n";
    const char *prefix = "()\'`";
    //TODO: ABSTRACT IMPLEMENTATION FOR OS
    str += strspn(str, ws);

    if(str[0] == '\0'){
        *start = *end = NULL;
        return Error_Syntax;
    }
    *start = str;
    //TODO: ABSTRACT IMPLEMENTATION FOR OS
    if(strchr(prefix, str[0]) != NULL){
        *end = str + 1;
    }
    else if(str[0] == ',')
        *end = str + (str[1] == '@' ? 2 : 1);
    else{
        //TODO: ABSTRACT IMPLEMENTATION FOR OS
        *end = str + strcspn(str, delim);
    }
    return Error_OK;
}

//Parse integers, symbols, and NIL
int parse_simple(const char *start, const char *end, Atom *result){
    char *buf, *p;

    //Check if string
    if(start[0] == '\"' && *(end - 1) == '\"' && *(end - 2) != '\\'){
        start++;
        buf = malloc(end - start + 1);
        p = buf;
        while(start != end){
            *p++ = *start;
            start++;
            if(*start == '\"'){
                //break;
                if(*(start-1) != '\\'){
                    break;
                }
            }
        }
        *p = 0;
        *result = driver.make_string(buf);
        return Error_OK;
    }

    //Check if Integer
    //TODO: ABSTRACT IMPLEMENTATION FOR OS
    int val = strtol(start, &p, 10);
    if(p == end){
        result->type = Atom_INT;
        result->value.integer = val;
        return Error_OK;
    }

    //NIL or symbol
    buf = malloc(end - start + 1);
    p = buf;
    while(start != end){
        //TODO: ABSTRACT IMPLEMENTATION FOR OS
        *p++ = toupper(*start);
        start++;
    }
    *p = '\0';
    if(strcmp(buf, "NIL") == 0){
        *result = nil;
    }
    else{
        *result = driver.make_sym(buf);
    }
    free(buf);
    return Error_OK;
}

int read_list(const char *start, const char **end, Atom *result){
    Atom p;
    *end = start;
    p = *result = nil;
    for(;;){
        const char *token;
        Atom item;
        Error err;

        err = lex(*end, &token, end);
        if(err){
            return err;
        }
        if(token[0] == ')'){
            return Error_OK;
        }
        if(token[0] == '.' && *end - token == 1){
            //Improper List
            if(nilp(p)){
                return Error_Syntax;
            }
            err = read_expr(*end, end, &item);
            if(err){
                return err;
            }
            cdr(p) = item;
            err = lex(*end, &token, end);
            if(!err && token[0] != ')'){
                err = Error_Syntax;
            }
            return err;
        }
        
        err = read_expr(token, end, &item);
        if(err){
            return err;
        }
        if(nilp(p)){
            //First Item
            *result = driver.cons(item, nil);
            p = *result;
        }
        else{
            cdr(p) = driver.cons(item, nil);
            p = cdr(p);
        }
    }
}

int read_expr(const char *input, const char **end, Atom *result){
    const char *token;
    Error err;
    err = lex(input, &token, end);
    if(err){
        return err;
    }
    if(token[0] == '('){
        return read_list(*end, end, result);
    }
    else if(token[0] == ')'){
        return Error_Syntax;
    }
    else if(token[0] == '\''){
        *result = driver.cons(driver.make_sym("QUOTE"), driver.cons(nil, nil));
        return read_expr(*end, end, &car(cdr(*result)));
    }
    else if(token[0] == '`'){
        *result = driver.cons(driver.make_sym("QUASIQUOTE"), driver.cons(nil, nil));
        return read_expr(*end, end, &car(cdr(*result)));
    }
    else if(token[0] == ','){
        *result = driver.cons(driver.make_sym(
            token[1] == '@' ? "UNQUOTE-SPLICING" : "UNQUOTE"),
            driver.cons(nil, nil));
        return read_expr(*end, end, &car(cdr(*result)));
    }
    else{
        return parse_simple(token, *end, result);
    }
}

void print_expr(Atom atom){
    switch(atom.type){
        case Atom_NIL:
            puts("NIL");
            break;
        case Atom_PAIR:
            putchar('(');
            print_expr(car(atom));
            atom = cdr(atom);
            while(!(nilp(atom))){
                if(atom.type == Atom_PAIR){
                    putchar(' ');
                    print_expr(car(atom));
                    atom = cdr(atom);
                }
                else{
                    puts(" . ");
                    print_expr(atom);
                    break;
                }
            }
            putchar(')');
            break;
        case Atom_SYMBOL:
            printf("%s", (char *) atom.value.symbol);
            break;
        case Atom_INT:
            printf("%d", atom.value.integer);
            break;
        case Atom_BUILTIN:
            printf("#<BUILTIN: 0x%x>", (uint32_t) atom.value.builtin);
            break;
        case Atom_STRING:
            printf("\"%s\"", atom.value.string);
            break;
        case Atom_REAL:
            printf("%d", atom.value.real);
            printf(".%d", (atom.value.real - ((int) atom.value.real)) * 100);
            break;
        case Atom_CLOSURE:
            puts("<Closure Fn>");
            break;
        case Atom_MACRO:
            puts("<MACRO>");
            break;
    }
    //putchar('\n');
}

int eval_expr(Atom expr, Atom *env, Atom *result){
    Atom op, args, p;
    Error err;

    if(expr.type == Atom_SYMBOL){
        return driver.env_get(*env, expr, result);
    }
    else if(expr.type != Atom_PAIR){
        *result = expr;
        return Error_OK;
    }

    if(!driver.listp(expr)){
        return Error_Syntax;
    }

    op = car(expr);
    args = cdr(expr);

    if(op.type == Atom_SYMBOL){
        if(!strcmp(op.value.symbol, "QUOTE")){
            if(nilp(args) || !nilp(cdr(args))){
                return Error_Args;
            }
            *result = car(args);
            return Error_OK;
        }
        else if(!strcmp(op.value.symbol, "DEFINE")){
            Atom sym, val;
            if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))){
                return Error_Args;
            }
            sym = car(args);
            if(sym.type != Atom_SYMBOL)
                return Error_Type;
            err = eval_expr(car(cdr(args)), env, &val);
            if(err)
                return err;
            *result = sym;
            return driver.env_set(*env, sym, val);
        }
        else if(!strcmp(op.value.symbol, "ENV?")){
            if(!nilp(args)){
                return Error_Args;
            }
            *result = *env;
            return Error_OK;
        }
        else if(!strcmp(op.value.symbol, "LAMBDA")){
            if(nilp(args) || nilp(cdr(args)))
                return Error_Args;
            return driver.make_closure(*env, car(args), cdr(args), result);
        }
        else if(!strcmp(op.value.symbol, "IF")){
            Atom cond, val;

            if(nilp(args) || nilp(cdr(args)) || nilp(cdr(cdr(args))) || !nilp(cdr(cdr(cdr(args))))){
                return Error_Args;
            }
            err = eval_expr(car(args), env, &cond);
            if(err)
                return err;
            val = nilp(cond) ? car(cdr(cdr(args))) : car(cdr(args));
            return eval_expr(val, env, result);
        }
        else if(!strcmp(op.value.symbol, "DEFMACRO")){
            Atom name, macro;
            Error err;

            if(nilp(args) || nilp(cdr(args)))
                return Error_Args;
            if(car(args).type != Atom_PAIR)
                return Error_Syntax;
            name = car(car(args));
            if(name.type != Atom_SYMBOL)
                return Error_Type;
            
            err = driver.make_closure(*env, cdr(car(args)), cdr(args), &macro);
            if(err){
                puts("Error making closure\n");
                return err;
            }
            macro.type = Atom_MACRO;
            *result = name;
            return driver.env_set(*env, name, macro);
        }
        else if(!strcmp(op.value.symbol, "EVAL")){
            Atom expr;
            if(nilp(args) || !nilp(cdr(args))){
                return Error_Args;
            }

            int err = eval_expr(car(args), env, &expr);
            if(err) return err;


            return eval_expr(expr, env, result);
        }
    }

    //Eval Operator
    err = eval_expr(op, env, &op);
    if(err) return err;

    //Eval Macro
    if(op.type == Atom_MACRO){
        Atom expansion;
        op.type = Atom_CLOSURE;
        err = driver.apply(op, args, &expansion);
        if(err)
            return err;
        return eval_expr(expansion, env, result);
    }

    //Eval args
    args = driver.copy_list(args);
    p = args;
    while(!nilp(p)){
        err = eval_expr(car(p), env, &car(p));
        if(err) return err;
        p = cdr(p);
    }
    return driver.apply(op, args, result);  
}