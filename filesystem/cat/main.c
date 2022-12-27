#include "../libc/libc.h"
void load_text(FILE* file, unsigned char *target_buffer);

int main(){
    register char* arg asm("eax");
    int start_index = 0;
    while(*(arg+start_index) != ' '){
        start_index++;
    }
    char* file = (arg+start_index+1);
    printval("%s\n", (int) file);
    FILE* open_file = fopen(file);
    char *file_buf = malloc(open_file->size);
    load_text(open_file, file_buf);
    set_backbuffer(1);
    print("File Contents:\n");
    for(int i = 0; i < (open_file ->size / 0xA0) + 1; i++){
        print(file_buf+(i*0xA0));
        swap_buffers();
    }
    set_backbuffer(0);
    print("\n");
    free(file_buf);
}

void load_text(FILE* file, unsigned char *target_buffer){
    unsigned char *file_buf;
    for(unsigned int sector = 0; sector < file->sector_count; sector++){
        file_buf = fread(file->drive, file->sector + sector);
        for(unsigned int i = 0; i < 0x800; i++){
            if(sector * 0x800 + i > file->size){
                break;
            }
            target_buffer[sector*0x800 + i] = file_buf[i];
        }
    }
}