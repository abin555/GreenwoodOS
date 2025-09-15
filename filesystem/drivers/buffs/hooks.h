#ifndef HOOKS_H
#define HOOKS_H

#include <stdint.h>
#include <gwos/vfs.h>

extern void (*vfs_addFS)(struct VFS_RootInterface *interface);
extern void *(*kmalloc)(size_t size);
extern void (*print_serial)(char *fmt, ...);
extern int (*get_task_id)();

int hooks_load();

#endif