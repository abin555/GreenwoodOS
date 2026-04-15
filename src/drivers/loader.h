#ifndef LOADER_H
#define LOADER_H

struct Loader {
    char name[10];
    int (*validate_program_fn)(int system_fd);
    int (*load_program_fn)(int system_fd, char *filename, int argc, char **argv, void *vctx);
};

struct LoaderSystem {
    struct Loader loader;
    int creator_pid;
    int enabled;
};

#define LOADER_MAX 10
extern struct LoaderSystem loader_list[LOADER_MAX];
extern int loader_count;

int loader_init();
int loader_addLoader(struct Loader loader);
struct Loader *loader_identify(int systemFD);
int loader_loadProgram(int systemFD, char *filename, int argc, char **argv, void *vctx);

struct Loader loader_getDefault();

#endif