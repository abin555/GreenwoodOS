#include "vfs.h"

#define VFS_maxFiles 200
struct VFS_File VFS_fileTable[VFS_maxFiles];

#define VFS_maxRootInodes 10
struct VFS {
    struct VFS_Inode inodes[VFS_maxRootInodes];
    int num_inodes;
} VFS;

extern void expandPath(char *dest, int dest_size, struct DIRECTORY *dir, char *path);

void vfs_init(){
    print_serial("[VFS] Init\n");
    memset(&VFS.inodes, 0, sizeof(VFS.inodes));
    VFS.num_inodes = 0;

    for(int i = 0; i < drive_count; i++){
        print_serial("[VFS] Adding Drive %d (type %d)\n", i, drives[i]->format);
        switch(drives[i]->format){
            case EXT2:
                vfs_addFS(ext2_generateVFSRoot(drives[i]->format_info.ext2));
                break;
            default:
                break;
        }
        //vfs_addRoot(drives[i]);
    }
    for(int i = 0; i < VFS_maxFiles; i++){
        VFS_fileTable[i].status = 1;
    }
}

void vfs_addFS(struct VFS_RootInterface *interface){
    if(interface == NULL) return;
    struct VFS_Inode *inode = &VFS.inodes[VFS.num_inodes++];
    inode->interface = interface;

    print_serial("[VFS] Letter: %c\n", inode->interface->vfsLetter == 0 ? inode->interface->drive->identity : inode->interface->vfsLetter);
    print_serial("[VFS] Get: 0x%x\n", inode->interface->fs_getLink);
    print_serial("[VFS] Read: 0x%x\n", inode->interface->fs_read);
    print_serial("[VFS] Write: 0x%x\n", inode->interface->fs_write);
    print_serial("[VFS] Seek: 0x%x\n", inode->interface->fs_seek);
    print_serial("[VFS] Creat: 0x%x\n", inode->interface->fs_creat);
    print_serial("[VFS] List: 0x%x\n", inode->interface->fs_listDirectory);
    print_serial("[VFS] Trunc: 0x%x\n", inode->interface->fs_truncate);

    inode->isRoot = 1;
    inode->root = inode;
    inode->isValid = 1;

    print_serial("[VFS] Add Root Inode For Drive %c format \"%s\"\n", inode->interface->vfsLetter == 0 ? inode->interface->drive->identity : inode->interface->vfsLetter, inode->interface->fs_label);
}

int vfs_allocFileD(){
    for(int i = 0; i < VFS_maxFiles; i++){
        if(VFS_fileTable[i].status == 1){
            VFS_fileTable[i].status = 0;
            return i;
        }
    }
    return -1;
}

void vfs_freeFileD(int fd){
    VFS_fileTable[fd].head = 0;
    VFS_fileTable[fd].status = 1;
}

struct VFS_Inode *vfs_findRoot(char driveLetter){
    for(int i = 0; i < VFS.num_inodes; i++){
        if(
            (VFS.inodes[i].interface->vfsLetter == 0 && VFS.inodes[i].interface->drive->identity == driveLetter) ||
            (VFS.inodes[i].interface->vfsLetter == driveLetter)
        ){
            return &VFS.inodes[i];
        }
    }
    return NULL;
}

struct VFS_Inode vfs_followLink(struct VFS_Inode *root, char *path){
    struct VFS_Inode result;
    result.isValid = 0;
    result.isRoot = 0;
    if(root == NULL || path == NULL) return result;
    result.root = root;
    result.type = result.root->type;
    result.interface = result.root->interface;
    void *queried = NULL;
    if(root->interface->fs_getLink != NULL)
        queried = root->interface->fs_getLink(root->interface->root, path, &result.meta);
    
    if(queried != NULL){
        result.isValid = 1;
        result.fs.fs = queried;
    }
    return result;
}

int vfs_openRel(struct DIRECTORY *dir, char *path, int flags){
    char big_path[200];
	memset(big_path, 0, sizeof(big_path));
	expandPath(big_path, sizeof(big_path), dir, path);
    return vfs_open(big_path, flags);
}

