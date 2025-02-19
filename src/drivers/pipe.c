#include "pipe.h"

struct Pipe *pipe_create(int size){
    struct Pipe *pipe = malloc(sizeof(struct Pipe));
    if(pipe == NULL) return NULL;

    pipe->buf = malloc(size);
    memset(pipe->buf, 0, size);
    pipe->buf_size = size;
    pipe->write_head = 0;
    return pipe;
}

int pipe_read(struct Pipe *pipe, void *buf, int nbytes){
    if(pipe == NULL || buf == NULL) return -1;
    int count = 0;
    for(int i = 0; i < nbytes; i++){
        ((char *)buf)[i] = pipe->buf[0];
        count++;
        for(int j = pipe->write_head; j > 0; j--){
            pipe->buf[j-1] = pipe->buf[j];
            pipe->write_head--;
        }
        if(pipe->write_head == 0){
            break;
        }
    }
    return count;
}

int pipe_write(struct Pipe *pipe, void *buf, int nbytes){
    if(pipe == NULL || buf == NULL) return -1;
    int count = 0;
    for(int i = 0; i < nbytes; i++){
        if(pipe->write_head >= pipe->buf_size) return count;
        pipe->buf[pipe->write_head++] = ((char *) buf)[i];
        count++;
    }
    return count;
}

void pipe_close(struct Pipe *pipe){
    pipe->buf = NULL;
    pipe->buf_size = 0;
    pipe->write_head = 0;
}
