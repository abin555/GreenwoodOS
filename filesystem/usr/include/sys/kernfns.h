#ifndef SYS_KERNFNS_H
#define SYS_KERNFNS_H

struct kernel_fn_def {
    void *fn;
    int n_args;
    int name_len;
    char name[];
} __attribute__((packed));

#endif