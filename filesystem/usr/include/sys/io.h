#ifndef SYS_IO_H
#define SYS_IO_H

#define O_READ 0b1
#define O_WRITE 0b10

int open(char *file, int flags);
void close(int fd);
int read(int fd, void *buf, int nbytes);
int write(int fd, void *buf, int nbytes);

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int lseek(int fd, int off, int whence);
int creat(char *path);

int ftruncate(int fd, unsigned int length);

#endif