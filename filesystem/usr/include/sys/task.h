#ifndef SYS_TASK_H
#define SYS_TASK_H

typedef enum {
    ALWAYS = 1,
    ONFOCUS = 2,
    NOCHILD = 3,
	NEVER = 4
} ScheduleType;

void exec(char *filename, int argc, char **argv);
void set_schedule(ScheduleType type);
void yield();
void task_lock(int state);
void manual_task(void *address, char *name);

#endif