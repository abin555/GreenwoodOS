#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <sys/vp.h>
#include <sys/task.h>
#include <sys/mouse.h>

#define WIDTH 500
#define HEIGHT 400

int running;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_EXIT){
        set_schedule(ALWAYS);
        running = 0;
    }
    else if(event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE){
        set_schedule(NEVER);
    }
}

double normalize_int(int min, int max, int v, double a, double b){
    return (((b - a) * (double) (v - min)) / (double) (max - min)) + a;
}

uint32_t createRGB(uint8_t r, uint8_t g, uint8_t b){
    uint32_t color = r << 8*2 | g << 8 | b;
    return color;
}

typedef struct {
    double x;
    double y;
} Vec2;

void renderMandelbrot(uint32_t *palette, uint32_t palette_size, uint32_t *drawbuf, uint32_t width, uint32_t height, Vec2 xB, Vec2 yB){
    for(int Px = 0; Px < width; Px++){
        for(int Py = 0; Py < height; Py++){
            double x0, y0;
            x0 = normalize_int(0, width, Px, xB.x, xB.y);
            y0 = normalize_int(0, height, Py, yB.x, yB.y);
            double x, y;
            x = 0.0f;
            y = 0.0f;
            int iteration = 0;
            while(x*x + y*y <= 2*2 && iteration < 1000){
                double xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                iteration++;
            }
            uint32_t color = palette[iteration % palette_size];
            drawbuf[Px + Py*width] = color;
        }
    }
}

int main(int argc, char **argv){

    struct Viewport *vp = vp_open(WIDTH, HEIGHT, "Mandelbrot");
    if(vp == NULL) return 1;
    vp_add_event_handler(vp, event_handler);

    uint32_t *drawbuf = malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
    if(drawbuf == NULL) return 1;

    int palette_size = 16;
    uint32_t *palette = malloc(sizeof(uint32_t) * palette_size);
    palette[0] = createRGB(66, 30, 15);
    palette[1] = createRGB(25, 7, 26);
    palette[2] = createRGB(9, 1, 47);
    palette[3] = createRGB(4, 4, 73);
    palette[4] = createRGB(0, 7, 100);
    palette[5] = createRGB(12, 44, 138);
    palette[6] = createRGB(24, 82, 177);
    palette[7] = createRGB(57, 125, 209);
    palette[8] = createRGB(134, 181, 229);
    palette[9] = createRGB(211, 236, 248);
    palette[10] = createRGB(241, 233, 191);
    palette[11] = createRGB(248, 201, 95);
    palette[12] = createRGB(255, 170, 0);
    palette[13] = createRGB(204, 128, 0);
    palette[14] = createRGB(153, 87, 0);
    palette[15] = createRGB(106, 52, 3);

    vp_set_buffer(vp, drawbuf, sizeof(uint32_t) * WIDTH * HEIGHT);

    FILE *mouseFile = fopen("/-/sys/mouse", "r");
    if(mouseFile == NULL){
        printf("Can't access mouse!\n");
        vp_close(vp);
        return 1;
    }

    struct MouseStatus mStatus;
    struct MouseStatus mStatusPrev;

    Vec2 mbrotX = {-2.5f, 1.0f};
    Vec2 mbrotY = {-1.0f, 1.0f};

    renderMandelbrot(
        palette,
        palette_size,
        drawbuf,
        WIDTH,
        HEIGHT,
        mbrotX,
        mbrotY
    );
    vp_copy(vp);


    running = 1;
    fread(&mStatusPrev, sizeof(mStatusPrev), 1, mouseFile);
    fseek(mouseFile, 0, SEEK_SET);

    while(running){
        fread(&mStatus, sizeof(mStatus), 1, mouseFile);
        fseek(mouseFile, 0, SEEK_SET);


        int relMouseX, relMouseY;
        relMouseX = mStatus.pos.x - vp->loc.x;
        relMouseY = mStatus.pos.y - vp->loc.y;

        int redraw = 0;

        if(!mStatus.buttons.left && mStatusPrev.buttons.left && 
            relMouseX > 0 && relMouseX < vp->loc.w &&
            relMouseY > 0 && relMouseY < vp->loc.h
        ){
            double normalizedMouseX = normalize_int(
                0, vp->loc.w,
                relMouseX,
                mbrotX.x,
                mbrotX.y
            );
            double normalizedMouseY = normalize_int(
                0, vp->loc.h,
                relMouseY,
                mbrotY.x,
                mbrotY.y
            );

            mbrotX.x = normalizedMouseX - (normalizedMouseX - mbrotX.x) * 0.75f;
            mbrotX.y = normalizedMouseX - (normalizedMouseX - mbrotX.y) * 0.75f;
            mbrotY.x = normalizedMouseY - (normalizedMouseY - mbrotY.x) * 0.75f;
            mbrotY.y = normalizedMouseY - (normalizedMouseY - mbrotY.y) * 0.75f;
            redraw = 1;
        }
        if(
            !mStatus.buttons.right && mStatusPrev.buttons.right && 
            relMouseX > 0 && relMouseX < vp->loc.w &&
            relMouseY > 0 && relMouseY < vp->loc.h
        ){
            double normalizedMouseX = normalize_int(
                0, vp->loc.w,
                relMouseX,
                mbrotX.x,
                mbrotX.y
            );
            double normalizedMouseY = normalize_int(
                0, vp->loc.h,
                relMouseY,
                mbrotY.x,
                mbrotY.y
            );

            mbrotX.x = normalizedMouseX - (normalizedMouseX - mbrotX.x) * 1.25f;
            mbrotX.y = normalizedMouseX - (normalizedMouseX - mbrotX.y) * 1.25f;
            mbrotY.x = normalizedMouseY - (normalizedMouseY - mbrotY.x) * 1.25f;
            mbrotY.y = normalizedMouseY - (normalizedMouseY - mbrotY.y) * 1.25f;
            redraw = 1;
        }
        if(
            vp->ascii == 'R'
        ){
            vp->ascii = '\0';
            mbrotX = (Vec2) {-2.5f, 1.0f};
            mbrotY = (Vec2) {-1.0f, 1.0f};
            redraw = 1;
        }

        if(redraw){
            renderMandelbrot(
                palette,
                palette_size,
                drawbuf,
                WIDTH,
                HEIGHT,
                mbrotX,
                mbrotY
            );
            vp_copy(vp);
        }

        mStatusPrev = mStatus;
    }

    vp_close(vp);    
    return 1;
}