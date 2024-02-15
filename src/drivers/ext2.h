#ifndef EXT2_H
#define EXT2_H

#include "drive.h"
#include "allocator.h"
#include "serial.h"
#include "console.h"
#include "utils.h"

#define EXT2_SUPER_MAGIC 0xEF53

struct ext2_super_block{ //Stored at 0x400 (sector 2)
	uint32_t inodes_count;
	uint32_t blocks_count;
	uint32_t r_blocks_count;
	uint32_t free_blocks_count;
	uint32_t free_inodes_count;
	uint32_t first_data_block;
	uint32_t log_block_size;	/* Block size */
	uint32_t log_frag_size;	/* Fragment size */
	uint32_t blocks_per_group;	/* # Blocks per group */
	uint32_t frags_per_group;	/* # Fragments per group */
	uint32_t inodes_per_group;	/* # Inodes per group */
	uint32_t mtime;		/* Mount time */
	uint32_t wtime;		/* Write time */
	uint16_t mnt_count;		/* Mount count */
	uint16_t max_mnt_count;	/* Maximal mount count */
	uint16_t magic;		/* Magic signature */
	uint16_t state;		/* File system state */
	uint16_t errors;		/* Behaviour when detecting errors */
	uint16_t minor_rev_level; 	/* minor revision level */
	uint32_t lastcheck;		/* time of last check */
	uint32_t checkinterval;	/* max. time between checks */
	uint32_t creator_os;		/* OS */
	uint32_t rev_level;		/* Revision level */
	uint16_t def_resuid;		/* Default uid for reserved blocks */
	uint16_t def_resgid;		/* Default gid for reserved blocks */
	/*
	 * These fields are for EXT2_DYNAMIC_REV superblocks only.
	 *
	 * Note: the difference between the compatible feature set and
	 * the incompatible feature set is that if there is a bit set
	 * in the incompatible feature set that the kernel doesn't
	 * know about, it should refuse to mount the filesystem.
	 * 
	 * e2fsck's requirements are more strict; if it doesn't know
	 * about a feature in either the compatible or incompatible
	 * feature set, it must abort and not try to meddle with
	 * things it doesn't understand...
	 */
	uint32_t first_ino; 		/* First non-reserved inode */
	uint16_t inode_size; 		/* size of inode structure */
	uint16_t block_group_nr; 	/* block group # of this superblock */
	uint32_t feature_compat; 	/* compatible feature set */
	uint32_t feature_incompat; 	/* incompatible feature set */
	uint32_t feature_ro_compat; 	/* readonly-compatible feature set */
	uint8_t	uuid[16];		/* 128-bit uuid for volume */
	char	volume_name[16]; 	/* volume name */
	char	last_mounted[64]; 	/* directory where last mounted */
	uint32_t	algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_COMPAT_PREALLOC flag is on.
	 */
	uint8_t	prealloc_blocks;	/* Nr of blocks to try to preallocate*/
	uint8_t prealloc_dir_blocks;	/* Nr to preallocate for dirs */
	uint16_t padding1;
	/*
	 * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
	uint8_t	s_journal_uuid[16];	/* uuid of journal superblock */
	uint32_t	s_journal_inum;		/* inode number of journal file */
	uint32_t	s_journal_dev;		/* device number of journal file */
	uint32_t	s_last_orphan;		/* start of list of inodes to delete */
	uint32_t	s_hash_seed[4];		/* HTREE hash seed */
	uint8_t	s_def_hash_version;	/* Default hash version to use */
	uint8_t	s_reserved_char_pad;
	uint16_t	s_reserved_word_pad;
	uint32_t	s_default_mount_opts;
 	uint32_t	s_first_meta_bg; 	/* First metablock block group */
	uint32_t	s_reserved[190];	/* Padding to the end of the block */
};

struct EXT2_BlockGroupDescriptor{
	uint32_t block_usage_addr;
	uint32_t inode_usage_addr;
	uint32_t inode_table_addr;
	uint16_t num_unalloc_blocks_per_group;
	uint16_t num_unalloc_inodes_per_group;
	uint16_t num_directories_per_group;
	uint16_t _pad;
	uint32_t _reserved[3];
};

#define EXT2_InodeType_FIFO 0x1000
#define EXT2_InodeType_CharDevice 0x2000
#define EXT2_InodeType_Directory 0x4000
#define EXT2_InodeType_BlockDevice 0x6000
#define EXT2_InodeType_RegularFile 0x8000
#define EXT2_InodeType_SymbolicLink 0xA000
#define EXT2_InodeType_Unix_Socket 0xC000

#define EXT2_ROOT_INODE 2

struct EXT2_Inode{
	uint16_t type_perms;
	uint16_t userId;
	uint32_t lsbSize;
	uint32_t lastAccessTime;
	uint32_t creationTime;
	uint32_t lastModificationTime;
	uint32_t deletionTime;
	uint16_t GroupID;
	uint16_t hardLinkCount;
	uint32_t diskSectorsCount;
	uint32_t flags;
	uint32_t OSval;
	uint32_t BlockPointers[15];
	uint32_t GenerationNumber;
	uint32_t ExtendedAttributes;
	uint32_t msbSize;//If file, ACL if directory, invalid if version = 0
	uint32_t fragmentBlock;
	uint8_t  OSval2[12];
};

struct EXT2_Directory{
	uint32_t inode;
	uint16_t entry_size;
	uint8_t name_length;
	uint8_t type;
	char name[];
};

struct EXT2_FS{
	struct DRIVE *drive;
	char *buf;
	uint32_t ext2_version;
	uint32_t block_size;
	uint32_t inode_size;
	uint32_t sectors_per_block;
	uint32_t block_count;
	uint32_t inode_count;
	uint32_t group_count;
	uint32_t starting_block_number;
	uint32_t blocks_per_group;
	uint32_t inodes_per_group;
	
	uint32_t *block_addr_block_usage_map;
	uint32_t *block_addr_inode_usage_map;
	uint32_t *inode_table_starting_addr;

	uint32_t first_inode;
};

void *ext2_read_block(struct EXT2_FS *ext2, uint32_t block_id);
struct EXT2_Inode ext2_read_inode_data(struct EXT2_FS *ext2, uint32_t inodeIdx);
void ext2_console_printDirectory(struct CONSOLE *console, struct EXT2_FS *ext2, uint32_t inodeIdx);
int ext2_check_format(struct DRIVE *drive);
void ext2_listDirectory(struct CONSOLE *console, struct EXT2_FS *ext2, char *path);
uint32_t ext2_get_inodeIdx_from_path(struct EXT2_FS *ext2, char *path);

#endif