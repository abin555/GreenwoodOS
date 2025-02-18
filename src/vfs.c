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
        vfs_addRoot(drives[i]);
    }
    for(int i = 0; i < VFS_maxFiles; i++){
        VFS_fileTable[i].status = 1;
    }
}

void vfs_addRoot(struct DRIVE *drive){
    if(VFS.num_inodes == VFS_maxRootInodes) return;
    if(drive == NULL) return;

    char *label;

    struct VFS_Inode *inode = &VFS.inodes[VFS.num_inodes++];
    inode->isRoot = 1;
    inode->root = inode;
    inode->isValid = 1;
    
    switch(drive->format){
        case EXT2:
            inode->drive = drive;
            inode->type = VFS_EXT2;
            inode->fs.ext2 = NULL;
            label = "EXT2";
            break;
        case ISO9660:
            inode->drive = drive;
            inode->type = VFS_ISO9660;
            inode->fs.iso = NULL;
            label = "ISO9660";
            break;
        case VFS_SYS:
            inode->drive = drive;
            inode->type = VFS_SYS;
            label = "SYSFS";
            break;
        default:
            label = "UNKNOWN";
            return;
    }

    print_serial("[VFS] Add Root Inode For Drive %c format \"%s\"\n", drive->identity, label);
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
        if(VFS.inodes[i].drive->identity == driveLetter){
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
    result.drive = result.root->drive;
    print_serial("[VFS] Following link to %s\n", path);

    //char *walker_start = path;
    //char *walker_end = path;

    if(root->type == VFS_EXT2){
        uint32_t inodeIdx = ext2_get_inodeIdx_from_path(result.drive->format_info.ext2, path);
        if(inodeIdx == 0){
			print_console(kernel_console, "[VFS] Inode is Zero???\n");
			return result;
		}
        result.ext2_inode_idx = inodeIdx;
        result.fs.ext2 = malloc(sizeof(struct EXT2_Inode));
        *result.fs.ext2 = ext2_read_inode_data(result.drive->format_info.ext2, inodeIdx);
        result.isValid = 1;
        return result;
    }
    else if(root->type == VFS_ISO9660){
        struct File_Info *isoInfo = malloc(sizeof(struct File_Info));
        if(isoInfo == NULL) return result;
        *isoInfo = ISO9660_GetFile(result.drive->format_info.ISO, path);
        if(isoInfo->drive == NULL) return result;
        result.fs.iso = isoInfo;
        result.isValid = 1;
        return result;
    }
    else if(root->type == VFS_SYS){

    }
    return result;
}

int vfs_openRel(struct DIRECTORY *dir, char *path){
    char big_path[200];
	memset(big_path, 0, sizeof(big_path));
	expandPath(big_path, sizeof(big_path), dir, path);
    return vfs_open(big_path);
}

int vfs_open(char *path){
    print_serial("[VFS] Opening %s\n", path);
    if(path == NULL) return -1;
    char driveLetter = path[0];
    path += 2;
    struct VFS_Inode *root = vfs_findRoot(driveLetter);
    print_serial("[VFS] Drive root: %c\n", root->drive->identity);
    if(root == NULL){
        print_serial("[VFS] Unable to find root: \"%s\"\n", path);
        goto fail;
    }
    struct VFS_Inode inode = vfs_followLink(root, path);
    if(inode.isValid == 0){
        print_serial("[VFS] Unable to follow path \"%s\" from root %c\n", path, root->drive->identity);
        goto fail;
    }
    int fd = vfs_allocFileD();
    if(fd == -1){
        print_serial("[VFS] Unable to alloc file descriptor!\n");
        goto fail;
    }

    struct VFS_File *file_idx = &VFS_fileTable[fd];
    file_idx->status = 0;
    file_idx->inode = inode;
    file_idx->head = 0;
    return fd;

    fail:;
    return -1;
}

void vfs_close(int fd){
    if(fd == -1) return;
    struct VFS_File *file_idx = &VFS_fileTable[fd];
    file_idx->status = 1;
    file_idx->head = 0;
    memset(&file_idx->inode, 0, sizeof(struct VFS_Inode));
}

int vfs_readExt2(struct VFS_File *file, void *buf, uint32_t nbytes){
    struct EXT2_FS *ext2 = file->inode.drive->format_info.ext2;
    struct EXT2_Inode *inode = file->inode.fs.ext2;
    
    uint32_t block_idx = 0;
    uint32_t single_indirect_idx = 0;
    uint32_t double_indirect_idx = 0;
    uint32_t idx = 0;
    uint32_t block_head = 0;

    uint32_t block_offset = file->head / ext2->block_size;
    uint32_t pointers_per_block = ext2->block_size / 4;

    block_head = file->head % ext2->block_size;

    char *block;
    
    if(block_offset < 12){
        block_idx = block_offset;
        single_indirect_idx = 0;
        double_indirect_idx = 0;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
    }
    else if(12 <= block_offset && block_offset < 12 + pointers_per_block){
        block_idx = 12;
        single_indirect_idx = block_offset - 12;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        uint32_t *indirect_blocks = (uint32_t *) block;
        uint32_t indirect_block = indirect_blocks[single_indirect_idx++];
        block = ext2_read_block(ext2, indirect_block);
    }
    else if(block_offset >= 12 + pointers_per_block){
        block_idx = 13;
        uint32_t offset = block_offset - (11 + (ext2->block_size / 4));
        single_indirect_idx = offset % pointers_per_block - 1;
        double_indirect_idx = (offset / pointers_per_block) * (ext2->block_size / 4) % 255;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        uint32_t *double_indirect_blocks = (uint32_t *) block;
        uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
        
        block = ext2_read_block(ext2, double_indirect_block);
        uint32_t *indirect_blocks = (uint32_t *) block;
        uint32_t indirect_block = indirect_blocks[single_indirect_idx++];
        block = ext2_read_block(ext2, indirect_block);
    }

    while((uint32_t) block_head < ext2->block_size && idx < (uint32_t) nbytes){
        ((uint8_t *)buf)[idx] = ((uint8_t *)block)[block_head];
        file->head++;
        idx++;
        block_head++;

        if((uint32_t) block_head == ext2->block_size){
            //print_serial("[VFS] [EXT2] Update - Block No. %d Idx: %d\n", file->head / ext2->block_size, idx);
            //print_serial("[VFS] Block offset %d - %d\n", block_idx, inode->BlockPointers[block_idx]);
            if(block_idx == 12){
                handle_first_indirect:;
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                uint32_t *indirect_blocks = (uint32_t *) block;
                uint32_t indirect_block = indirect_blocks[single_indirect_idx];
                //print_serial("[VFS] Indirect Block is at %d, idx %d is %d\n", inode->BlockPointers[block_idx], single_indirect_idx, indirect_block);
                block = ext2_read_block(ext2, indirect_block);
                //block = ext2_read_block(ext2, inode->BlockPointers[0]);
                single_indirect_idx++;
                if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    block_idx++;
                    if(block_idx == 13){
                        single_indirect_idx = 0;
                    }
                }
                block_head = 0;
            }
            else if(block_idx == 13){
                handle_first_double_indirect:;
                //print_serial("[VFS] [EXT2] Double Indirect Block\n");
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                uint32_t *double_indirect_blocks = (uint32_t *) block;
                uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
                
                block = ext2_read_block(ext2, double_indirect_block);
                uint32_t *indirect_blocks = (uint32_t *) block;
                uint32_t indirect_block = indirect_blocks[single_indirect_idx];
                //print_serial("[VFS] Indirect Block is at %d-%d block %d, idx %d is %d\n", inode->BlockPointers[block_idx], double_indirect_idx, double_indirect_block, single_indirect_idx, indirect_block);
                block = ext2_read_block(ext2, indirect_block);
                //block = ext2_read_block(ext2, inode->BlockPointers[0]);
                single_indirect_idx++;
                if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    double_indirect_idx++;
                    single_indirect_idx = 0;
                }

                if(double_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    block_idx++;
                }

                block_head = 0;
            }
            else if(block_idx > 13){
                print_serial("[VFS] [EXT2] Fucky Wucky time on the block idx\n");
                break;
            }
            else{
                block_idx++;
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                if(block_idx == 12){
                    goto handle_first_indirect;
                }
                if(block_idx == 13){
                    single_indirect_idx = 0;
                    goto handle_first_double_indirect;
                }
            }

            block_head = 0;
        }
    }

    return idx;
}

