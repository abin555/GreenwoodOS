#include <sys/vp.h>
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

//(ptr) vp => nil
int builtin_vp_close(struct Atom args, struct Atom *result){
    if(nilp(args) || nilp(car(args)) || !nilp(cdr(args))) return Error_Args;
    Atom vp = car(args);
    if(vp.type != Atom_PTR) return Error_Type;
    vp_close(vp.value.ptr);
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
    driver.env_set(*driver.env, driver.make_sym("VP_CLOSE"), driver.make_builtin(builtin_vp_close));
    return 0;
}