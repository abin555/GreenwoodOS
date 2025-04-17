#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vp.h>
#include <sys/task.h>
#include <sys/memory.h>

#include "gif_lib.h"

struct Viewport *vp;
int running = 1;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

uint32_t make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

void vp_clear(struct Viewport *vp, uint32_t color){
	for(size_t i = 0; i < vp->loc.w * vp->loc.h; i++){
		vp->backbuf[i] = color;
	}
}

struct InternalFile {
	char *rawfile;
	int headidx;
};

struct InternalFile Ifile;

unsigned int nreads;
int readgif(GifFileType *gif_file, GifByteType *byte, int len){
	//printf("Reading %d bytes (#%d)\n", len, nreads++);
	struct InternalFile *Ifile = (struct InternalFile *) gif_file->UserData;
	memcpy(byte, Ifile->rawfile+Ifile->headidx, len);
	Ifile->headidx += len;
	return len;
}

uint32_t *viewbuf;
GifRowType *ScreenBuffer;

void GIF_CopyImageToViewport(GifFileType *gif, int image_idx){
	SavedImage *image = &gif->SavedImages[image_idx];
	GifImageDesc *desc = &image->ImageDesc;
	ColorMapObject *colorMap = desc->ColorMap ? desc->ColorMap : gif->SColorMap;
	GraphicsControlBlock gcb;
	int transparent_idx = -1;
	if(DGifSavedExtensionToGCB(gif, image_idx, &gcb) == GIF_OK){
		if(gcb.TransparentColor != NO_TRANSPARENT_COLOR){
			transparent_idx = gcb.TransparentColor;
		}
	}


	for(int y = 0; y < desc->Height; y++){
		for(int x = 0; x < desc->Width; x++){
			int c=  image->RasterBits[y * desc->Width + x];
			if(colorMap){
				GifColorType rgb;

				if(gcb.DisposalMode == DISPOSE_BACKGROUND){
					rgb = gif->SColorMap->Colors[gif->SBackGroundColor];
					viewbuf[(y+desc->Top)*gif->SWidth + (x+desc->Left)] = make_color(rgb.Red, rgb.Green, rgb.Blue, 0);
				}
				else if(gcb.DisposalMode == DISPOSE_PREVIOUS){
					viewbuf[(y+desc->Top)*gif->SWidth + (x+desc->Left)] = vp->frontbuf[(y+desc->Top)*gif->SWidth + (x+desc->Left)];
				}

				if(transparent_idx != -1 && c == transparent_idx){
					continue;
				}
				rgb = colorMap->Colors[c];
				uint32_t color = make_color(rgb.Red, rgb.Green, rgb.Blue, 0);
				viewbuf[(y+desc->Top)*gif->SWidth + (x+desc->Left)] = color;
			}
		}
	}

	for(int i = 0; i < 0xFFFFF; i++){
		for(int j = 0; j < 4*gcb.DelayTime; j++){

		}
	}
}

int main(int argc, char **argv){
	if(argc < 2){
		return 1;
	}
	nreads = 0;
	FILE *gif_file = fopen(argv[1], "r");
	if(gif_file == NULL) return 1;
	fseek(gif_file, 0, SEEK_END);
	int size = ftell(gif_file);
	fseek(gif_file, 0, SEEK_SET);
	Ifile.rawfile = memory_requestRegion(size);
	Ifile.headidx = 0;
	
	fread(Ifile.rawfile, size, 1, gif_file);
	fclose(gif_file);

	int gif_err = 0;
	GifFileType *gif = DGifOpen(&Ifile, readgif, &gif_err);
	task_lock(1);
	gif_err = DGifSlurp(gif);
	task_lock(0);
	memory_returnRegion(Ifile.rawfile, size);
	
	vp = vp_open(gif->SWidth, gif->SHeight, argv[1]);
	viewbuf = malloc(gif->SWidth * gif->SHeight * 4);
	vp_set_buffer(vp, viewbuf, gif->SWidth * gif->SHeight * 4);
	vp_add_event_handler(vp, event_handler);

	int image_idx = 0;
	running = 1;
	while(running){
		GIF_CopyImageToViewport(gif, image_idx);
		vp_copy(vp);

		image_idx++;
		if(image_idx >= gif->ImageCount){
			image_idx = 0;
		}

		yield();
	}
	DGifCloseFile(gif, &gif_err);
	vp_close(vp);
	printf("[GIF] Terminated\n");
	return 0;
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_EXIT){
        running = 0;
        set_schedule(ALWAYS);
    }
	else if(event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
	}
	else if(event == VP_MINIMIZE){
		set_schedule(NEVER);
	}
}