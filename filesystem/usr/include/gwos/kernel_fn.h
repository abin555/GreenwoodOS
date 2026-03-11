#ifndef USR_GWOS_KERNEL_FN_H
#define USR_GWOS_KERNEL_FN_H

#include <sys/kernfns.h>
#include <stdio.h>


FILE *kernel_fn_ready();
struct kernel_fn_def *kernel_fn_get(FILE *kern_file, char *fn_name);

#endif