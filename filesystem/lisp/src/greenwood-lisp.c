#include "greenwood-lisp.h"

#ifndef Greenwood_OS
char *slurp(const char *path){
    FILE *file;
    char *buf;
    long len;
    file = fopen(path, "r");
    if(!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);

    buf = os_malloc(len + 1);
    if(!buf)
        return NULL;
    
    fread(buf, 1, len, file);
    buf[len] = 0;
    fclose(file);
    return buf;
}
#endif
#ifdef Greenwood_OS
char *slurp(const char *path){
    struct FILE *file = fopen((char *) path);
    if(file == NULL) return NULL;
    int size = fsize(file);
    print_int("File size is %d\n", size);
    char *buf = kmalloc(size+2);
    memset(buf, 0, size+2);
    fcopy(file, buf, size);
    return buf;
}
#endif

void load_file(Atom env, const char *path){
    char *text;
    print_str("Reading %s...\n", (char *) path);
    text = slurp(path);
    if(text != NULL) {
        const char *p = text;
        Atom expr;
        print("Evaluating\n");
        Error err = Error_OK;
        while(err == Error_OK){
            err = read_expr(p, &p, &expr);
            if(err){
                print("Error while reading!\n");
                break;
            }
            else{
                print_expr(expr);
                print("\n");
            }
            Atom result;
            err = eval_expr(expr, env, &result);
            if(err){
                print("Error in expression:\n\t");
                print_expr(expr);
                print("\n");
            }
            else{
                print_expr(result);
                print("\n");
            }
        }
        os_free(text);
    }
}

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
        buf = os_malloc(end - start + 1);
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
        *result = make_string(buf);
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
    buf = os_malloc(end - start + 1);
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
        *result = make_sym(buf);
    }
    os_free(buf);
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
            *result = cons(item, nil);
            p = *result;
        }
        else{
            cdr(p) = cons(item, nil);
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
        *result = cons(make_sym("QUOTE"), cons(nil, nil));
        return read_expr(*end, end, &car(cdr(*result)));
    }
    else if(token[0] == '`'){
        *result = cons(make_sym("QUASIQUOTE"), cons(nil, nil));
        return read_expr(*end, end, &car(cdr(*result)));
    }
    else if(token[0] == ','){
        *result = cons(make_sym(
            token[1] == '@' ? "UNQUOTE-SPLICING" : "UNQUOTE"),
            cons(nil, nil));
        return read_expr(*end, end, &car(cdr(*result)));
    }
    else{
        return parse_simple(token, *end, result);
    }
}

void print_expr(Atom atom){
    switch(atom.type){
        case Atom_NIL:
            print("NIL");
            break;
        case Atom_PAIR:
            pchar('(');
            print_expr(car(atom));
            atom = cdr(atom);
            while(!(nilp(atom))){
                if(atom.type == Atom_PAIR){
                    pchar(' ');
                    print_expr(car(atom));
                    atom = cdr(atom);
                }
                else{
                    print(" . ");
                    print_expr(atom);
                    break;
                }
            }
            pchar(')');
            break;
        case Atom_SYMBOL:
            print_str("%s", (char *) atom.value.symbol);
            break;
        case Atom_INT:
            print_int("%d", atom.value.integer);
            break;
        case Atom_BUILTIN:
            #ifndef Greenwood_OS
            print_int("#<BUILTIN: 0x%x>", (unsigned long long) atom.value.builtin);
            #endif
            #ifdef Greenwood_OS
            print_int("#<BUILTIN: 0x%x>", (uint32_t) atom.value.builtin);
            #endif
            break;
        case Atom_STRING:
            print_str("\"%s\"", atom.value.string);
            break;
        case Atom_REAL:
            #ifndef Greenwood_OS
            printf("%f", atom.value.real);
            #endif
            #ifdef Greenwood_OS
            print_int("%d", atom.value.real);
            print_int(".%d", (atom.value.real - ((int) atom.value.real)) * 100);
            #endif
            break;
        case Atom_CLOSURE:
            print("<Closure Fn>");
            break;
        case Atom_MACRO:
            print("<MACRO>");
            break;
    }
    //pchar('\n');
}

Atom cons(Atom car_val, Atom cdr_val){
    Atom p;
    p.type = Atom_PAIR;
    p.value.pair = os_malloc(sizeof(struct Pair));

    car(p) = car_val;
    cdr(p) = cdr_val;

    return p;
}

int listp(Atom expr){
    while(!nilp(expr)){
        if(expr.type != Atom_PAIR)
            return 0;
        expr = cdr(expr);
    }
    return 1;
}

Atom make_int(int x){
    Atom a;
    a.type = Atom_INT,
    a.value.integer = x;
    return a;
}

static Atom sym_table = {Atom_NIL};

