#include "loader.h"
#include "elf.h"
#include "multitasking.h"
#include "program.h"

struct LoaderSystem loader_list[LOADER_MAX];
int loader_count;

struct Loader loader_buf;

void loader_cdev_write(void *cdev, int offset, int nbytes, int *head){
    (void) cdev;
    if(nbytes != sizeof(loader_buf) || offset != 0) return;
    *head = 0;
    loader_addLoader(loader_buf);
    memset(&loader_buf, 0, sizeof(loader_buf));
    return;
}

int loader_init(){
    for(int i = 0; i < LOADER_MAX; i++){
        loader_list[i].creator_pid = -1;
        loader_list[i].enabled = 0;
        memset(loader_list[i].loader.name, 0, sizeof(loader_list[i].loader.name));
        loader_list[i].loader.validate_program_fn = NULL;
        loader_list[i].loader.load_program_fn = NULL;
    }
    loader_count = 0;
    loader_addLoader(loader_getDefault());

    struct VFS_Inode *vfs_sysroot = vfs_findRoot('-');
    struct SysFS_Inode *sysfs = vfs_sysroot->root->interface->root;

    struct SysFS_Chardev *loader_cdev = sysfs_createCharDevice(
        (char *) &loader_buf,
        sizeof(loader_buf),
        CDEV_WRITE
    );
    sysfs_setCallbacks(
        loader_cdev,
        loader_cdev_write,
        NULL,
        NULL,
        NULL
    );
    struct SysFS_Inode *sys_inode = sysfs_find(sysfs, "sys\0", NULL);
    struct SysFS_Inode *loader_inode = sysfs_mkcdev("loader", loader_cdev);
    sysfs_addChild(sys_inode, loader_inode);

    return 1;
}

int loader_addLoader(struct Loader loader){
    if(loader_count == LOADER_MAX) return 0;
    print_serial("[LOADER] added \"%s\"\n", loader.name);
    loader_list[loader_count++] = (struct LoaderSystem) {
        loader,
        task_getCurrentPID(),
        1
    };
    return 1;
}

struct Loader *loader_identify(int systemFD){
    for(int i = 0; i < loader_count; i++){
        struct LoaderSystem *system = &loader_list[i];
        if(system->enabled == 0) continue;
        vfs_seek(systemFD, 0, 0);
        if(system->loader.validate_program_fn == NULL) continue;
        if(system->loader.validate_program_fn(systemFD)){
            return &system->loader;
        }
    }
    return NULL;
}

int loader_loadProgram(int systemFD, char *filename, int argc, char **argv, void *vctx){
    struct Loader *loader = loader_identify(systemFD);
    if(loader == NULL){
        return 0;
    }
    print_serial("[LOADER] Using \"%s\" to load \"%s\"\n", loader->name, filename);
    if(loader->load_program_fn == NULL) return -1;
    return loader->load_program_fn(systemFD, filename, argc, argv, vctx);
}

int loader_default_isValid(int systemFD){
    if(elf_check_supported(systemFD)){
        if(!elf_is_dyn(systemFD)){
            return 1;
        }
    }
    return 0;
}

int loader_default_loadProgram(int systemFD, char *filename, int argc, char **argv, void *vctx){
    int slot = program_findSlot();
    //print_serial("Loading Program %s to slot %d\n", filename, slot);
    uint32_t entry = elf_get_entry_addr(systemFD);
    //print_serial("[PROGRAM] Is ELF Format, entry @ 0x%x!\n", entry);
    memset((void *) (program_region_virt_base + PROGRAM_MAX_SIZE*slot), 0, PROGRAM_MAX_SIZE);
    elf_load(systemFD, (void *) (program_region_virt_base + PROGRAM_MAX_SIZE*slot));
    //vfs_read(file, (char *) (program_region_virt_base + 0x400000*slot), size);
    //memcpy((void *) (PROGRAM_VIRT_REGION_BASE + 0x400000*slot), (void *) (PROGRAM_VIRT_REGION_BASE + 0x400000*slot + 0x1000), fsize(file) - 0x1000);	
    int pid = start_task((void *)entry, slot, argc, argv, filename, vctx);
    return pid;
}

struct Loader loader_getDefault(){
    struct Loader loader = {
        .name = "SMPL ELF",
        .validate_program_fn = loader_default_isValid,
        .load_program_fn = loader_default_loadProgram
    };
    return loader;
}