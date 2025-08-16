#include <sys/vp.h>
#include <sys/task.h>
#include "lisp_driver.h"

struct LISP_DRIVER driver;

//(number) w (number) h (str) name => (ptr) vp
int builtin_vp_open(struct Atom args, struct Atom *result){
    if(nilp(args) || nilp(car(args)) || nilp(cdr(args)) || nilp(car(cdr(args))) || nilp(cdr(cdr(args))) || nilp(car(cdr(cdr(args)))) || !nilp(cdr(cdr(cdr(args))))) return Error_Args;
    Atom w = car(args);
    Atom h = car(cdr(args));
    Atom name = car(cdr(cdr(args)));
    if(
        w.type != Atom_INT ||
        h.type != Atom_INT ||
        name.type != Atom_STRING
    ) return Error_Type;
    struct Viewport *vp = vp_open(
        w.value.integer,
        h.value.integer,
        name.value.string
    );
    *result = driver.make_ptr(vp);
    return Error_OK;
}

int builtin_exec(struct Atom args, struct Atom *result){
    int n_args = 0;
    struct Atom args_walker = args;
    if(nilp(args)){
        return Error_Args;
    }
    while(!nilp(cdr(args_walker))){
        n_args++;
        args_walker = cdr(args_walker);
    }
    
    char **argv = malloc(sizeof(char *) * n_args);
    args_walker = args;
    for(int i = 0; i < n_args; i++){
        Atom arg = car(args_walker);
        if(arg.type == Atom_STRING){
            argv[i] = strdup(arg.value.string);
        }
        else{
            argv[i] = NULL;
        }
        args_walker = cdr(args_walker);
    }
    //exec(argv[0], n_args, argv);
    char buf[1000];
    snprintf(buf, 1000, "%s %d\n", argv[0], n_args);
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
    driver.env_set(*driver.env, driver.make_sym("VP_OPEN"), driver.make_builtin(builtin_vp_open));
    driver.env_set(*driver.env, driver.make_sym("EXEC"), driver.make_builtin(builtin_exec));
    return 0;
}