#include "pipe.h"
#include "vfs.h"

struct Pipe *pipe_create(int size){
    struct Pipe *pipe = malloc(sizeof(struct Pipe));
    if(pipe == NULL) return NULL;

    pipe->buf = malloc(size);
    memset(pipe->buf, 0, size);
    pipe->buf_size = size;
    pipe->write_head = 0;
    print_serial("[PIPE] Created of size %d\n", pipe->buf_size);
    return pipe;
}

int pipe_read(struct Pipe *pipe, void *buf, int nbytes){
    if(pipe == NULL) return -1;
    print_serial("[PIPE] Reading %d bytes, %s\n", nbytes, pipe->buf);
    int count = 0;
    if(buf != NULL){
        for(int i = 0; i < nbytes && ((char*)pipe->buf)[i] != '\0'; i++){
            ((char *)buf)[i] = pipe->buf[i];
            count++;
        }
    }
    else{
        for(int i = 0; i < nbytes && ((char*)pipe->buf)[i] != '\0'; i++){
            count++;
        }
    }
    for(int j = 0; pipe->buf[j] != '\0'; j++){
        //print_serial("%s - %d %d\n", pipe->buf, j, pipe->write_head);
        pipe->buf[j] = pipe->buf[count+j];
        pipe->write_head--;
    }
    return count;
}

int pipe_vfs_read(void *f, void *buf, int nbytes){
    print_serial("[PIPEFS] Read!\n");
    struct VFS_File *file = f;
    if(file->inode.type == VFS_PIPE){
        return pipe_read(file->inode.fs.pipe, buf, nbytes);
    }
    return 0;
}

int pipe_write(struct Pipe *pipe, void *buf, int nbytes){
    if(pipe == NULL || buf == NULL) return -1;
    print_serial("[PIPE] Writing %d bytes, %s\n", nbytes, buf);
    int count = 0;
    for(int i = 0; i < nbytes && ((char*) buf)[i] != '\0'; i++){
        if(pipe->write_head >= pipe->buf_size) return count;
        pipe->buf[pipe->write_head++] = ((char *) buf)[i];
        count++;
    }
    print_serial("%s - %d\n", pipe->buf, pipe->write_head);
    return count;
}

int pipe_vfs_write(void *f, void *buf, int nbytes){
    print_serial("[PIPEFS] Write!\n");
    struct VFS_File *file = f;
    if(file->inode.type == VFS_PIPE){
        return pipe_write(file->inode.fs.pipe, buf, nbytes);
    }
    return 0;
}

void pipe_close(struct Pipe *pipe){
    pipe->buf = NULL;
    pipe->buf_size = 0;
    pipe->write_head = 0;
}

struct VFS_RootInterface pipe_interface = {
    NULL,
    'P',
    "OS PIPE",
    NULL,

    NULL,
    pipe_vfs_read,
    pipe_vfs_write,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

void *pipe_getInterface(){
    return (void *) &pipe_interface;
}