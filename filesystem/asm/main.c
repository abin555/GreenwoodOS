#include "libc.h"

char *input_filename;
char *output_filename;

FILE *input_file;
FILE *output_file;



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
        print_arg("Assembly File %s does not exists!\n", (uint32_t) input_filename);
        return 1;
    }
    if(!fexists(output_filename)){
        fmkfile(output_filename, 1000);
    }

    input_file = fopen(input_filename);
    output_file = fopen(output_filename);

    

    fclose(input_file);
    fclose(output_file):

    return 0;
}

