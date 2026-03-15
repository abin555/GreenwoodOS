#include "pipe.h"
#include "vfs.h"

struct Pipe *pipe_create(int fd_w, int fd_r, int size) {
    struct Pipe *pipe = malloc(sizeof(struct Pipe));
    if (pipe == NULL) return NULL;

    pipe->buf = malloc(size);
    if (pipe->buf == NULL) {
        return NULL;
    }

    memset(pipe->buf, 0, size);
    pipe->fd_w = fd_w;
    pipe->fd_r = fd_r;
    pipe->buf_size  = size;
    pipe->write_head = 0;
    pipe->read_head  = 0;            // FIX 2: initialize read_head
    pipe->write_open = 1;
    pipe->read_open  = 1;

    print_serial("[PIPE] Created of size %d\n", pipe->buf_size);
    return pipe;
}

int pipe_read(struct Pipe *pipe, void *buf, int nbytes) {
    if (pipe == NULL) return -1;

    // FIX 2: use read_head; FIX 1: use nbytes as limit, not '\0'
    int available = pipe->write_head - pipe->read_head;
    if (available <= 0) return 0;

    int count = (nbytes < available) ? nbytes : available;

    if (buf != NULL) {
        memcpy(buf, pipe->buf + pipe->read_head, count);
    }

    pipe->read_head += count;

    // Reset both heads when buffer is fully drained (avoid growing offset forever)
    if (pipe->read_head == pipe->write_head) {
        pipe->read_head  = 0;
        pipe->write_head = 0;
    }

    //print_serial("[PIPE] %d Read %d bytes, head=%d\n", pipe->fd_r, count, pipe->read_head);
    return count;
}

int pipe_write(struct Pipe *pipe, void *buf, int nbytes) {
    if (pipe == NULL || buf == NULL) return -1;

    // FIX 4: respect nbytes fully, don't stop on '\0'
    int space = pipe->buf_size - pipe->write_head;
    if (space <= 0) return 0;

    int count = (nbytes < space) ? nbytes : space;
    memcpy(pipe->buf + pipe->write_head, buf, count);
    pipe->write_head += count;

    //print_serial("[PIPE] %d Wrote %d bytes (%c), head=%d\n", pipe->fd_w, count, ((char *) buf)[0], pipe->write_head);
    return count;
}

int pipe_close(struct Pipe *pipe, int side) {
    if (pipe == NULL) return -1;

    if (side == VFS_FLAG_READ) {
        print_serial("[PIPE] Reader Closed\n");
        pipe->read_open = 0;
    } else if (side == VFS_FLAG_WRITE) {
        print_serial("[PIPE] Writer Closed\n");
        pipe->write_open = 0;
    } else {
        return -1;
    }

    // FIX 3: check is now reachable; free memory properly
    if (!pipe->write_open && !pipe->read_open) {
        print_serial("[PIPE] Both Ends Closed! Freeing Pipe!\n");
        pipe->buf        = NULL;
        pipe->buf_size   = 0;
        pipe->write_head = 0;
        pipe->read_head  = 0;
        return 1;
    }

    return 0;
}

// --- VFS wrappers (unchanged logic, kept for completeness) ---

int pipe_vfs_read(void *f, void *buf, int nbytes) {
    struct VFS_File *file = f;
    if (file->inode.type == VFS_PIPE)
        return pipe_read(file->inode.fs.pipe, buf, nbytes);
    return 0;
}

int pipe_vfs_write(void *f, void *buf, int nbytes) {
    struct VFS_File *file = f;
    if (file->inode.type == VFS_PIPE)
        return pipe_write(file->inode.fs.pipe, buf, nbytes);
    return 0;
}

int pipe_vfs_stat(void *f, void *statbuf) {
    if (f == NULL || statbuf == NULL) return -1;
    struct VFS_File *file = f;
    if (file->inode.type != VFS_PIPE) return -1;

    struct Pipe     *pipe = file->inode.fs.pipe;
    struct VFS_stat *stat = statbuf;
    memcpy(stat->fs_ownerIden, "OS PIPE", 8);
    stat->open_stat = pipe->read_open && pipe->write_open;
    stat->size      = pipe->write_head - pipe->read_head;
    print_serial("[PIPE] Stat: Open = %d\n", stat->open_stat);
    return 0;
}

struct VFS_RootInterface pipe_interface = {
    NULL, 'P', "OS PIPE",
    NULL, NULL,
    pipe_vfs_read, pipe_vfs_write,
    NULL, NULL, NULL, NULL, NULL,
    pipe_vfs_stat
};

void *pipe_getInterface() {
    return (void *)&pipe_interface;
}