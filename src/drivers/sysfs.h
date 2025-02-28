#ifndef SYSFS_H
#define SYSFS_H

typedef enum {
    CDEV_WRITE = 0b1,
    CDEV_READ = 0b10
} CDEV_PERMS;

struct SysFS_Chardev {
    char *buf;
    int buf_size;
    void (*write_callback)(void *, int offset, int nbytes);
    void (*read_callback)(void *, int offset, int nbytes);
    CDEV_PERMS perms;
};

struct SysFS_Inode;
struct SysFS_Inode {
    enum {
        SysFS_Directory,
        SysFS_Chardev
    } type;
    char name[20];
    int namelen;
    union {
        struct {
            struct SysFS_Inode *children[20];
            int numChildren;
        } dir;
        struct SysFS_Chardev *chardev;
    } data;
    struct SysFS_Inode *parent;
};

struct SysFS_Inode *sysfs_createRoot();
int sysfs_addChild(struct SysFS_Inode *parent, struct SysFS_Inode *child);
struct SysFS_Inode *sysfs_mkdir(char *dirname);
struct SysFS_Inode *sysfs_mkcdev(char *name, struct SysFS_Chardev *cdev);
struct SysFS_Chardev *sysfs_createCharDevice(char *buf, int buf_size, CDEV_PERMS perms);
void sysfs_setCallbacks(struct SysFS_Chardev *cdev, void (*write_callback)(void *, int offset, int nbytes), void (*read_callback)(void *, int offset, int nbytes));
void sysfs_debugTree(struct SysFS_Inode *fs, int depth);

struct SysFS_Inode *sysfs_find(struct SysFS_Inode *root, char *path);

struct VFS_File;
int sysfs_read(struct VFS_File *file, void *buf, int nbytes);
#endif