int vfs_readISO9660(struct VFS_File *file, void *buf, int nbytes){
    char *iso_buf = (char *) ISO_read_sector(file->inode.fs.iso->drive, file->inode.fs.iso->drive->format_info.ISO->buf, file->inode.fs.iso->sector);
    if(iso_buf == NULL) return 1;
    int idx = 0;
    int sector_offset = 0;
    while(file->head < 512*4 && idx < nbytes){
        ((char *) buf)[idx] = iso_buf[file->head];
        file->head++;
        idx++;
        if(file->head == 512*4){
            sector_offset++;
            print_serial("[DRIVE] Sector Offset %d\n", sector_offset);
            iso_buf = (char *) ISO_read_sector(file->inode.fs.iso->drive, file->inode.fs.iso->drive->format_info.ISO->buf, file->inode.fs.iso->sector+sector_offset);
            file->head = 0;
        }
    }
    return idx;
}

int vfs_read(int fd, void *buf, uint32_t nbytes){
    if(fd < 0 || fd > VFS_maxFiles) return -1;

    struct VFS_File *file_idx = &VFS_fileTable[fd];
    switch(file_idx->inode.type){
        case VFS_ISO9660:
            return vfs_readISO9660(file_idx, buf, nbytes);
            break;
        case VFS_EXT2:
            return vfs_readExt2(file_idx, buf, nbytes);
            break;
        case VFS_SYS:
            break;
    }
    return -1;
}

