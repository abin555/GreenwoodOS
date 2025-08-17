#include <sys/vp.h>
#include <sys/console.h>
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

int builtin_malloc(struct Atom args, struct Atom *result){
    if(nilp(args) || nilp(car(args)) || !nilp(cdr(args))) return Error_Args;
    Atom size = car(args);
    if(size.type != Atom_INT) return Error_Type;
    void *ptr = malloc(size.value.integer);
    *result = driver.make_ptr(ptr);
    return Error_OK;
}

int builtin_vp_set_buffer(struct Atom args, struct Atom *result){
    if(nilp(args) || nilp(car(args)) || nilp(cdr(args)) || nilp(car(cdr(args))) || nilp(cdr(cdr(args))) || nilp(car(cdr(cdr(args)))) || !nilp(cdr(cdr(cdr(args))))) return Error_Args;
    Atom vp = car(args);
    Atom ptr = car(cdr(args));
    Atom size = car(cdr(cdr(args)));
    if(
        vp.type != Atom_PTR ||
        ptr.type != Atom_PTR ||
        size.type != Atom_INT
    ) return Error_Type;
    printf("Setting Buffer 0x%x - 0x%x %d\n", vp.value.ptr, ptr.value.ptr, size.value.ptr);
    vp_set_buffer(
        vp.value.ptr,
        ptr.value.ptr,
        size.value.integer
    );
    *result = nil;
    return Error_OK;
}

int builtin_putstring(struct Atom args, struct Atom *result){
    if(nilp(args) || nilp(car(args)) || nilp(cdr(args)) || nilp(car(cdr(args))) || nilp(cdr(cdr(args))) || nilp(car(cdr(cdr(args)))) || nilp(car(cdr(cdr(cdr(args))))) || !nilp(cdr(cdr(cdr(cdr(args)))))) return Error_Args;
    Atom vp = car(args);
    Atom x = car(cdr(args));
    Atom y = car(cdr(cdr(args)));
    Atom c = car(cdr(cdr(cdr(args))));
    if(
        vp.type != Atom_PTR ||
        x.type != Atom_INT ||
        y.type != Atom_INT ||
        c.type != Atom_STRING
    ) return Error_Type;
    int len = strlen(c.value.string);
    for(int i = 0; i < len; i++){
        vp_drawChar(vp.value.ptr, x.value.integer+i*8, y.value.integer, c.value.string[i], 0xFFFFFFFF, 0x0);
    }
    vp_copy(vp.value.ptr);
    *result = nil;
    return Error_OK;
}

int builtin_console_open(struct Atom args, struct Atom *result){
    if(nilp(args) || nilp(car(args)) || !nilp(cdr(args))) return Error_Args;
    Atom vp = car(args);
    if(vp.type != Atom_PTR) return Error_Type;
    void *con = console_open_vp(vp.value.ptr);
    *result = driver.make_ptr(con);
    return Error_OK;
}

int builtin_console_close(struct Atom args, struct Atom *result){
    if(!nilp(args)) return Error_Args;
    console_close();
    *result = nil;
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
    driver.env_set(*driver.env, driver.make_sym("MALLOC"), driver.make_builtin(builtin_malloc));
    driver.env_set(*driver.env, driver.make_sym("VP_BUF"), driver.make_builtin(builtin_vp_set_buffer));
    driver.env_set(*driver.env, driver.make_sym("VP_WRITE"), driver.make_builtin(builtin_putstring));
    driver.env_set(*driver.env, driver.make_sym("CON_OPEN"), driver.make_builtin(builtin_console_open));
    driver.env_set(*driver.env, driver.make_sym("CON_CLOSE"), driver.make_builtin(builtin_console_close));
    return 0;
}