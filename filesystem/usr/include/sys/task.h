#ifndef SYS_TASK_H
#define SYS_TASK_H

typedef enum {
    ALWAYS = 1,
    ONFOCUS = 2,
    NOCHILD = 3,
	NEVER = 4
} ScheduleType;

struct task_file_association {
    int creator_fd;
    int new_fd;
};

struct task_file_ctx {
    int num_fds;
    struct task_file_association fds[];
};

struct exec_spec_ctx {
    char *filename;
    int argc;
    char **argv;
    struct task_file_ctx *file_ctx;
};

int exec(char *filename, int argc, char **argv);
int exec_spec(struct exec_spec_ctx *ctx);
void waitpid(int pid);//Program does not run while given pid is alive.
void set_schedule(ScheduleType type);
void yield();
void task_lock(int state);
int manual_task(void *address, char *name);

#endif