int vfs_writeExt2(struct VFS_File *file, void *buf, int nbytes){
    print_serial("[VFS] Writing %d bytes\n", nbytes);
    struct EXT2_FS *ext2 = file->inode.drive->format_info.ext2;
    struct EXT2_Inode *inode = file->inode.fs.ext2;

    if((uint32_t)(file->head + nbytes) >= inode->lsbSize){
        ext2_extendFile(ext2, file->inode.ext2_inode_idx, file->head + nbytes - inode->lsbSize);
    }
    
    uint32_t block_idx = 0;
    uint32_t single_indirect_idx = 0;
    uint32_t double_indirect_idx = 0;
    uint32_t idx = 0;
    uint32_t block_head = 0;

    uint32_t block_offset = file->head / ext2->block_size;
    uint32_t pointers_per_block = ext2->block_size / 4;
    uint32_t last_real_block_idx = 0;

    block_head = file->head % ext2->block_size;

    char *block;
    
    if(block_offset < 12){
        block_idx = block_offset;
        single_indirect_idx = 0;
        double_indirect_idx = 0;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        last_real_block_idx = inode->BlockPointers[block_idx];
    }
    else if(12 <= block_offset && block_offset < 12 + pointers_per_block){
        block_idx = 12;
        single_indirect_idx = block_offset - 12;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        last_real_block_idx = inode->BlockPointers[block_idx];
        uint32_t *indirect_blocks = (uint32_t *) block;
        uint32_t indirect_block = indirect_blocks[single_indirect_idx++];
        block = ext2_read_block(ext2, indirect_block);
        last_real_block_idx = indirect_block;
    }
    else if(block_offset >= 12 + pointers_per_block){
        block_idx = 13;
        uint32_t offset = block_offset - (11 + (ext2->block_size / 4));
        single_indirect_idx = offset % pointers_per_block - 1;
        double_indirect_idx = (offset / pointers_per_block) * (ext2->block_size / 4) % 255;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        last_real_block_idx = inode->BlockPointers[block_idx];
        uint32_t *double_indirect_blocks = (uint32_t *) block;
        uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
        
        block = ext2_read_block(ext2, double_indirect_block);
        last_real_block_idx = double_indirect_block;
        uint32_t *indirect_blocks = (uint32_t *) block;
        uint32_t indirect_block = indirect_blocks[single_indirect_idx++];
        block = ext2_read_block(ext2, indirect_block);
        last_real_block_idx = indirect_block;
    }

    while((uint32_t) block_head < ext2->block_size && idx < (uint32_t) nbytes){
        ((uint8_t *)block)[block_head] = ((uint8_t *)buf)[idx];
        print_serial("%d,%d - %c @ %d\n", block_head, idx, block[block_head], last_real_block_idx);
        file->head++;
        idx++;
        block_head++;

        if((uint32_t) block_head == ext2->block_size){
            ext2_write_block(ext2, last_real_block_idx, block);
            //print_serial("[VFS] [EXT2] Update - Block No. %d Idx: %d\n", file->head / ext2->block_size, idx);
            //print_serial("[VFS] Block offset %d - %d\n", block_idx, inode->BlockPointers[block_idx]);
            if(block_idx == 12){
                handle_first_indirect:;
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                last_real_block_idx = inode->BlockPointers[block_idx];
                uint32_t *indirect_blocks = (uint32_t *) block;
                uint32_t indirect_block = indirect_blocks[single_indirect_idx];
                //print_serial("[VFS] Indirect Block is at %d, idx %d is %d\n", inode->BlockPointers[block_idx], single_indirect_idx, indirect_block);
                block = ext2_read_block(ext2, indirect_block);
                last_real_block_idx = indirect_block;
                //block = ext2_read_block(ext2, inode->BlockPointers[0]);
                single_indirect_idx++;
                if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    block_idx++;
                    if(block_idx == 13){
                        single_indirect_idx = 0;
                    }
                }
                block_head = 0;
            }
            else if(block_idx == 13){
                handle_first_double_indirect:;
                //print_serial("[VFS] [EXT2] Double Indirect Block\n");
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                last_real_block_idx = inode->BlockPointers[block_idx];
                uint32_t *double_indirect_blocks = (uint32_t *) block;
                uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
                
                block = ext2_read_block(ext2, double_indirect_block);
                last_real_block_idx = double_indirect_block;
                uint32_t *indirect_blocks = (uint32_t *) block;
                uint32_t indirect_block = indirect_blocks[single_indirect_idx];
                //print_serial("[VFS] Indirect Block is at %d-%d block %d, idx %d is %d\n", inode->BlockPointers[block_idx], double_indirect_idx, double_indirect_block, single_indirect_idx, indirect_block);
                block = ext2_read_block(ext2, indirect_block);
                last_real_block_idx = indirect_block;
                //block = ext2_read_block(ext2, inode->BlockPointers[0]);
                single_indirect_idx++;
                if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    double_indirect_idx++;
                    single_indirect_idx = 0;
                }

                if(double_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    block_idx++;
                }

                block_head = 0;
            }
            else if(block_idx > 13){
                print_serial("[VFS] [EXT2] Fucky Wucky time on the block idx\n");
                break;
            }
            else{
                block_idx++;
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                last_real_block_idx = inode->BlockPointers[block_idx];
                if(block_idx == 12){
                    goto handle_first_indirect;
                }
                if(block_idx == 13){
                    single_indirect_idx = 0;
                    goto handle_first_double_indirect;
                }
            }

            block_head = 0;
        }
    }
    ext2_write_block(ext2, last_real_block_idx, block);

    return idx;
}

