#ifndef HOOKS_H
#define HOOKS_H

#include <stdint.h>

extern void (*buf_putChar)(uint32_t *buf, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
extern void (*buf_w_putChar)(uint32_t *buf, uint32_t w, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
extern void (*select_program)(int task_id);
extern int (*get_task_id)();
extern void (*set_task_id)();
extern int (*get_program_slot)(int task_id);
extern void (*memfcpy)(void *, void *, uint32_t);
extern void (*print_serial)(char *fmt, ...);

void hooks_attach();

#endif