#ifndef HOOKS_H
#define HOOKS_H

#include <stdint.h>
#include <gwos/vfs.h>

extern int (*vfs_meta_countRoot)();
extern int (*vfs_meta_getRoot)(int idx, struct VFS_Inode **dst_inode_ptr);
extern int (*MEM_findRegionIdx)(uint32_t size);
extern uint32_t (*MEM_reserveRegionBlock)(int idx, uint32_t size, uint32_t virtual_base, int type);
extern void (*MEM_freeRegionBlock)(uint32_t virtual, uint32_t size);


int hooks_load();

#endif