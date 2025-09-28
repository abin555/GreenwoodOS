#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <sys/vp.h>
#include <sys/task.h>
#include <string.h>
#include <sys/mouse.h>

unsigned int HEIGHT;
unsigned int WIDTH;
uint32_t *drawbuf;

typedef struct {
    double x;
    double y;
    double z;
} Vector3;

typedef struct {
    float m[4][4];
  } Matrix4x4;

struct Hyperboloid {
    int num_points;
    Vector3 **points;
    double radius;
    double angle;
    double rotX;
    double rotY;
    double rotZ;
};

struct RenderContext {
    Vector3 camera;
    Vector3 camera_rot;

    Matrix4x4 ProjectionMatrix;
    Matrix4x4 MatRotZ;
    Matrix4x4 MatRotX;
    Matrix4x4 MatRotY;

    double fNear;
    double fFar;
    double fFov;
    double fAspectRatio;
    double fFovRad;
};

struct RenderContext *init_renderContext(){
    struct RenderContext *context = malloc(sizeof(struct RenderContext));
    if(context == NULL) return NULL;

    context->camera = (Vector3) {0.0f, 0.0f, -3.0f};
    context->camera_rot = (Vector3) {0.0f, 0.0f, 0.0f};

    context->fNear = 0.1f;
    context->fFar = 10.0f;
    context->fFov = 90.0f;
    context->fAspectRatio = (double) HEIGHT / (double) WIDTH;
    context->fFovRad = 1.0f / (sin(context->fFov * 0.5f / 180.0f * M_PI) / cos(context->fFov * 0.5f / 180.0f * M_PI));

    context->ProjectionMatrix.m[0][0] = context->fAspectRatio * context->fFovRad;
    context->ProjectionMatrix.m[1][1] = context->fFovRad;
    context->ProjectionMatrix.m[2][2] = context->fFar / (context->fFar - context->fNear);
    context->ProjectionMatrix.m[3][3] = (-context->fFar * context->fNear) / (context->fFar - context->fNear);
    context->ProjectionMatrix.m[2][3] = 1.0f;
    context->ProjectionMatrix.m[3][3] = 0.0f;

    context->MatRotX = (Matrix4x4) {0};
    context->MatRotY = (Matrix4x4) {0};
    context->MatRotZ = (Matrix4x4) {0};

    return context;
}

void update_renderContext(struct RenderContext *ctx){
    ctx->MatRotX.m[0][0] = 1;
    ctx->MatRotX.m[1][1] =  cos(ctx->camera_rot.x);
    ctx->MatRotX.m[1][2] =  sin(ctx->camera_rot.x);
    ctx->MatRotX.m[2][1] = -sin(ctx->camera_rot.x);
    ctx->MatRotX.m[2][2] =  cos(ctx->camera_rot.x);
    ctx->MatRotX.m[3][3] = 1;

    ctx->MatRotY.m[0][0] =  cos(ctx->camera_rot.y);
    ctx->MatRotY.m[2][0] =  sin(ctx->camera_rot.y);
    ctx->MatRotY.m[1][1] = 1;
    ctx->MatRotY.m[0][2] = -sin(ctx->camera_rot.y);
    ctx->MatRotY.m[2][2] =  cos(ctx->camera_rot.y);

    ctx->MatRotZ.m[0][0] =  cos(ctx->camera_rot.z);
    ctx->MatRotZ.m[0][1] =  sin(ctx->camera_rot.z);
    ctx->MatRotZ.m[1][0] = -sin(ctx->camera_rot.z);
    ctx->MatRotZ.m[1][1] =  cos(ctx->camera_rot.z);
    ctx->MatRotZ.m[2][2] = 1;
    ctx->MatRotZ.m[3][3] = 1;
}

struct Hyperboloid *init_hyperboloid(int points, double radius){
    struct Hyperboloid *h = malloc(sizeof(struct Hyperboloid));
    if(h == NULL) return h;
    h->num_points = points;
    h->points = malloc(sizeof(Vector3 *) * 2);
    for(int i = 0; i < 2; i++){
        h->points[i] = malloc(sizeof(Vector3) * h->num_points);
    }
    h->radius = radius;
    h->angle = 0.0f;
    h->rotX = 0.0f;
    h->rotY = 0.0f;
    h->rotZ = 0.0f;