int vfs_open(char *path, int flags){
    print_serial("[VFS] Opening %s\n", path);
    if(path == NULL) return -1;
    char driveLetter = path[0];
    path += 2;
    struct VFS_Inode *root = vfs_findRoot(driveLetter);
    if(root->interface->drive != NULL){
        //print_serial("[VFS] Drive root: %c\n", root->interface->drive->identity);
    }
    else{
        //print_serial("[VFS] Drive root: %c\n", root->interface->vfsLetter);
    }
    if(root == NULL){
        //print_serial("[VFS] Unable to find root: \"%s\"\n", path);
        goto fail;
    }
    struct VFS_Inode inode = vfs_followLink(root, path);
    inode.flags = flags;
    if(inode.isValid == 0){
        /*
        if(inode.interface->drive != NULL){
            print_serial("[VFS] Unable to follow path \"%s\" from root %c\n", path, root->interface->drive->identity);
        }
        */
        goto fail;
    }
    int fd = vfs_allocFileD();
    if(fd == -1){
        //print_serial("[VFS] Unable to alloc file descriptor!\n");
        goto fail;
    }

    struct VFS_File *file_idx = &VFS_fileTable[fd];
    file_idx->status = 0;
    file_idx->inode = inode;
    file_idx->head = 0;
    return fd;

    fail:;
    print_serial("[VFS] Open %s Fail!\n", path);
    return -1;
}

void vfs_close(int fd){
    if(fd == -1) return;
    print_serial("[VFS] Closing FD %d\n", fd);
    struct VFS_File *file_idx = &VFS_fileTable[fd];
    if(file_idx->inode.type == VFS_PIPE){
        pipe_close(file_idx->inode.fs.pipe, file_idx->inode.flags);
    }
    else{
        vfs_freeFileD(fd);
        memset(&file_idx->inode, 0, sizeof(struct VFS_Inode));
    }
}

int vfs_read(int fd, void *buf, uint32_t nbytes){
    if(fd < 0 || fd > VFS_maxFiles) return -1;
    struct VFS_File *file_idx = &VFS_fileTable[fd];
    //print_serial("[VFS] Read %d bytes @ %d\n", nbytes, file_idx->head);
    if(!(file_idx->inode.flags & VFS_FLAG_READ)) return -1;

    if(file_idx->inode.interface->fs_read != NULL)
        return file_idx->inode.interface->fs_read(file_idx, buf, nbytes);
    return -1;
}

int vfs_write(int fd, void *buf, uint32_t nbytes){
    if(fd < 0 || fd > VFS_maxFiles) return -1;
    //print_serial("[VFS] Write %x %d\n", (char *) buf, nbytes);
    struct VFS_File *file_idx = &VFS_fileTable[fd];
    if(!(file_idx->inode.flags & VFS_FLAG_WRITE)) return -1;

    if(file_idx->inode.interface->fs_write != NULL)
        return file_idx->inode.interface->fs_write(file_idx, buf, nbytes);
    return -1;
}

int vfs_seek(int fd, int offset, int whence){
    if(fd < 0 || fd > VFS_maxFiles) return -1;
    struct VFS_File *file_idx = &VFS_fileTable[fd];

    if(file_idx->inode.root->interface->fs_seek)
        return file_idx->inode.root->interface->fs_seek(file_idx, offset, whence);
    return -1;
}

int vfs_creatRel(struct DIRECTORY *dir, char *path){
    char big_path[200];
	memset(big_path, 0, sizeof(big_path));
	expandPath(big_path, sizeof(big_path), dir, path);
    return vfs_creat(big_path);
}

int vfs_creat(char *path){
    print_serial("[VFS] Create file %s\n", path);

    if(path == NULL) return -1;
    char driveLetter = path[0];
    path += 2;
    struct VFS_Inode *root = vfs_findRoot(driveLetter);

    if(root->interface->fs_creat != NULL)
        return root->interface->fs_creat(root->interface->root, path, 1);
    return -1;
}

int vfs_creatDirRel(struct DIRECTORY *dir, char *path){
    char big_path[200];
	memset(big_path, 0, sizeof(big_path));
	expandPath(big_path, sizeof(big_path), dir, path);
    return vfs_creatdir(big_path);
}

int vfs_creatdir(char *path){
    print_serial("[VFS] Create directory %s\n", path);

    if(path == NULL) return -1;
    char driveLetter = path[0];
    path += 2;
    struct VFS_Inode *root = vfs_findRoot(driveLetter);

    if(root->interface->fs_creat != NULL)
        return root->interface->fs_creatDir(root->interface->root, path);
    return -1;
}

