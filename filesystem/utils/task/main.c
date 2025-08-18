#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum PROC_CMD {
    PROC_GET_TASK_COUNT = 0,
    PROC_GET_TASK = 1,
    PROC_PAUSE_TASK = 2,
    PROC_RESUME_TASK = 3,
};

struct PROC_Request {
    enum PROC_CMD cmd;
    int task_id;
    unsigned int command;
};

struct PROC_Response_Task_Info {
    enum PROC_CMD cmd;
    int task_id;
    int slot;
    uint8_t slot_active;
    int schedule_type;
    char *name;
};

int main(int argc, char **argv){
    FILE *task_file = fopen("/-/proc/task", "rw");
    struct PROC_Request req_ntasks;
    req_ntasks.cmd = PROC_GET_TASK_COUNT;
    fwrite(&req_ntasks, sizeof(req_ntasks), 1, task_file);
    int n_tasks;
    fread(&n_tasks, sizeof(n_tasks), 1, task_file);
    printf("There are %d tasks!\n", n_tasks);

    for(int i =0; i < n_tasks; i++){
        struct PROC_Request req_task;
        req_task.cmd = PROC_GET_TASK;
        req_task.task_id = i;
        fwrite(&req_task, sizeof(req_task), 1, task_file);
        struct PROC_Response_Task_Info task_info;
        fread(&task_info, sizeof(task_info), 1, task_file);
        printf("#%d - Slot: %d \"%s\"\n", i, task_info.slot, task_info.name);
    }

    if(argc == 2){
        int pause_id = -1 * atoi(argv[1]);
        struct PROC_Request req_pause;
        req_pause.cmd = PROC_PAUSE_TASK;
        req_pause.task_id = pause_id;
        printf("Pausing #%d\n", pause_id);
        fwrite(&req_pause, sizeof(req_pause), 1, task_file);
    }

    fclose(task_file);
    return 0;
}