    double rotation_step_size = (M_PI * 2.0f) / h->num_points;

    for(int i = 0; i < h->num_points; i++){
        double x, z;
        x = h->radius*sin(i*rotation_step_size);
        z = h->radius*cos(i*rotation_step_size);
        h->points[0][i] = (Vector3) {x, h->radius, z};
        h->points[1][i] = (Vector3) {x, -1.0f * h->radius, z};
    }

    return h;
}

void drawLine(uint32_t *buf, size_t buf_width, int x1, int y1, int x2, int y2, uint32_t color){
    if(!(x1 >= 0 && x1 <= WIDTH && y1 >= 0 && y1 <= HEIGHT)){
      return;
    }
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    int e2;
  
    for(;;){
      if(x1 >= 0 && x1 <= WIDTH && y1 >= 0 && y1 <= HEIGHT){
        buf[x1 + y1 * buf_width] = color;
      }    
      if(x1 == x2 && y1 == y2) break;
      e2 = 2 * err;
      if(e2 >= dy){err += dy; x1 += sx;}
      if(e2 <= dx){err += dx; y1 += sy;}
    }
}

void multiplyMatrixVector(Vector3 *vi, Vector3 *vo, Matrix4x4 *m){
    vo->x = vi->x * m->m[0][0] + vi->y * m->m[1][0] + vi->z * m->m[2][0] + m->m[3][0];
    vo->y = vi->x * m->m[0][1] + vi->y * m->m[1][1] + vi->z * m->m[2][1] + m->m[3][1];
    vo->z = vi->x * m->m[0][2] + vi->y * m->m[1][2] + vi->z * m->m[2][2] + m->m[3][2];
    float w = vi->x * m->m[0][3] + vi->y * m->m[1][3] + vi->z * m->m[2][3] + m->m[3][3];
    if(w != 0.0f){
        vo->x /= w; vo->y /= w; vo->z /= w;
    }
}

void rotateVec3X(Vector3 *v, double theta){
    Matrix4x4 rMat = {0};
    rMat.m[0][0] = 1;
    rMat.m[1][1] = cos(theta);
    rMat.m[1][2] = sin(theta);
    rMat.m[2][1] = -sin(theta);
    rMat.m[2][2] = cos(theta);
    rMat.m[3][3] = 1;
    Vector3 temp = *v;
    multiplyMatrixVector(&temp, v, &rMat);
}

void rotateVec3Y(Vector3 *v, double theta){
    Matrix4x4 rMat = {0};
    rMat.m[0][0] = cos(theta);
    rMat.m[2][0] = sin(theta);
    rMat.m[1][1] = 1;
    rMat.m[0][2] = -sin(theta);
    rMat.m[2][2] = cos(theta);
    Vector3 temp = *v;
    multiplyMatrixVector(&temp, v, &rMat);
}

void rotateVec3Z(Vector3 *v, double theta){
    Matrix4x4 rMat = {0};
    rMat.m[0][0] = cos(theta);
    rMat.m[0][1] = sin(theta);
    rMat.m[1][0] = -sin(theta);
    rMat.m[1][1] = cos(theta);
    rMat.m[2][2] = 1;
    rMat.m[3][3] = 1;
    Vector3 temp = *v;
    multiplyMatrixVector(&temp, v, &rMat);
}

int running;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    switch(event){
        case VP_EXIT:
            running = 0;
            set_schedule(ALWAYS);
            break;
        case VP_MAXIMIZE:
            set_schedule(ALWAYS);
            break;
        case VP_MINIMIZE:
            set_schedule(NEVER);
            break;
        case VP_RESIZE:
            WIDTH = vp->resizeLoc.w;
            HEIGHT = vp->resizeLoc.h;
            vp_set_buffer(vp, drawbuf, WIDTH * HEIGHT * sizeof(uint32_t));
            vp->loc.w = WIDTH;
            vp->loc.h = HEIGHT;
            break;
        default:
            return;
    }
}

