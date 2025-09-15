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
        BUFFS_File,
        BUFFS_Dir
    } type;

    char *name;
    int creator_pid;

    union {
        struct BUFFS_file *file;
        struct BUFFS_directory *dir;
    };
    struct BUFFS_Inode *parent;
};

struct BUFFS_Inode *buffs_createDir(char *name);
struct BUFFS_Inode *buffs_createFile(char *name, size_t size);

void *buffs_getLink(void *fs, char *path, uint32_t *meta);

#endif