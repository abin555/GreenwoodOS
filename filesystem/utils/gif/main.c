#include "libc.h"
#include "utils.h"

#include "gif_lib.h"
#define REGION_SIZE (0x400000 * 12)

char *heap;
int heap_idx = 0;

struct ViewportFunctions *vp_funcs;
struct Viewport *vp;
int running = 1;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

void end_callback(){
	freeRegion(heap, REGION_SIZE);
	print_serial("[GIF] Freed requested kernel region!\n");
}

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


int readgif(GifFileType *gif_file, GifByteType *byte, int len){
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


	vp_funcs = viewport_get_funcs();

	int gif_file = open(argv[1]);
	if(gif_file == -1) return 1;
	heap = requestRegion(REGION_SIZE);
	memset(heap, 0, REGION_SIZE);
	addEndCallback(end_callback);

	int size = lseek(gif_file, 0, 2);
    lseek(gif_file, 0, 0);
	Ifile.rawfile = malloc(size);
	Ifile.headidx = 0;
	read(gif_file, Ifile.rawfile, size);
	close(gif_file);

	int gif_err;
	GifFileType *gif = DGifOpen(&Ifile, readgif, &gif_err);
	gif_err = DGifSlurp(gif);
	
	vp = vp_funcs->open(gif->SWidth, gif->SHeight, argv[1]);
	viewbuf = malloc(gif->SWidth * gif->SHeight * 4);
	vp_funcs->set_buffer(vp, viewbuf, gif->SWidth * gif->SHeight * 4);
	vp_funcs->add_event_handler(vp, event_handler);


	int image_idx = 0;
	running = 1;
	while(running){
		GIF_CopyImageToViewport(gif, image_idx);
		vp_funcs->copy(vp);

		image_idx++;
		if(image_idx >= gif->ImageCount){
			image_idx = 0;
		}
		//task_lock(0);
		yield();
	}
	DGifCloseFile(gif, &gif_err);
	terminate:;
	vp_funcs->close(vp);
	freeRegion(heap, REGION_SIZE);
	print_serial("[GIF] Terminated\n");
	return 0;
}

void *malloc(int size){
	//print_serial("[GIF] Alloc!\n");
	if(heap_idx > REGION_SIZE){
		return NULL;
	}
	void *address = (void *) &heap[heap_idx];
	heap_idx += size;
	return address;
}

void *calloc(size_t nmemb, size_t size){
	return malloc(nmemb * size);
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

void free(void *mem){
	return;
}