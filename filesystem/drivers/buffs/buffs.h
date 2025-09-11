#ifndef BUFFS_H
#define BUFFS_H

#include <stdint.h>

struct BUFFS_file {
    char *name;
    int creator_pid;

    uint32_t size;

    uint32_t num_pages;
    void *page_base;

};

struct BUFFS_directory;

struct BUFFS_DirEntry {
    enum {
        BUFFS_File,
        BUFFS_Dir
    } type;
    union {
        struct BUFFS_file *file;
        struct BUFFS_directory *dir;
    };
};

struct BUFFS_llist;

struct BUFFS_llist {
    struct BUFFS_DirEntry entry;
    struct BUFFS_llist *next;
};

struct BUFFS_directory {
    char *name;
    int creator_pid;

    int num_children;
    struct BUFFS_llist *head;
};

struct BUFFS {
    struct BUFFS_directory root;
};

#endif