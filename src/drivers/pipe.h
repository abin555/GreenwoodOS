#ifndef PIPE_H
#define PIPE_H
#include "stddef.h"
#include "stdint.h"
#include "allocator.h"

struct Pipe {
    char *buf;
    int buf_size;
    int write_head;
};

struct Pipe *pipe_create(int size);
int pipe_read(struct Pipe *pipe, void *buf, int nbytes);
int pipe_write(struct Pipe *pipe, void *buf, int nbytes);
void pipe_close(struct Pipe *pipe);


#endif