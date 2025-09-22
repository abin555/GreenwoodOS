#ifndef GWOS_VFS_H
#define GWOS_VFS_H

#include <stdint.h>

struct DRIVE {
	enum{
		Drive_AHCI
	} type;
	union {
		void *ahci;
	} driver;

	enum{
		FAT32,
		FAT16,
		FAT12,
		exFAT,
		ISO9660,
		RAW,
		EXT2
	} format;
	union{
		void *fat32;
		void *ISO;
		void *ext2;
	} format_info;
	char locked;
	char ready;
	char identity;
};

enum DirEntryType{
	ENTRY_FILE = 0,
	ENTRY_DIRECTORY = 1
};

struct DirectoryEntry{
	char filename[50];
	int name_len;
	uint32_t type;
};

struct DirectoryListing{
	char *directory_path;
	int directory_path_len;
	struct DirectoryEntry *entries;
	int num_entries;
};


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
};

#endif