#ifndef KERNEL_FN_H
#define KERNEL_FN_H

struct kernel_fn_def {
    void *fn;
    int n_args;
    int name_len;
    char name[];
} __attribute__((packed));

struct kernel_fn_list {
    int num_fns;
    struct kernel_fn_def *fns[200];
};

extern struct kernel_fn_list kernel_fn_list;

void kernel_fn_add(void *fn, int n_args, char *name);

void kernel_fn_init();

#endif