#ifndef HOOKS_H
#define HOOKS_H

#include <stdint.h>
#include <gwos/vfs.h>

extern void (*vfs_addFS)(struct VFS_RootInterface *interface);
extern void *(*kmalloc)(size_t size);
extern void (*print_serial)(char *fmt, ...);
extern int (*get_task_id)();
extern int (*MEM_findRegionIdx)(uint32_t size);
extern uint32_t (*MEM_reserveRegionBlock)(int idx, uint32_t size, uint32_t virtual_base, int type);
extern void (*MEM_freeRegionBlock)(uint32_t virtual, uint32_t size);


int hooks_load();

#endif