int vfs_mkpipe(int *writer_fd, int *reader_fd){
    if(writer_fd == NULL || reader_fd == NULL) return -1;
    *writer_fd = vfs_allocFileD();
    *reader_fd = vfs_allocFileD();
    if(*writer_fd == -1 || *reader_fd == -1) return -1;
    print_serial("[VFS] Making Pipe Write FD = %d, Read FD = %d\n", *writer_fd, *reader_fd);

    struct Pipe *pipe = pipe_create(1000);
    
    struct VFS_File *writer_file = &VFS_fileTable[*writer_fd];
    struct VFS_File *reader_file = &VFS_fileTable[*reader_fd];

    writer_file->inode.flags = VFS_FLAG_WRITE;
    writer_file->inode.type = VFS_PIPE;
    writer_file->inode.fs.pipe = pipe;
    writer_file->inode.root = NULL;
    writer_file->inode.interface = pipe_getInterface();
    writer_file->inode.isValid = 1;

    reader_file->inode.flags = VFS_FLAG_READ;
    reader_file->inode.type = VFS_PIPE;
    reader_file->inode.fs.pipe = pipe;
    reader_file->inode.root = NULL;
    reader_file->inode.interface = pipe_getInterface();
    reader_file->inode.isValid = 1;
    return 0;
}

struct DirectoryListing vfs_listDirectory(struct DIRECTORY *dir, char *path){
    struct DirectoryListing listing = {0};
    if(dir == NULL || path == NULL){
		print_serial("[VFS] List Directory: Err: Invalid Args\n");
		return listing;
	}
	char big_path[100];
	expandPath(big_path, sizeof(big_path), dir, path);
	char drive_letter = big_path[0];
	path = big_path + 2;
    struct VFS_Inode *root = vfs_findRoot(drive_letter);
    if(root == NULL) return listing;

    if(root->interface->fs_listDirectory != NULL)
        listing = root->interface->fs_listDirectory(root->interface->root, path);
    return listing;
}

#include "multitasking.h"
struct DirectoryListing vfs_taskListDirectory(char *path){
    return vfs_listDirectory(&tasks[task_running_idx].currentDirectory, path);
}

int vfs_chdir(struct DIRECTORY *dir, char *path){
	/*
	Manipulates the task currentDirectory string.
	Everything is appended to the string except for:
	./ is ignored, and removed.
	../ removed until the previous / and continues
	- Note, ensure that ../ to idx 0 is covered.
	*/
	int path_size = 0;
	//char workbuf[100];
	while(path[path_size] != 0){
		path_size++;
	}
	if(path[0] == '/'){//Absolute Directory
        char driveLetter = path[1];
        struct VFS_Inode *root = vfs_findRoot(driveLetter);
        if(root == NULL){
            return 1;
        }
        struct VFS_Inode inode = vfs_followLink(root, path + 3);
		if(inode.isValid == 1){
			memset(dir->path, 0 , sizeof(dir->path));
			memcpy(dir->path, path+1, path_size-1);
			if(dir->path[path_size-2] != '/') dir->path[path_size-1] = '/';
			return 0;
		}
		else{
			return 1;
		}
	}
	else{//Relative Directory
		if(path[0] == '.' && path[1] == '.' && path[2] == '\0'){
			int last_slash = 0;
			for(int i = 0; dir->path[i] != 0 && i < (int) sizeof(dir->path); i++) last_slash++;
			for(int i = last_slash-2; dir->path[i] != '/'; i--) last_slash = i;
			dir->path[last_slash] = '\0';
			return 0;
		}
	}
	return 1;
}

int vfs_ftruncate(int fd, unsigned int length){
    if(fd == -1 || fd >= VFS_maxFiles || VFS_fileTable[fd].status == 1) return -1;
    print_serial("[VFS] truncate %d\n", fd);
    struct VFS_File *file = &VFS_fileTable[fd];
    if(file->inode.interface->fs_truncate != NULL){
        return file->inode.interface->fs_truncate(file, length);
    }
    return -1;
}

int vfs_stat(int fd, void *statbuf){
    if(fd == -1 || fd >= VFS_maxFiles || VFS_fileTable[fd].status == 1) return -1;
    print_serial("[VFS] stat %d\n", fd);
    struct VFS_File *file = &VFS_fileTable[fd];
    if(file->inode.interface->fs_stat != NULL){
        return file->inode.interface->fs_stat(file, statbuf);
    }
    return -1;
}