#ifndef PIPE_H
#define PIPE_H
#include "stddef.h"
#include "stdint.h"
#include "allocator.h"

struct Pipe {
    char *buf;
    int buf_size;
    int write_head;
    char read_open;
    char write_open;
};

struct Pipe *pipe_create(int size);
int pipe_read(struct Pipe *pipe, void *buf, int nbytes);
int pipe_write(struct Pipe *pipe, void *buf, int nbytes);
int pipe_close(struct Pipe *pipe, int side);

void *pipe_getInterface();

#endif