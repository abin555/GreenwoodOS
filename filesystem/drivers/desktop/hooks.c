#include <sys/io.h>
#include <stdio.h>
#include <string.h>
#include "hooks.h"

struct kernel_fn_def {
    void *fn;
    int n_args;
    int name_len;
    char name[];
} __attribute__((packed));

void (*buf_putChar)(uint32_t *buf, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void (*buf_w_putChar)(uint32_t *buf, uint32_t w, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void (*select_program)(int task_id);
int (*get_task_id)();
void (*set_task_id)();
int (*get_program_slot)(int task_id);
void (*memfcpy)(void *, void *, uint32_t);
void (*print_serial)(char *fmt, ...);

void hooks_attach(){
    FILE *kernel_fn_file = fopen("/-/dev/kernel", "r");
    
    if(kernel_fn_file != NULL){
        int num_fns;
        fread(&num_fns, sizeof(num_fns), 1, kernel_fn_file);
        struct kernel_fn_def *def;
        for(int i = 0; i < num_fns; i++){
            fread(&def, sizeof(def), 1, kernel_fn_file);
            printf("#%d: 0x%x - \"%s\"\n", i, def->fn, def->name);
            if(!strcmp("buf_putChar", def->name)){
                buf_putChar = def->fn;
                printf("Found buf_putChar!\n");
            }
            if(!strcmp("buf_w_putChar", def->name)){
                buf_w_putChar = def->fn;
                printf("Found buf_w_putChar!\n");
            }
            if(!strcmp("select_program", def->name)){
                select_program = def->fn;
                printf("Found select_program!\n");
            }
            if(!strcmp("get_task_id", def->name)){
                get_task_id = def->fn;
                printf("Found get_task_id!\n");
            }
            if(!strcmp("set_task_id", def->name)){
                set_task_id = def->fn;
                printf("Found set_task_id!\n");
            }
            if(!strcmp("get_program_slot", def->name)){
                get_program_slot = def->fn;
                printf("Found get_program_slot!\n");
            }
            if(!strcmp("memfcpy", def->name)){
                memfcpy = def->fn;
                printf("Found memfcpy!\n");
            }
            if(!strcmp("print_serial", def->name)){
                print_serial = def->fn;
                printf("Found print_serial!\n");
            }
        }
    }
}