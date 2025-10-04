#ifndef VFS_H
#define VFS_H

#include "ext2.h"
#include "sysfs.h"
#include "netfs.h"
#include "drive.h"
#include "pipe.h"

typedef enum {
    VFS_FLAG_READ = 0b1,
    VFS_FLAG_WRITE = 0b10
} VFS_Inode_Flags;

typedef enum {
    VFS_ISO9660,
    VFS_EXT2,
    VFS_SYS,
    VFS_NET,
    VFS_PIPE
} VFS_InodeType;

struct VFS_File;
struct VFS_RootInterface {
    struct DRIVE *drive;
    char vfsLetter;
    char *fs_label;
    void *root;

    void *(*fs_getLink)(void *, char *path, uint32_t *meta);
    int (*fs_read)(void *f, void *buf, int nbytes);
    int (*fs_write)(void *f, void *buf, int nbytes);
    int (*fs_seek)(void *f, int offset, int whence);
    int (*fs_creat)(void *fs, char *path, unsigned int size);
    int (*fs_creatDir)(void *fs, char *path);
    struct DirectoryListing (*fs_listDirectory)(void *fs, char *path);
    int (*fs_truncate)(void *f, unsigned int len);
};

struct VFS_Inode;
struct VFS_Inode {
    VFS_InodeType type;

    union {
        void *fs;
        struct Pipe *pipe;
    } fs;
    uint32_t meta;

    struct VFS_Inode *root;
    struct VFS_RootInterface *interface;
    int isRoot;
    int isValid;
    int flags;
};

struct VFS_File {
    struct VFS_Inode inode;
    int head;
    int status;//1 = Free 0 = Used
    int owner_pid;
};

void vfs_init();
void vfs_addFS(struct VFS_RootInterface *interface);
void vfs_addRoot(struct DRIVE *drive);
void vfs_addSysRoot(struct SysFS_Inode *sysfs, char letter);
void vfs_addNetRoot(struct NetFS_Inode *netfs, char letter);
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
int vfs_creatDirRel(struct DIRECTORY *dir, char *path);
int vfs_creatdir(char *path);
int vfs_mkpipe(int *writer_fd, int *reader_fd);
struct DirectoryListing vfs_listDirectory(struct DIRECTORY *dir, char *path);
struct DirectoryListing vfs_taskListDirectory(char *path);
int vfs_chdir(struct DIRECTORY *dir, char *path);
int vfs_ftruncate(int fd, unsigned int length);


#endif