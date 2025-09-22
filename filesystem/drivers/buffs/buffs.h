#ifndef BUFFS_H
#define BUFFS_H

#include <stdint.h>

struct BUFFS_Inode;

struct BUFFS_file {
    uint32_t size;

    uint32_t num_pages;
    void *page_base;

};

struct BUFFS_llist;
struct BUFFS_llist {
    struct BUFFS_Inode *entry;
    struct BUFFS_llist *next;
};

struct BUFFS_directory {
    int num_children;
    struct BUFFS_llist *head;
};

struct BUFFS_Inode {
    enum {
        BUFFS_File = 0,
        BUFFS_Dir = 1
    } type;

    char *name;
    int creator_pid;

    union {
        struct BUFFS_file *file;
        struct BUFFS_directory *dir;
    };
    struct BUFFS_Inode *parent;
};

struct BUFFS_llist *buffs_llist_getNth(struct BUFFS_llist *head, int n);
int buffs_llist_insert(struct BUFFS_llist *head, struct BUFFS_Inode *entry);
struct BUFFS_llist *buffs_llist_getLast(struct BUFFS_llist *head);

struct BUFFS_Inode *buffs_createDir(char *name);
struct BUFFS_Inode *buffs_createFile(char *name, size_t size);
int buffs_addInodeToDirInode(struct BUFFS_Inode *dir, struct BUFFS_Inode *child);

void *buffs_getLink(void *fs, char *path, uint32_t *meta);
int buffs_read(void *f, void *buf, int nbytes);
int buffs_write(void *f, void *buf, int nbytes);
int buffs_seek(void *f, int offset, int whence);
int buffs_creat(void *fs, char *path, unsigned int size);
int buffs_creatDir(void *fs, char *path);
struct DirectoryListing buffs_advListDirectory(void *fs, char *path);
int buffs_truncate(void *f, unsigned int len);

#endif