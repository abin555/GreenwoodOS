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
	int i, j, Row, Col, Width, Height, ExtCode, Count;
	GifRecordType RecordType;
	GifByteType *Extension;

	static const int InterlacedOffset[] = {
	    0, 4, 2, 1}; /* The way Interlaced image should. */
	static const int InterlacedJumps[] = {
	    8, 8, 4, 2}; /* be read - offsets and jumps... */

	do {
		if (DGifGetRecordType(gif, &RecordType) == GIF_ERROR) {
			print_serial("[GIF] Exit Error 1\n");
			exit(1);
		}
		switch (RecordType) {
		case IMAGE_DESC_RECORD_TYPE:
			print_serial("[GIF] Get Image Desc Record\n");
			if (DGifGetImageDesc(gif) == GIF_ERROR) {
				print_serial("[GIF] Exit Error 2\n");
				exit(1);
			}
			Row = gif->Image
			          .Top; /* Image Position relative to Screen. */
			Col = gif->Image.Left;
			Width = gif->Image.Width;
			Height = gif->Image.Height;
			if (gif->Image.Left + gif->Image.Width >
			        gif->SWidth ||
			    gif->Image.Top + gif->Image.Height >
			        gif->SHeight) {
				print_serial(
				        "Image is not confined to screen "
				        "dimension, aborted.\n");
				exit(1);
			}
			if (gif->Image.Interlace) {
				/* Need to perform 4 passes on the images: */
				for (Count = i = 0; i < 4; i++) {
					for (j = Row + InterlacedOffset[i];
					     j < Row + Height;
					     j += InterlacedJumps[i]) {
						if (DGifGetLine(
						        gif,
						        &ScreenBuffer[j][Col],
						        Width) == GIF_ERROR) {

							print_serial("[GIF] Exit Error 3\n");
							exit(1);
						}
					}
				}
			} else {
				for (i = 0; i < Height; i++) {
					if (DGifGetLine(
					        gif,
					        &ScreenBuffer[Row++][Col],
					        Width) == GIF_ERROR) {

						print_serial("[GIF] Exit Error 4\n");
						exit(1);
					}
				}
			}
			break;
		case EXTENSION_RECORD_TYPE:
			/* Skip any extension blocks in file: */
			if (DGifGetExtension(gif, &ExtCode, &Extension) ==
			    GIF_ERROR) {
				
				print_serial("[GIF] Exit Error 5\n");
				exit(1);
			}
			while (Extension != NULL) {
				if (DGifGetExtensionNext(gif, &Extension) ==
				    GIF_ERROR) {
					print_serial("[GIF] Exit Error 6\n");
					exit(1);
				}
			}
			break;
		case TERMINATE_RECORD_TYPE:
			break;
		default: /* Should be trapped by DGifGetRecordType. */
			break;
		}
	} while (RecordType != TERMINATE_RECORD_TYPE);

	ColorMapObject *ColorMap = (gif->Image.ColorMap ? gif->Image.ColorMap
	                                    : gif->SColorMap);
	if (ColorMap == NULL) {
		print_serial("Gif Image does not have a colormap\n");
		exit(1);
	}

	/* check that the background color isn't garbage (SF bug #87) */
	if (gif->SBackGroundColor < 0 ||
	    gif->SBackGroundColor >= ColorMap->ColorCount) {
		print_serial("Background color out of range for colormap\n");
		exit(1);
	}

	unsigned char *Buffer, *BufferP;

	GifRowType GifRow;
	GifColorType *ColorMapEntry;
	for(i = 0; i < gif->SHeight; i++){
		GifRow = ScreenBuffer[i];
		for(j = 0, BufferP = Buffer; j < gif->SWidth; j++){
			if (GifRow[j] >= ColorMap->ColorCount){
				continue;
			}
			ColorMapEntry = &ColorMap->Colors[GifRow[j]];
			viewbuf[i*gif->SWidth + j] = make_color(ColorMapEntry->Red, ColorMapEntry->Green, ColorMapEntry->Blue, 0);
		}
	}

	vp_funcs->copy(vp);
}

int main(int argc, char **argv){
	if(argc < 2){
		return 1;
	}


	vp_funcs = viewport_get_funcs();

	struct FILE *gif_file = fopen(argv[1]);
	if(gif_file == NULL) return 1;
	heap = requestRegion(REGION_SIZE);
	addEndCallback(end_callback);

	Ifile.rawfile = malloc(fsize(gif_file));
	Ifile.headidx = 0;
	fcopy(gif_file, Ifile.rawfile, fsize(gif_file));
	fclose(gif_file);

	int gif_err;
	GifFileType *gif = DGifOpen(&Ifile, readgif, &gif_err);
	if(gif_err != D_GIF_SUCCEEDED){
		print_serial("[GIF] Error\n");
		print_serial((char *) GifErrorString(gif_err));
		print_serial("\n");

		freeRegion(heap, REGION_SIZE);
		return 1;
	}
	gif_err = DGifSlurp(gif);
	
	vp = vp_funcs->open(gif->SWidth, gif->SHeight, argv[1]);
	viewbuf = malloc(gif->SWidth * gif->SHeight * 4);
	vp_funcs->set_buffer(vp, viewbuf, gif->SWidth * gif->SHeight * 4);
	vp_funcs->add_event_handler(vp, event_handler);

	int i, j, Size;

	if ((ScreenBuffer = (GifRowType *)malloc(gif->SHeight * sizeof(GifRowType))) == NULL) {
		print_serial("Failed to allocate memory required, aborted.");
		goto terminate;
	}

	Size = gif->SWidth * sizeof(GifPixelType); /* Size in bytes one row.*/
	if ((ScreenBuffer[0] = (GifRowType)malloc(Size)) == NULL) { /* First row. */
		print_serial("Failed to allocate memory required, aborted.");
		goto terminate;
	}

	for (i = 0; i < gif->SWidth;
	     i++) { /* Set its color to BackGround. */
		ScreenBuffer[0][i] = gif->SBackGroundColor;
	}
	for (i = 1; i < gif->SHeight; i++) {
		/* Allocate the other rows, and set their color to background
		 * too: */
		if ((ScreenBuffer[i] = (GifRowType)malloc(Size)) == NULL) {
			print_serial("Failed to allocate memory required, aborted.");
			goto terminate;
		}

		memcpy(ScreenBuffer[i], ScreenBuffer[0], Size);
	}

	GIF_CopyImageToViewport(gif, 0);

	running = 1;
	while(running){

	}
	DGifCloseFile(gif, &gif_err);
	terminate:;
	vp_funcs->close(vp);
	freeRegion(heap, REGION_SIZE);
	return 0;
}

void *malloc(int size){
	//print_serial("[GIF] Alloc!\n");
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