void doRender(struct Hyperboloid *h, struct RenderContext *ctx, struct Viewport *vp, uint32_t *drawbuf){
    memset(drawbuf, 0, (sizeof(uint32_t) * HEIGHT * WIDTH));
    update_renderContext(ctx);
    for(int i = 0; i < h->num_points; i++){
        h->points[1][i] = h->points[0][i];
        h->points[1][i].y *= -1.0f;
        rotateVec3Y(&h->points[1][i], h->angle);
    }

    for(int i = 0; i < h->num_points; i++){
        Vector3 v_w0, v_w1;
        Vector3 v_t0, v_t1;
        v_w0 = h->points[0][i];
        v_w1 = h->points[1][i];

        rotateVec3Y(&v_w0, h->rotY);
        rotateVec3Y(&v_w1, h->rotY);
        rotateVec3Z(&v_w0, h->rotZ);
        rotateVec3Z(&v_w1, h->rotZ);
        rotateVec3X(&v_w0, h->rotX);
        rotateVec3X(&v_w1, h->rotX);
        
        v_w0.z += ctx->camera.z;
        v_w1.z += ctx->camera.z;
        v_w0.x += ctx->camera.x;
        v_w1.x += ctx->camera.x;
        v_w0.y += ctx->camera.y;
        v_w1.y += ctx->camera.y;

        multiplyMatrixVector(&v_w0, &v_t0, &ctx->MatRotY);
        multiplyMatrixVector(&v_w1, &v_t1, &ctx->MatRotY);
        multiplyMatrixVector(&v_t0, &v_w0, &ctx->MatRotX);
        multiplyMatrixVector(&v_t1, &v_w1, &ctx->MatRotX);

        Vector3 v_f0, v_f1;
        multiplyMatrixVector(&v_w0, &v_f0, &ctx->ProjectionMatrix);
        multiplyMatrixVector(&v_w1, &v_f1, &ctx->ProjectionMatrix);
        v_f0.x += 1.0f; v_f0.y += 1.0f;
        v_f1.x += 1.0f; v_f1.y += 1.0f;
        v_f0.x *= 0.5f * (float) WIDTH;
        v_f0.y *= 0.5f * (float) HEIGHT;
        v_f1.x *= 0.5f * (float) WIDTH;
        v_f1.y *= 0.5f * (float) HEIGHT;

        drawLine(
            drawbuf, WIDTH,
            v_f0.x,
            v_f0.y,
            v_f1.x,
            v_f1.y,
            0xFFFFFF
        );
    }
    h->angle += 0.01f;
    if(h->angle > 6.1f){
        h->angle = 0.0f;
    }
}

struct MouseStatus lastMouse = {0};
void handleMouseInteraction(FILE *mouseFile, struct Hyperboloid *h, struct RenderContext *ctx, struct Viewport *vp){
    struct MouseStatus mouse;
    fread(&mouse, sizeof(mouse), 1, mouseFile);
    fseek(mouseFile, 0, SEEK_SET);

    if(
        mouse.pos.x > vp->loc.x &&
        mouse.pos.x < vp->loc.x + vp->loc.w &&
        mouse.pos.y > vp->loc.y + 14 &&
        mouse.pos.y < vp->loc.y + vp->loc.h &&
        mouse.buttons.left
    ){
        h->rotX += ((double )mouse.lastDelta.y) * 0.01f;
        h->rotZ += ((double )mouse.lastDelta.x) * 0.01f;
    }    
}

int main(int argc, char **argv){
    printf("Hyperboloid Renderer Test!\n");
    WIDTH = 300;
    HEIGHT = 300;
    struct Hyperboloid *h = init_hyperboloid(50, 1);
    struct RenderContext *ctx = init_renderContext();

    FILE *mouseFile = fopen("/-/sys/mouse", "r");
    if(mouseFile == NULL){
        printf("Unable to access mouse!\n");
        return 1;
    }

    struct Viewport *vp = vp_open(WIDTH, HEIGHT, "Hyperboloid");
    if(vp == NULL) return 1;
    drawbuf = malloc(sizeof(uint32_t) * HEIGHT * WIDTH);
    vp_set_buffer(vp, drawbuf, sizeof(uint32_t) * HEIGHT * WIDTH);
    vp_set_options(vp, VP_OPT_RESIZE);
    vp_add_event_handler(vp, event_handler);
    running = 1;
    while(running){
        doRender(h, ctx, vp, drawbuf);

        handleMouseInteraction(mouseFile, h, ctx, vp);
        //rotateVec3Z(&ctx->camera, 0.2f);
        //h->rotY += 0.01f;
        //h->rotX += 0.01f;

        vp_copy(vp);
        yield();
    }

    fclose(mouseFile);
    vp_close(vp);
}

