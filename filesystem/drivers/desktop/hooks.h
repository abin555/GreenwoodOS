#ifndef HOOKS_H
#define HOOKS_H

#include <stdint.h>
#include <gwos/vfs.h>

extern int (*vfs_meta_countRoot)();
extern int (*vfs_meta_getRoot)(int idx, struct VFS_Inode **dst_inode_ptr);
extern int (*MEM_findRegionIdx)(uint32_t size);
extern uint32_t (*MEM_reserveRegionBlock)(int idx, uint32_t size, uint32_t virtual_base, int type);
extern void (*MEM_freeRegionBlock)(uint32_t virtual, uint32_t size);
extern int (*select_program)(int slot);
extern int (*task_getCurrentID)();
extern int (*task_setRunningID)(int id);
extern int (*task_get_slot)(int task_idx);
extern int (*task_getCurrentPID)();
extern int (*taskID_fromPID)(int pid);
extern void (*buf_putChar)(uint32_t *buf, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
extern void (*buf_w_putChar)(uint32_t *buf, uint32_t w, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
extern void (*window_draw_cursor)(int x, int y);

int hooks_load();

#endif