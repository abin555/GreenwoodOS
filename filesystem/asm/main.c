#include "libc.h"

char *input_filename;
char *output_filename;

struct FILE *input_file;
struct FILE *output_file;

void *work_region;

int main(int argc, char** argv){
    if(argc < 3){
        print("Not enough arguments, need [Input File] [Output File]!\n");
        return 1;
    }
    else if(argc > 3){
        print("Too many arguments!\n");
        return 1;
    }
    print_arg("Assembling File %s\n", (uint32_t) argv[1]);
    print_arg("To Binary File %s\n", (uint32_t) argv[2]);

    input_filename = argv[1];
    output_filename = argv[2];

    if(!fexists(input_filename)){
        print_arg("Assembly File %s does not exist!\n", (uint32_t) input_filename);
        return 1;
    }
    if(!fexists(output_filename)){
        fmkfile(output_filename, 1000);
    }

    input_file = fopen(input_filename);
    output_file = fopen(output_filename);

    work_region = requestRegion(0x10000);
    fcopy(input_file, work_region, fsize(input_file));
    //print_arg("%s\n", work_region);
    char *file_buf = work_region;
    int x = 0;
    while(*file_buf){
        //drawChar(8*x++, 0, *file_buf++);
        print_arg("%c", *file_buf++);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}

