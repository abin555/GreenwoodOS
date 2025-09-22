#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/io.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/vp.h>
#include <internal/stdio.h>

typedef struct {
  unsigned char magic1;             // must be zero
  unsigned char colormap;           // must be zero
  unsigned char encoding;           // must be 2
  unsigned short cmaporig, cmaplen; // must be zero
  unsigned char cmapent;            // must be zero
  unsigned short x;                 // must be zero
  unsigned short y;                 // image's height
  unsigned short w;                 // image's height
  unsigned short h;                 // image's width
  unsigned char bpp;                // must be 32
  unsigned char pixeltype;          // must be 40
} __attribute__((packed)) tga_header_t;

int main(int argc, char **argv){
    if(argc != 5){
        printf("Usage: resize [in file] [outfile] [outsize x] [outsize y]\n");
        return 1;
    }
    char *infile_name = argv[1];
    char *outfile_name = argv[2];
    char *osizex_str = argv[3];
    char *osizey_str = argv[4];
    int osizex = atoi(osizex_str);
    int osizey = atoi(osizey_str);

    tga_header_t infile_header;
    tga_header_t outfile_header;

    FILE *infile, *outfile;
    infile = fopen(infile_name, "r");
    if(infile == NULL){
        printf("Error, \"%s\" does not exist!\n", infile_name);
        return 1;
    }

    outfile = fopen(outfile_name, "w");
    if(outfile == NULL){
        printf("Creating output \"%s\"\n", outfile_name);
        if(creat(outfile_name) == -1){
            printf("Error creating file!\n");
            fclose(infile);
            return 1;
        }
        outfile = fopen(outfile_name, "w");
        if(outfile == NULL){
            printf("output creation failed. aborting\n");
            fclose(infile);
            return 1;
        }
    }

    fread(&infile_header, sizeof(infile_header), 1, infile);
    printf("Input file is %dx%d\nWanting to resize to %dx%d\n", infile_header.w, infile_header.h, osizex, osizey);
    outfile_header = infile_header;
    outfile_header.y = osizey;
    outfile_header.w = osizex;
    outfile_header.h = osizey;


    float x_scale, y_scale;
    x_scale = ((float) infile_header.w / (float) outfile_header.w);
    y_scale = ((float) infile_header.h / (float) outfile_header.h);

    uint32_t *in_image = memory_requestRegion(sizeof(uint32_t) * infile_header.w * infile_header.h);
    fread(in_image, sizeof(uint32_t) * infile_header.w * infile_header.h, 1, infile);

    uint32_t *new_image = memory_requestRegion(sizeof(uint32_t) * outfile_header.w * outfile_header.h);

    for(int y = 0; y < outfile_header.h; y++){
        for(int x = 0; x < outfile_header.w; x++){
            uint32_t color;
            int src_x = (int) ((float) x * x_scale);
            int src_y = (int) ((float) y * y_scale);

            new_image[x + (y * outfile_header.w)] = in_image[src_x + (src_y * infile_header.w)];
        }
    }

    //struct Viewport *vp = vp_open(outfile_header.w, outfile_header.h, "Resize");
    //vp_set_buffer(vp, new_image, sizeof(uint32_t) * outfile_header.w * outfile_header.h);
    //vp_copy(vp);

    ftruncate(outfile->fd, sizeof(outfile_header) + sizeof(uint32_t) *outfile_header.w * outfile_header.h);

    fwrite(&outfile_header, sizeof(outfile_header), 1, outfile);
    fwrite(new_image, sizeof(uint32_t) *outfile_header.w * outfile_header.h, 1, outfile);

    memory_returnRegion(in_image, sizeof(uint32_t) * infile_header.w * infile_header.h);
    memory_returnRegion(new_image, sizeof(uint32_t) * outfile_header.w * outfile_header.h);

    fclose(infile);
    fclose(outfile);

    return 0;
}