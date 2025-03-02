#include "sysfs.h"
#include "allocator.h"
#include "memory.h"
#include "utils.h"
#include "vfs.h"

struct SysFS_Inode *sysfs_createRoot(){
    struct SysFS_Inode *sysfs = malloc(sizeof(struct SysFS_Inode));
    if(sysfs == NULL) return NULL;
    sysfs->type = SysFS_Directory;
    char rootname[] = "SYSROOT";
    memcpy(sysfs->name, rootname, sizeof(rootname));
    sysfs->namelen = sizeof(rootname)-1;
    memset(
        sysfs->data.dir.children,
        0,
        sizeof(sysfs->data.dir.children)
    );
    sysfs->data.dir.numChildren = 0;
    sysfs->parent = sysfs;
    print_serial("[SYSFS] Created Root Filesystem\n");
    return sysfs;
}

int sysfs_addChild(struct SysFS_Inode *parent, struct SysFS_Inode *child){
    if(parent == NULL || child == NULL) return 1;
    if(!(parent->type == SysFS_Directory)) return 1;

    if(parent->data.dir.numChildren == 20) return 2;
    parent->data.dir.children[parent->data.dir.numChildren++] = child;
    child->parent = parent;
    print_serial("[SYSFS] Added Child \"%s\" to Parent \"%S\"\n", child->name, parent->name);
    return 0;
}

struct SysFS_Inode *sysfs_mkdir(char *dirname){
    struct SysFS_Inode *sysfs = malloc(sizeof(struct SysFS_Inode));
    if(sysfs == NULL) return NULL;
    sysfs->type = SysFS_Directory;
    memcpy(sysfs->name, dirname, strlen(dirname) < 20 ? strlen(dirname) : 19);
    sysfs->namelen = strlen(dirname) < 20 ? strlen(dirname) : 19;
    memset(
        sysfs->data.dir.children,
        0,
        sizeof(sysfs->data.dir.children)
    );
    sysfs->data.dir.numChildren = 0;
    sysfs->parent = NULL;
    print_serial("[SYSFS] Created Directory \"%s\"\n", sysfs->name);
    return sysfs;
}

struct SysFS_Inode *sysfs_mkcdev(char *name, struct SysFS_Chardev *cdev){
    struct SysFS_Inode *sysfs = malloc(sizeof(struct SysFS_Inode));
    if(sysfs == NULL) return NULL;
    sysfs->type = SysFS_Chardev;
    memcpy(sysfs->name, name, strlen(name) < 20 ? strlen(name) : 19);
    sysfs->namelen = strlen(name) < 20 ? strlen(name) : 19;
    sysfs->data.chardev = cdev;
    sysfs->parent = NULL;
    print_serial("[SYSFS] Created Character Device File \"%s\"\n", sysfs->name);
    return sysfs;
}

struct SysFS_Chardev *sysfs_createCharDevice(char *buf, int buf_size, CDEV_PERMS perms){
    if(buf == NULL) return NULL;
    struct SysFS_Chardev *cdev = malloc(sizeof(struct SysFS_Chardev));
    if(cdev == NULL) return NULL;
    cdev->buf = buf;
    cdev->buf_size = buf_size;
    cdev->perms = perms;
    cdev->write_callback = NULL;
    cdev->read_callback = NULL;
    print_serial("[SYSFS] Created CDEV of size %d\n", cdev->buf_size);
    return cdev;
}

void sysfs_setCallbacks(struct SysFS_Chardev *cdev, void (*write_callback)(void *, int offset, int nbytes, int *head), void (*read_callback)(void *, int offset, int nbytes, int *head)){
    if(cdev == NULL) return;
    cdev->write_callback = write_callback;
    cdev->read_callback = read_callback;
}

void sysfs_debugTree(struct SysFS_Inode *fs, int depth){
    for(int i = 0; i < depth; i++){
        print_serial(" ");
    }
    print_serial("%s\n", fs->name);
    if(fs->type == SysFS_Directory){
        for(int i = 0; i < fs->data.dir.numChildren; i++){
            sysfs_debugTree(fs->data.dir.children[i], depth+1);
        }
    }
}

int sysfs_read(struct VFS_File *file, void *buf, int nbytes){
    print_serial("[SYSFS] Reading %d bytes with head at %d and file size %d\n", nbytes, file->head, file->inode.fs.sysfs->data.chardev->buf_size);
    if(file == NULL || buf == NULL || nbytes == 0) return 0;
    if(file->inode.type != VFS_SYS) return 0;

    struct SysFS_Inode *sysfs = file->inode.fs.sysfs;
    if(sysfs->type != SysFS_Chardev) return 0;
    struct SysFS_Chardev *cdev = sysfs->data.chardev;
    if((cdev->perms & CDEV_READ) == 0) return 0;
    
    int i = 0;
    for(; i < nbytes && file->head < cdev->buf_size; i++){
        ((char *)buf)[i] = cdev->buf[file->head++];
    }
    if(cdev->read_callback != NULL){
        cdev->read_callback(cdev, file->head-i, i, &file->head);
    }
    return i;
}

int sysfs_write(struct VFS_File *file, void *buf, int nbytes){
    print_serial("[SYSFS] Writing %d bytes with head at %d and file size %d\n", nbytes, file->head, file->inode.fs.sysfs->data.chardev->buf_size);
    if(file == NULL || buf == NULL || nbytes == 0) return 0;
    if(file->inode.type != VFS_SYS) return 0;
    
    struct SysFS_Inode *sysfs = file->inode.fs.sysfs;
    if(sysfs->type != SysFS_Chardev){
        print_serial("[SYSFS] Wrong FS device\n");
        return 0;
    }
    struct SysFS_Chardev *cdev = sysfs->data.chardev;
    if((cdev->perms & CDEV_WRITE) == 0){
        print_serial("[SYSFS] No write permission");
        return 0;
    };
    print_serial("[SYSFS] Start Writing\n");
    int i = 0;
    for(; i < nbytes && file->head < cdev->buf_size; i++){
        cdev->buf[file->head++] = ((char *)buf)[i];
    }
    if(cdev->write_callback != NULL){
        cdev->write_callback(cdev, file->head-i, i, &file->head);
    }
    return i;
}

struct SysFS_Inode *sysfs_find(struct SysFS_Inode *root, char *path){
    if(root == NULL) return NULL;
    if(path == NULL) return root;

    print_serial("[SYSFS] Finding \"%s\" in \"%s\"\n", path, root->name);
    sysfs_debugTree(root, 0);
    if(path[0] == 0){
        return root;
    }

    if(!strcmp(root->name, path)){
        print_serial("[SYSFS] Found %s\n", root->name);
        return root;
    }

    int part_len = 0;
    for(int i = 0; path[i] != NULL && path[i] != '/'; i++){
        part_len++;
    }
    char buf[100];
    memset(buf, 0, 100);
    memcpy(buf, path, part_len);
    print_serial("%s %d\n", buf, part_len);


    if(root->type == SysFS_Directory){
        for(int i = 0; i < root->data.dir.numChildren; i++){
            if(root->data.dir.children[i] == NULL) continue;
            print_serial("[SYSFS] Checking #%d - %s=%s\n", i, root->data.dir.children[i]->name, buf);
            if(!strcmp(buf, root->data.dir.children[i]->name)){
                if(root->data.dir.children[i]->type == SysFS_Directory){
                    struct SysFS_Inode *inode_candidate = sysfs_find(root->data.dir.children[i], path+part_len+1);
                    if(inode_candidate != NULL) return inode_candidate;
                }
                else{
                    return root->data.dir.children[i];
                }
            }            
        }
    }

    return NULL;
}
