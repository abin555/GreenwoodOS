#include <sys/io.h>
#include <stdio.h>

struct kernel_fn_def {
    void *fn;
    int n_args;
    int name_len;
    char name[];
} __attribute__((packed));

int main(int argc, char **argv){
    int kernel_fn_fd = open("/-/dev/kernel", O_READ);
    int num_fns;
    lseek(kernel_fn_fd, 0, SEEK_SET);
    read(kernel_fn_fd, &num_fns, sizeof(num_fns));
    printf("Kernel Function List:\nThere are %d functions\n", num_fns);

    struct kernel_fn_def *def;
    for(int i = 0; i < num_fns; i++){
        read(kernel_fn_fd, &def, sizeof(def));
        printf("#%d: 0x%x - \"%s\"\n", i, def->fn, def->name);
    }

    close(kernel_fn_fd);
}