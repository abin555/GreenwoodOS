#include "monitors.h"
#include "allocator.h"
#include "vfs.h"
#include "sysfs.h"

int monitor_heap_read(void *dev, void *buf, int roffset, int nbytes, int *head){
    struct SysFS_Chardev *heapdev = dev;
    int n = snprintf(
        heapdev->buf,
        heapdev->buf_size,
        "Heap Size: 0x%x\nTotal Alloc: 0x%x\nFree: 0x%x\n",
        alloc_table_size,
        total_alloc,
        alloc_table_size - total_alloc
    );
    heapdev->buf[n] = '\0';
    //print_serial(heapdev->buf);
    int step = roffset;
    int i;
    for(i = 0; i < nbytes && step < heapdev->buf_size && i < n; i++){
        ((char *) buf)[i] = heapdev->buf[step++];
    }
    //print_serial("Wrote %d bytes with snprintf of %d bytes at offset %d\n", i, n, roffset);
    *head += i;
    return i;
}

void monitors_init(){
    print_serial("[MONITORS] Init\n");
    struct VFS_Inode *vfs_sysfs = vfs_findRoot('-');
    struct SysFS_Inode *sysfs = vfs_sysfs->root->interface->root;

    struct SysFS_Inode *monitor_dir = sysfs_mkdir("kernel");
    sysfs_addChild(sysfs, monitor_dir);

    struct SysFS_Chardev *heap_cdev = sysfs_createCharDevice(
        malloc(100),
        100,
        CDEV_READ
    );
    sysfs_setCallbacks(
        heap_cdev,
        NULL,
        NULL,
        NULL,
        monitor_heap_read
    );
    struct SysFS_Inode *heap = sysfs_mkcdev("kheap", heap_cdev);
    sysfs_addChild(monitor_dir, heap);
}