int vfs_write(int fd, void *buf, uint32_t nbytes){
    if(fd < 0 || fd > VFS_maxFiles) return -1;

    struct VFS_File *file_idx = &VFS_fileTable[fd];
    switch(file_idx->inode.type){
        case VFS_ISO9660:
            break;
        case VFS_EXT2:
            return vfs_writeExt2(file_idx, buf, nbytes);
            break;
        case VFS_SYS:
            break;
    }
    return -1;
}

int vfs_seek(int fd, int offset, int whence){
    if(fd < 0 || fd > VFS_maxFiles) return -1;
    struct VFS_File *file_idx = &VFS_fileTable[fd];

    if(whence == 0){//SEEK_SET
        file_idx->head = offset;
    }
    else if(whence == 1){//SEEK_CUR
        file_idx->head += offset;
    }
    else if(whence == 2){//SEEK_END
        if(file_idx->inode.type == VFS_EXT2){
            file_idx->head += file_idx->inode.fs.ext2->lsbSize + offset;
        }
        else if(file_idx->inode.type == VFS_ISO9660){
            file_idx->head += file_idx->inode.fs.iso->size + offset;
        }
        else if(file_idx->inode.type == VFS_SYS){

        }
    }
    else{
        return -1;
    }

    return file_idx->head;
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
    switch(root->type){
        case VFS_EXT2:
            if(ext2_createFile(root->drive->format_info.ext2, path, 1)){
                return -1;
            }
            return 0;
            break;
        default:
            return -1;
            break;
    }
}