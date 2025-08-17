#ifndef SYSFS_H
#define SYSFS_H

typedef enum {
    CDEV_WRITE = 0b1,
    CDEV_READ  = 0b10
} CDEV_PERMS;

struct SysFS_Chardev {
    int owner_pid;
    char *buf;
    int buf_size;
    void (*write_callback)(void *, int offset, int nbytes, int *head);
    void (*read_callback)(void *, int offset, int nbytes, int *head);
    int (*write_specialized_callback)(void *cdev, void *buf, int woffset, int nbytes, int *head);
    int (*read_specialized_callback)(void *cdev, void *buf, int roffset, int nbytes, int *head);
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

struct SysFS_Meta {
    struct SysFS_Inode *(*mkdir)(char *dirname);
    struct SysFS_Inode *(*mkcdev)(char *name, struct SysFS_Chardev *cdev);
    struct SysFS_Chardev *(*createCdev)(char *buf, int buf_size, CDEV_PERMS perms);
    int (*addChild)(struct SysFS_Inode *parent, struct SysFS_Inode *child);
    void (*setCallbacks)(
        struct SysFS_Chardev *cdev,
        void (*write_callback)(void *, int offset, int nbytes, int *head), 
        void (*read_callback)(void *, int offset, int nbytes, int *head),
        int (*write_specialized_callback)(void *cdev, void *buf, int woffset, int nbytes, int *head),
        int (*read_specialized_callback)(void *cdev, void *buf, int roffset, int nbytes, int *head)
    );
    struct SysFS_Inode *root;
};

struct SysFS_Inode *sysfs_createRoot();
int sysfs_addChild(struct SysFS_Inode *parent, struct SysFS_Inode *child);
struct SysFS_Inode *sysfs_mkdir(char *dirname);
struct SysFS_Inode *sysfs_mkcdev(char *name, struct SysFS_Chardev *cdev);
struct SysFS_Chardev *sysfs_createCharDevice(char *buf, int buf_size, CDEV_PERMS perms);
void sysfs_setCallbacks(struct SysFS_Chardev *cdev, 
    void (*write_callback)(void *, int offset, int nbytes, int *head), 
    void (*read_callback)(void *, int offset, int nbytes, int *head),
    int (*write_specialized_callback)(void *cdev, void *buf, int woffset, int nbytes, int *head),
    int (*read_specialized_callback)(void *cdev, void *buf, int roffset, int nbytes, int *head)
);
void sysfs_debugTree(struct SysFS_Inode *fs, int depth);

void *sysfs_find(void *r, char *path, unsigned int *meta);

struct VFS_File;
int sysfs_read(void *f, void *buf, int nbytes);
int sysfs_write(void *f, void *buf, int nbytes);
int sysfs_seek(void *f, int offset, int whence);

struct DirectoryListing sysfs_advListDirectory(void *fs, char *path);

void *sysfs_generateVFSRoot(struct SysFS_Inode *root, char letter);
struct SysFS_Inode *sysfs_createMetaFile(struct SysFS_Inode *root);

#endif