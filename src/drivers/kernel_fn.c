#include "kernel_fn.h"
#include "vfs.h"
#include "sysroot.h"
#include "multitasking.h"

struct kernel_fn_list kernel_fn_list;

void kernel_fn_init(){
    print_serial("[KERNEL FN] Init\n");
    struct VFS_Inode *vfs_sysroot = vfs_findRoot('-');
    struct SysFS_Inode *sysfs = vfs_sysroot->root->interface->root;

    struct SysFS_Inode *devdir = sysfs_find(sysfs, "dev\0", NULL);

    struct SysFS_Chardev *kernel_fn_dev = sysfs_createCharDevice((char *) &kernel_fn_list, sizeof(kernel_fn_list), CDEV_READ);
    struct SysFS_Inode *kernel_fn_inode = sysfs_mkcdev("kernel", kernel_fn_dev);
    sysfs_addChild(devdir, kernel_fn_inode);

    kernel_fn_add(vfs_read, 3, "read");
    kernel_fn_add(vfs_write, 3, "write");
    kernel_fn_add(vfs_open, 2, "open");
    kernel_fn_add(vfs_close, 1, "close");
    kernel_fn_add(vfs_seek, 3, "lseek");
    kernel_fn_add(set_schedule_task, 2, "set_schedule_task");
    kernel_fn_add(task_getCurrentID, 0, "get_task_id");
    kernel_fn_add(vfs_addFS, 1, "vfs_addFS");
    kernel_fn_add(drive_read, 4, "drive_read");
    kernel_fn_add(drive_write, 4, "drive_write");
    kernel_fn_add(drives, -1, "drives_s");
    kernel_fn_add(&drive_count, -1, "drive_count_s");
    kernel_fn_add(malloc, 1, "kmalloc");
    kernel_fn_add(print_serial, 100, "print_serial");
    kernel_fn_add(buf_putChar, 6, "buf_putChar");
    kernel_fn_add(buf_w_putChar, 7, "buf_w_putChar");
    kernel_fn_add(select_program, 1, "select_program");
    kernel_fn_add(task_setCurrentID, 1, "set_task_id");
    kernel_fn_add(task_get_program_slot, 1, "get_program_slot");
    kernel_fn_add(memfcpy, 3, "memfcpy");
}

void kernel_fn_add(void *fn, int n_args, char *name){
    if((long unsigned int) kernel_fn_list.num_fns > sizeof(kernel_fn_list.fns) / sizeof(kernel_fn_list.fns[0])){
        print_serial("[KERNEL FN] Error: unable to add \"%s\" too many definitions\n");
        return;
    }
    if(name == NULL){
        print_serial("[KERNEL FN] Error: fn 0x%x has null name!\n", fn);
        return;
    }
    int name_len = strlen(name) + 1;
    struct kernel_fn_def *def = malloc(sizeof(struct kernel_fn_def) + name_len);
    memset(def, 0, sizeof(struct kernel_fn_def) + name_len);
    def->fn = fn;
    def->n_args = n_args;
    def->name_len = name_len;
    memcpy(def->name, name, name_len);
    kernel_fn_list.fns[kernel_fn_list.num_fns++] = def;

}