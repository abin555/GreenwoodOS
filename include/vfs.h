#ifndef VFS_H
#define VFS_H

#include "ISO9660.h"
#include "ext2.h"
#include "sysfs.h"
#include "drive.h"
#include "pipe.h"

typedef enum {
    VFS_FLAG_READ = 0b1,
    VFS_FLAG_WRITE = 0b10
} VFS_Inode_Flags;

struct VFS_Inode;
struct VFS_Inode {
    enum {
        VFS_ISO9660,
        VFS_EXT2,
        VFS_SYS,
        VFS_PIPE
    } type;

    union {
        struct File_Info *iso;
        struct EXT2_Inode *ext2;
        struct SysFS_Inode *sysfs;
        struct Pipe *pipe;
    } fs;
    int ext2_inode_idx;

    struct VFS_Inode *root;

    int isRoot;
    int isValid;
    int flags;

    struct DRIVE *drive;
    char nonDriveLetter;
};

struct VFS_File {
    struct VFS_Inode inode;
    int head;
    int status;//1 = Free 0 = Used
};

void vfs_init();
void vfs_addRoot(struct DRIVE *drive);
void vfs_addSysRoot(struct SysFS_Inode *sysfs, char letter);
struct VFS_Inode *vfs_findRoot(char driveLetter);

struct VFS_Inode vfs_followLink(struct VFS_Inode *parent, char *path);

int vfs_openRel(struct DIRECTORY *dir, char *path, int flags);
int vfs_open(char *path, int flags);
void vfs_close(int fd);
int vfs_read(int fd, void *buf, uint32_t nbytes);
int vfs_write(int fd, void *buf, uint32_t nbytes);
int vfs_seek(int fd, int offset, int whence);
int vfs_creatRel(struct DIRECTORY *dir, char *path);
int vfs_creat(char *path);
int vfs_mkpipe(int *writer_fd, int *reader_fd);
struct DirectoryListing vfs_listDirectory(struct DIRECTORY *dir, char *path);
struct DirectoryListing vfs_taskListDirectory(char *path);
int vfs_chdir(struct DIRECTORY *dir, char *path);

#endif