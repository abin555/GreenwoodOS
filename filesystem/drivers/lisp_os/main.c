#include <sys/vp.h>
#include <sys/task.h>
#include "lisp_driver.h"

struct LISP_DRIVER driver;

int builtin_exec(struct Atom args, struct Atom *result){
    int n_args = 0;
    struct Atom args_walker = args;
    if(nilp(args)){
        return Error_Args;
    }
    while(!nilp(args_walker)){
        n_args++;
        args_walker = cdr(args_walker);
    }
    
    char **argv = malloc(sizeof(char *) * (n_args+1));
    args_walker = args;
    for(int i = 0; i < n_args; i++){
        Atom arg = car(args_walker);
        if(arg.type == Atom_STRING && arg.value.string != NULL){
            argv[i] = strdup(arg.value.string);
        }
        else{
            argv[i] = NULL;
        }
        args_walker = cdr(args_walker);
    }
    argv[n_args] = NULL;
    exec(argv[0], n_args, argv);
    char buf[1000];
    snprintf(buf, 1000, "%s %d", argv[0], n_args);
    *result = driver.make_string(strdup(buf));
    return Error_OK;
}

int main(int argc, char **argv){
    FILE *lisp_driver_file = fopen("/-/lisp/env", "r");
    if(lisp_driver_file == NULL){
        printf("Error: lisp engine not running!\n");
        return 1;
    }
    fread(&driver, sizeof(driver), 1, lisp_driver_file);
    fclose(lisp_driver_file);
    Atom module = driver.make_string(argv[0]);
    Atom modules;
    Atom modules_sym = driver.make_sym("MODULES");
    int err = driver.env_get(*driver.env, modules_sym, &modules);
    if(err == Error_Unbound){
        driver.env_set(*driver.env, modules_sym, driver.cons(module, nil));
    }
    else if(Error_OK){
        modules = driver.cons(module, modules);
        driver.env_set(*driver.env, modules_sym, modules);
    }
    driver.env_set(*driver.env, driver.make_sym("EXEC"), driver.make_builtin(builtin_exec));
    return 0;
}   