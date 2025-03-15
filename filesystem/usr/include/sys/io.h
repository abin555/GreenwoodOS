#ifndef SYS_IO_H
#define SYS_IO_H

typedef enum {
    O_READ = 0b1,
    O_WRITE = 0b10
} GWOS_FS_FLAGS;

int open(char *file, GWOS_FS_FLAGS flags);
void close(int fd);
int read(int fd, void *buf, int nbytes);
int write(int fd, void *buf, int nbytes);

typedef enum {
    GWOS_SEEK_SET = 0,
    GWOS_SEEK_CUR = 1,
    GWOS_SEEK_END = 2
} GWOS_FS_SEEK;

int lseek(int fd, int off, GWOS_FS_SEEK whence);
int creat(char *path);

#endif