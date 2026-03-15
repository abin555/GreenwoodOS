#include <gwos/kernel_fn.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

FILE *kernel_fn_ready(){
    FILE *kernel_fn_file = fopen("/-/dev/kernel", "r");
    return kernel_fn_file;
}

struct kernel_fn_def *kernel_fn_get(FILE *kern_file, char *fn_name){
    if(kern_file == NULL || fn_name == NULL) return NULL;
    fseek(kern_file, 0, SEEK_SET);
    int num_fns;
    fread(&num_fns, sizeof(num_fns), 1, kern_file);
    struct kernel_fn_def *def;
    for(int i = 0; i < num_fns; i++){
        fread(&def, sizeof(def), 1, kern_file);
        if(!strcmp(fn_name, def->name)){
            struct kernel_fn_def *fn_cpy = malloc(sizeof(struct kernel_fn_def) + def->name_len);
            memcpy(fn_cpy, def, sizeof(struct kernel_fn_def) + def->name_len);
            return fn_cpy;
        }
    }
    return NULL;
}
