#include "proc.h"
#include "allocator.h"
#include "vfs.h"
#include "sysfs.h"
#include "multitasking.h"
#include "program.h"
/*
PROC file system:
Command and control format:
Write PROC_Request command to file.
Read Response from file.

PROC remembers caller ID, gives appropriate response on next read based on reader id.
*/

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
    int pid;
};

#define PROC_queue_len 10

struct PROC_queue_entry {
    int open;
    int pending;
    int requestor_id;
    struct PROC_Request req;
};

struct PROC_queue {
    struct PROC_queue_entry entries[PROC_queue_len];
};

struct PROC_queue proc_queue;

int proc_task_write_callback(void *cdev, void *buf, int woffset, int nbytes, int *head){
    struct SysFS_Chardev *taskdev = cdev;
    if(taskdev == NULL || woffset != 0) return 0;
    //A request has been made.
    
    //Check that write size is correct for a request:
    if(nbytes != sizeof(struct PROC_Request)){
        return 0;
    }

    struct PROC_Request req;
    memcpy(
        &req,
        buf,
        nbytes
    );
    if(req.cmd == PROC_PAUSE_TASK){
        print_serial("[PROC FS] Removing %d from schedule\n", req.task_id);
        tasks[req.task_id].schedule_type = NEVER;
        return nbytes;
    } 
    else if(req.cmd == PROC_RESUME_TASK){
        tasks[req.task_id].schedule_type = ALWAYS;
        return nbytes;
    }
    else {
        int requester_id = task_running_idx;
        struct PROC_queue_entry *entry = NULL;
        for(int i = 0; i < PROC_queue_len; i++){
            if(proc_queue.entries[i].open){
                entry = &proc_queue.entries[i];
            }
        }
        if(entry == NULL) return 0;

        entry->open = 0;
        entry->pending = 1;
        entry->requestor_id = requester_id;
        memcpy(
            &entry->req,
            buf,
            nbytes
        );
        *head = 0;
        return nbytes;
    }  
    
}

int proc_task_read_callback(void *cdev, void *buf, int roffset, int nbytes, int *head){
    struct SysFS_Chardev *taskdev = cdev;
    if(taskdev == NULL || roffset != 0) return 0;

    int requester_id = task_running_idx;
    struct PROC_queue_entry *entry = NULL;
    for(int i = 0; i < PROC_queue_len; i++){
        if(!proc_queue.entries[i].open && proc_queue.entries[i].pending && proc_queue.entries[i].requestor_id == requester_id){
            entry = &proc_queue.entries[i];
        }
    }
    if(entry == NULL) return 0;

    entry->pending = 0;
    entry->open = 1;
    entry->requestor_id = -1;
    int nread = 0;
    switch(entry->req.cmd){
        case PROC_GET_TASK: {
            int real_id = 0;
            for(int i = 0; i < MAX_TASKS; i++){
                if(real_id == entry->req.task_id) break;
                if(tasks[i].slot_running == 0) continue;
                real_id++;
            }
            struct PROC_Response_Task_Info task_info;
            task_info.cmd = entry->req.cmd;
            task_info.task_id = entry->req.task_id;
            task_info.slot = tasks[task_info.task_id].program_slot;
            task_info.schedule_type = tasks[task_info.task_id].schedule_type;
            task_info.slot_active = tasks[task_info.task_id].slot_active;
            task_info.name = tasks[task_info.task_id].task_name;
            task_info.pid = tasks[task_info.task_id].pid;
            memcpy(
                buf,
                &task_info,
                (int) sizeof(task_info) <= nbytes ? (int) sizeof(task_info) : nbytes
            );
            nread = (int) sizeof(task_info) <= nbytes ? (int) sizeof(task_info) : nbytes;
            break;
        };
        case PROC_GET_TASK_COUNT: {
            if(nbytes == sizeof(int)){
                int ntasks = 0;
                for(int i = 0; i < MAX_TASKS; i++){
                    if(tasks[i].slot_active){
                        ntasks++;
                    }
                }
                nread = (int) sizeof(int);
                *((int *) buf) = ntasks;
            }
            break;
        };
        case PROC_PAUSE_TASK:
            break;
        case PROC_RESUME_TASK:
            break;
    }
    *head = 0;
    return nread;
}

struct SysFS_Inode *proc_fs_initSlotFiles(){
    struct SysFS_Inode *slot_dir = sysfs_mkdir("slot");
    char name[11];
    for(int i = 0; i < PROGRAM_MAX; i++){
        struct SysFS_Chardev *slot_cdev = sysfs_createCharDevice((char *) (program_region_virt_base + i*(PROGRAM_MAX_SIZE)), PROGRAM_MAX_SIZE, CDEV_READ | CDEV_WRITE);
        memset(name, 0, sizeof(name));
        snprintf(name, sizeof(name)-1, "mem%d", i);
        struct SysFS_Inode *cdev_inode = sysfs_mkcdev(name, slot_cdev);
        sysfs_addChild(slot_dir, cdev_inode);
    }
    return slot_dir;
}

void proc_fs_init(){
    print_serial("[PROC] Init!\n");
    for(int i = 0; i < PROC_queue_len; i++){
        proc_queue.entries[i].open = 1;
        proc_queue.entries[i].pending = 0;
        proc_queue.entries[i].requestor_id = -1;
    }

    struct VFS_Inode *vfs_sysfs = vfs_findRoot('-');
    struct SysFS_Inode *sysfs = vfs_sysfs->root->interface->root;

    struct SysFS_Inode *proc_dir = sysfs_mkdir("proc");
    sysfs_addChild(sysfs, proc_dir);

    struct SysFS_Chardev *task_cdev = sysfs_createCharDevice(NULL, sizeof(struct PROC_Response_Task_Info), CDEV_READ | CDEV_WRITE);
    sysfs_setCallbacks(task_cdev,
        NULL,
        NULL,
        proc_task_write_callback,
        proc_task_read_callback
    );
    struct SysFS_Inode *task_file = sysfs_mkcdev("task", task_cdev);
    sysfs_addChild(proc_dir, task_file);
    sysfs_addChild(proc_dir, proc_fs_initSlotFiles());
}