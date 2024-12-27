#include "libc.h"
#include "gifdec.h"

char *heap;
int heap_idx = 0;
void *alloc(int size);
void *calloc(size_t nmemb, size_t size);

struct ViewportFunctions *vp_funcs;
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


int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = s1;
    const unsigned char *p2 = s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i]; 
        }
    }
    return 0; 
}


void *memset (void *dest, register int val, register size_t len)
{
  register unsigned char *ptr = (unsigned char*)dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}

void *memcpy(void *dest, const void *src, int n){
  for(int i = 0; i < n; i++){
    ((uint8_t *) dest)[i] = ((uint8_t *) src)[i];
  }
  return dest;
}

void *realloc(void *ptr, size_t size){
	void *new_alloc = alloc(size);
	memcpy(new_alloc, ptr, size);
	return new_alloc;
}


int strncmp(const char *s1, const char *s2, register size_t n)
{
  register unsigned char u1, u2;

  while (n-- > 0)
    {
      u1 = (unsigned char) *s1++;
      u2 = (unsigned char) *s2++;
      if (u1 != u2)
	return u1 - u2;
      if (u1 == '\0')
	return 0;
    }
  return 0;
}

int main(int argc, char **argv){
	if(argc < 2){
		return 1;
	}
	vp_funcs = viewport_get_funcs();

	gd_GIF *gif;

	gif = gd_open_gif(argv[1]);
	if(gif == NULL){
		print_serial("[GIF] Unable to open GIF!\n");
		return 1;
	}

	uint8_t *frame = alloc(gif->width * gif->height * 3);
	uint32_t *image_buf = alloc(gif->width * gif->height * 4);
	
	vp = vp_funcs->open(gif->width, gif->height, argv[1]);
	vp_funcs->set_buffer(vp, image_buf, gif->width * gif->height * 4);
	vp_funcs->add_event_handler(vp, event_handler);

	uint8_t *color = &gif->gct.colors[gif->bgindex * 3];
	uint32_t background_color = make_color(color[0], color[1], color[2], 0x00);
	vp_clear(vp, background_color);
	vp_funcs->copy(vp);

	int ret;
	uint32_t pixel;
	void *addr;

	int i, j;

	while(running){
		ret = gd_get_frame(gif);
		if(ret == -1) break;

		gd_render_frame(gif, frame);
		color = frame;

		for (i = 0; i < gif->height; i++) {
            for (j = 0; j < gif->width; j++) {
                if (!gd_is_bgcolor(gif, color))
                    pixel = make_color(color[0], color[1], color[2], 0x00);
                else if (((i >> 2) + (j >> 2)) & 1)
                    pixel = make_color(0x7F, 0x7F, 0x7F, 0x00);
                else
                    pixel = make_color(0x00, 0x00, 0x00, 0x00);
                addr = image_buf + (i * gif->width + j * sizeof(pixel));
                memcpy(addr, &pixel, sizeof(pixel));
                color += 3;
            }
        }

		vp_funcs->copy(vp);

		if(ret == 0){
			gd_rewind(gif);
		}
	}
	gd_close_gif(gif);
	vp_funcs->close(vp);

}

void *alloc(int size){
	print_serial("[GIF] Alloc!\n");
	void *address = (void *) &heap[heap_idx];
	heap_idx += size;
	return address;
}

void *calloc(size_t nmemb, size_t size){
	return alloc(nmemb * size);
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
