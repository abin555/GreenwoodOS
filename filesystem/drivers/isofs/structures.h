#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdint.h>

#define getLe32(x) *((uint32_t *) &x.le)
#define getLe16(x) *((uint16_t *) &x.le)

typedef struct { uint16_t le; }__attribute__((packed))          l9660_luint16;
typedef struct { uint16_t be; }__attribute__((packed))          l9660_buint16;
typedef struct { uint8_t le[2], be[2]; }__attribute__((packed))   l9660_duint16;
typedef struct { uint8_t le[4]; }__attribute__((packed))          l9660_luint32;
typedef struct { uint8_t be[4]; }__attribute__((packed))          l9660_buint32;
typedef struct { uint8_t le[4], be[4]; }__attribute__((packed))   l9660_duint32;

struct ISO_Descriptor_Time{
    char d[17];
}__attribute__((packed));
struct ISO_File_Time{
    char d[7];
}__attribute__((packed));

struct ISO_PathTable_Entry{
    uint8_t Directory_Name_Length;
    uint8_t extended_attribute_length;
    uint32_t Location_of_Extent;
    uint16_t Directory_Number_Parent_Directory;
    char name[];
}__attribute__((packed));

struct ISO_Directory_Entry{
    uint8_t         length;
    uint8_t         xattr_length;
    l9660_duint32   sector;
    l9660_duint32   size;
    struct ISO_File_Time  time;
    uint8_t         flags;
    uint8_t         unit_size;
    uint8_t         gap_size;
    l9660_duint16   vol_seq_number;
    uint8_t         name_len;
    char            name[0];
}__attribute__((packed));

struct ISO_vdesc_header{
    uint8_t type;
    char magic[5];
    uint8_t version;
}__attribute__((packed));

struct ISO_Primary_Volume_Descriptor{
    struct ISO_vdesc_header  hdr;
    char                pad0[1];
    char                system_id[32];
    char                volume_id[32];
    char                pad1[8];
    l9660_duint32       volume_space_size;
    char                pad2[32];
    l9660_duint16       volume_set_size;
    l9660_duint16       volume_seq_number;
    l9660_duint16       logical_block_size;
    l9660_duint32       path_table_size;
    l9660_luint32       path_table_le;
    l9660_luint32       path_table_opt_le;
    l9660_buint32       path_table_be;
    l9660_buint32       path_table_opt_be;
    union {
        struct ISO_Directory_Entry root_dir_ent;
        char            pad3[34];
    };
    char                volume_set_id[128];
    char                data_preparer_id[128];
    char                app_id[128];
    char                copyright_file[38];
    char                abstract_file[36];
    char                bibliography_file[37];
    struct ISO_Descriptor_Time volume_created;
    struct ISO_Descriptor_Time volume_modified;
    struct ISO_Descriptor_Time volume_expires;
    struct ISO_Descriptor_Time volume_effective;
    uint8_t             file_structure_version;
    char                pad4[1];
    char                app_reserved[512];
    char                reserved[653];
}__attribute__((packed));

union ISO_Volume_Descriptor{
    struct ISO_vdesc_header hdr;
    char _bits[2048];
}__attribute__((packed));

struct ISO9660;

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
		struct ISO9660 *ISO;
		void *ext2;
	} format_info;
	char locked;
	char ready;
	char identity;
};

struct ISO9660{
	struct DRIVE *drive;
	char *buf;
	//struct ISO9660_FS_Entry root;
    uint32_t root_directory_sector;
    uint32_t nextTableSector;
    uint32_t nextFileSector;
};

struct ISO9660_File {
    uint32_t sector;
    uint32_t size;
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

struct kernel_fn_def {
    void *fn;
    int n_args;
    int name_len;
    char name[];
} __attribute__((packed));

#endif