Atom make_sym(const char *s){
    Atom a, p;

    p = sym_table;
    while(!nilp(p)){
        a = car(p);
        //TODO: ABSTRACT IMPLEMENTATION FOR OS
        if(strcmp(a.value.symbol, s) == 0)
            return a;
        p = cdr(p);
    }

    a.type = Atom_SYMBOL;
    //TODO: ABSTRACT IMPLEMENTATION FOR OS
    a.value.symbol = strdup(s);
    sym_table = cons(a, sym_table);

    return a;
}

Atom make_builtin(Builtin fn){
    Atom a;
    a.type = Atom_BUILTIN;
    a.value.builtin = fn;
    return a;
}

Atom make_string(char *s){
    Atom a;
    a.type = Atom_STRING;
    a.value.string = strdup(s);
    return a;
}

Atom make_real(float real){
    Atom a;
    a.type = Atom_REAL;
    a.value.real = real;
    return a;
}

Atom copy_list(Atom list){
    Atom a, p;
    if(nilp(list)){
        return nil;
    }
    a = cons(car(list), nil);
    p = a;
    list = cdr(list);

    while(!nilp(list)){
        cdr(p) = cons(car(list), nil);
        p = cdr(p);
        list = cdr(list);
    }
    return a;
}

int make_closure(Atom env, Atom args, Atom body, Atom *result){
    Atom p;
    if(!listp(body))
        return Error_Syntax;
    
    p = args;
    while(!nilp(p)){
        if(p.type == Atom_SYMBOL){
            break;
        }
        else if(p.type != Atom_PAIR || car(p).type != Atom_SYMBOL)
            return Error_Type;
        p = cdr(p);
    }
    *result = cons(env, cons(args, body));
    result->type = Atom_CLOSURE;
    return Error_OK;
}

int apply(Atom fn, Atom args, Atom *result){
    Atom env, arg_names, body;
    if(fn.type == Atom_BUILTIN){
        return (*fn.value.builtin)(args, result);
    }
    else if(fn.type != Atom_CLOSURE)
        return Error_Type;
    env = env_create(car(fn));
    arg_names = car(cdr(fn));
    body = cdr(cdr(fn));

    while(!nilp(arg_names)){
        if(arg_names.type == Atom_SYMBOL){
            env_set(env, arg_names, args);
            args = nil;
            break;
        }
        
        if(nilp(args)){
            return Error_Args;
        }
        env_set(env, car(arg_names), car(args));
        arg_names = cdr(arg_names);
        args = cdr(args);
    }
    if(!nilp(args))
        return Error_Args;
    while(!nilp(body)){
        Error err = eval_expr(car(body), env, result);
        if(err)
            return err;
        body = cdr(body);
    }
    return Error_OK;
}

Atom env_create(Atom parent){
    return cons(parent, nil);
}

int env_get(Atom env, Atom symbol, Atom *result){
    Atom parent = car(env);
    Atom bs = cdr(env);
    
    while(!nilp(bs)){
        Atom b = car(bs);
        //if(car(b).value.symbol == symbol.value.symbol){
        if(!strcmp(car(b).value.symbol, symbol.value.symbol)){
            *result = cdr(b);
            return Error_OK;
        }
        bs = cdr(bs);
    }
    if(nilp(parent)){
        return Error_Unbound;
    }
    return env_get(parent, symbol, result);
}

int env_set(Atom env, Atom symbol, Atom value){
    Atom bs = cdr(env);
    Atom b = nil;
    while(!nilp(bs)){
        b = car(bs);
        if(car(b).value.symbol == symbol.value.symbol){
            cdr(b) = value;
            return Error_OK;
        }
        bs = cdr(bs);
    }

    b = cons(symbol, value);
    cdr(env) = cons(b, cdr(env));

    return Error_OK;
}

int eval_expr(Atom expr, Atom env, Atom *result){
    Atom op, args, p;
    Error err;

    if(expr.type == Atom_SYMBOL){
        return env_get(env, expr, result);
    }
    else if(expr.type != Atom_PAIR){
        *result = expr;
        return Error_OK;
    }

    if(!listp(expr)){
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
            return env_set(env, sym, val);
        }
        else if(!strcmp(op.value.symbol, "ENV?")){
            if(!nilp(args)){
                return Error_Args;
            }
            print_expr(env);
            pchar('\n');
            return Error_OK;
        }
        else if(!strcmp(op.value.symbol, "LAMBDA")){
            if(nilp(args) || nilp(cdr(args)))
                return Error_Args;
            return make_closure(env, car(args), cdr(args), result);
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
            
            err = make_closure(env, cdr(car(args)), cdr(args), &macro);
            if(err){
                print("Error making closure\n");
                return err;
            }
            macro.type = Atom_MACRO;
            *result = name;
            return env_set(env, name, macro);
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
        err = apply(op, args, &expansion);
        if(err)
            return err;
        return eval_expr(expansion, env, result);
    }

    //Eval args
    args = copy_list(args);
    p = args;
    while(!nilp(p)){
        err = eval_expr(car(p), env, &car(p));
        if(err) return err;
        p = cdr(p);
    }
    return apply(op, args, result);  
}