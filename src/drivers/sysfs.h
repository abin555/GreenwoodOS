#ifndef SYSFS_H
#define SYSFS_H

struct SysFS_Chardev {
    char *buf;
    int buf_size;
    int buf_head;
};

struct SysFS_Inode;
struct SysFS_Inode {
    enum {
        Directory,
        Chardev,
    } type;
    char name[20];
    int namelen;
    union {
        struct {
            struct SysFS_Inode *children[20];
            int numChildren;
        } dir;
        struct SysFS_Chardev chardev;
    } data;
    struct SysFS_Inode *parent;
};



#endif