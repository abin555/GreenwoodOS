#include <sys/vp.h>
#include <sys/task.h>
#include <stdbool.h>
#include <sys/io.h>
#include <sys/memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "3D_types.h"

#define NULL 0
#define PI 3.14159263f
#define PI2 6.283

void multiplyMatrixVector(Vector3 *vi, Vector3 *vo, Matrix4x4 *matrix);
TriangleExpl triangleRefToExpl(TriangleRef *tri);
TriangleExpl transformTriangle(TriangleExpl triangle);
TriangleExpl projectTriangle(TriangleExpl triangle);
Vector3 calculateNormal(TriangleExpl triangle);
void rotateObjZ(Object *obj, float theta);
void rotateObjX(Object *obj, float theta);
void rotateObjY(Object *obj, float theta);

Object loadObjFile(char *filename);


float edgeFunction(Vector3 *a, Vector3 *b, Vector3 *c);
void drawTriangle(TriangleExpl triangle, uint32_t color);
void fillTriangle(TriangleExpl triangle, TriangleExpl *unprojected, uint32_t color);
void printTriangle(TriangleExpl tri);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
float sin(float x);
float cos(float x);
float sqrtf(float x);
void *alloc(int size);
float min3(float a, float b, float c);
float max3(float a, float b, float c);

int max(int a, int b);
int min(int a, int b);

Vector3 camera = {0.0f, 0.0f, -3.0f};
Vector3 camera_rot = {0.0f, 0.0f, 0.0f};
Vector3 light = {0.5f, -1.0f, -1.0f};

struct WINDOW *window;
uint32_t *win_buf;

Matrix4x4 ProjectionMatrix = {0};
Matrix4x4 MatRotZ = {0};
Matrix4x4 MatRotX = {0};
Matrix4x4 MatRotY = {0};

float fNear;
float fFar;
float fFov;
float fAspectRatio;
float fFovRad;
char Zbuffering;
char Outlines;
char SlowRender;
Object obj;

void *region;
uint32_t region_size;

void endCallback();
void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

#define WIDTH 250
#define HEIGHT 250

struct ViewportFunctions *vp_funcs;
struct Viewport *vp;
uint32_t *vp_buf;
int running;

uint32_t local_vp_buf[WIDTH * HEIGHT];

int main(int argc, char** argv){
  set_schedule(ALWAYS);

  fNear = 0.1f;
  fFar = 10.0f;
  fFov = 90.0f;
  fAspectRatio = (float) HEIGHT / (float) WIDTH;
  fFovRad = 1.0f / (sin(fFov * 0.5f / 180.0f * PI) / cos(fFov * 0.5f / 180.0f * PI));

  ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
  ProjectionMatrix.m[1][1] = fFovRad;
  ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
  ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
  ProjectionMatrix.m[2][3] = 1.0f;
  ProjectionMatrix.m[3][3] = 0.0f;

  
  Zbuffering = 1;
  Outlines = 0;
  SlowRender = 0;

  float theta;
  int direction = 1;
  Vector3 light_normalized;

  TriangleRef *triangleBuf;

  if(argc >= 2){
    int testOpen = open(argv[1], O_READ);
    close(testOpen);
    if(testOpen == -1){
      return 1;
    }
    obj = loadObjFile(argv[1]);
  }
  else{
    return 1;
  }

  vp = vp_open(WIDTH, HEIGHT, argv[1]);
  vp->transparent = 1;
  vp_buf = local_vp_buf;//requestRegion(WIDTH * HEIGHT * sizeof(uint32_t));
  vp_set_buffer(vp, vp_buf, WIDTH * HEIGHT * sizeof(uint32_t));
  vp_add_event_handler(vp, event_handler);
  vp_copy(vp);


  //addEndCallback(endCallback);
  //window->backbuffer = obj.ZBuff;
  int spinning = 0;
  running = 1;

  if(argc == 3){
    spinning = 1;
    vp->loc.x+=50;
    vp->loc.y+=50;
  }

  while(running){

    memset(vp_buf, 0, WIDTH * HEIGHT * sizeof(uint32_t));
    for(unsigned int i = 0; i < WIDTH * HEIGHT && Zbuffering; i++){
      obj.ZBuff[i] = 0;
    }


    float l = sqrtf(light.x*light.x + light.y*light.y + light.z*light.z);
    light_normalized.x = light.x / l; 
    light_normalized.y = light.y / l; 
    light_normalized.z = light.z / l;

    TriangleExpl tri;
    TriangleExpl projectedTri;
    Vector3 normal;
    float light_dp;
    int color_shift;
    uint32_t color_mod;

    /*
    Set Up Rotation Matrices for Camera
    */
    MatRotX.m[0][0] = 1;
    MatRotX.m[1][1] = cos(camera_rot.x);
    MatRotX.m[1][2] = sin(camera_rot.x);
    MatRotX.m[2][1] = -sin(camera_rot.x);
    MatRotX.m[2][2] = cos(camera_rot.x);
    MatRotX.m[3][3] = 1;

    MatRotY.m[0][0] = cos(camera_rot.y);
    MatRotY.m[2][0] = sin(camera_rot.y);
    MatRotY.m[1][1] = 1;
    MatRotY.m[0][2] = -sin(camera_rot.y);
    MatRotY.m[2][2] = cos(camera_rot.y);

    MatRotZ.m[0][0] = cos(camera_rot.z);
    MatRotZ.m[0][1] = sin(camera_rot.z);
    MatRotZ.m[1][0] = -sin(camera_rot.z);
    MatRotZ.m[1][1] = cos(camera_rot.z);
    MatRotZ.m[2][2] = 1;
    MatRotZ.m[3][3] = 1;


    for(int i = 0; i < obj.numTriangle; i++){
      tri = triangleRefToExpl(&obj.triangles[i]);
      tri = transformTriangle(tri);
      normal = calculateNormal(tri);
      
      
      if(
        normal.x * (tri.v0.x) +
        normal.y * (tri.v0.y) +
        normal.z * (tri.v0.z) >= 0.0f || tri.v0.z >= 0.0f || tri.v1.z >= 0.0f || tri.v2.z >= 0.0f 
        ) continue;
      

      light_dp = normal.x * light_normalized.x + normal.y * light_normalized.y + normal.z * light_normalized.z;
      light_dp += 1.0f;
      light_dp *= 0.5;
      color_shift = (int) 0xFF * light_dp;
      if(color_shift < 0) color_shift = 0;
      color_mod = color_shift << 16 | color_shift << 8 | color_shift;

      projectedTri = projectTriangle(tri);

      float xmin = min3(projectedTri.v0.x, projectedTri.v1.x, projectedTri.v2.x);
      float ymin = min3(projectedTri.v0.y, projectedTri.v1.y, projectedTri.v2.y);
      float xmax = max3(projectedTri.v0.x, projectedTri.v1.x, projectedTri.v2.x);
      float ymax = max3(projectedTri.v0.y, projectedTri.v1.y, projectedTri.v2.y);
      if (xmin > WIDTH - 1 || xmax < 0 || ymin > HEIGHT - 1 || ymax < 0) continue;
      
      fillTriangle(projectedTri, &tri, 0xFFFFFF - color_mod);
      if(Outlines){
        drawTriangle(projectedTri, 0);
      }
      //
      //drawTriangle(tri, 0xFFFFFF);
      if(SlowRender) vp_copy(vp);
    }
    
    

    /*
    int ver = rand() % obj.numVertex;
    float amt = (rand() % 2 ? 0.05f : -0.05f);
    switch(rand() % 3){
      case 0:
        obj.vertices[ver].x += amt;
      break;
      case 1:
        obj.vertices[ver].y += amt;
      break;
      case 2:
        obj.vertices[ver].z += amt;
      break;
    }
    */
    vp_copy(vp);

    char c = vp->ascii;
    switch(c){
      case '\0':
        break;
      case 'e':
        rotateObjZ(&obj, 0.1f);
        break;
      case 'q':
        rotateObjZ(&obj, -0.1f);
        break;
      case 's':
        rotateObjX(&obj, 0.1f);
        break;
      case 'w':
        rotateObjX(&obj, -0.1f);
        break;
      case 'a':
        rotateObjY(&obj, 0.1f);
        break;
      case 'd':
        rotateObjY(&obj, -0.1f);
        break;
      case 'S':
        spinning = 1;
        break;
      case 'X':
        spinning = 0;
        break;
      case 'y':
        camera.y -= 0.1f;
        break;
      case 'h':
        camera.y += 0.1f;
        break;
      case 'i':
        camera.z -= 0.1f;
        break;
      case 'k':
        camera.z += 0.1f;
        break;
      case 'j':
        camera.x -= 0.1f;
        break;
      case 'l':
        camera.x += 0.1f;
        break;
      case ',':
        camera_rot.y -= 0.1f;
        if(camera_rot.y <= -PI) camera_rot.y = PI;
        break;
      case '.':
        camera_rot.y += 0.1f;
        if(camera_rot.y >= PI) camera_rot.y = -PI;
        break;
      case '<':
        camera_rot.x -= 0.1f;
        if(camera_rot.x <= -PI) camera_rot.x = PI;
        break;
      case '>':
        camera_rot.x += 0.1f;
        if(camera_rot.x >= PI) camera_rot.x = -PI;
        break; 
      case 'z':
        Zbuffering = !Zbuffering;
        break;
      case 'x':
        Outlines = !Outlines;
        break;
      case 'v':
        SlowRender = !SlowRender;
        break;
      case '1':
        fFov -= 0.5f;
        fFovRad = 1.0f / (sin(fFov * 0.5f / 180.0f * PI) / cos(fFov * 0.5f / 180.0f * PI));
        ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
        ProjectionMatrix.m[1][1] = fFovRad;
        ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
        ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
        ProjectionMatrix.m[2][3] = 1.0f;
        ProjectionMatrix.m[3][3] = 0.0f;
        break;
      case '2':
        fFov += 0.5f;
        fFovRad = 1.0f / (sin(fFov * 0.5f / 180.0f * PI) / cos(fFov * 0.5f / 180.0f * PI));
        ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
        ProjectionMatrix.m[1][1] = fFovRad;
        ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
        ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
        ProjectionMatrix.m[2][3] = 1.0f;
        ProjectionMatrix.m[3][3] = 0.0f;
        break;
      case '3':
        fFar -= 0.5f;
        ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
        ProjectionMatrix.m[1][1] = fFovRad;
        ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
        ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
        ProjectionMatrix.m[2][3] = 1.0f;
        ProjectionMatrix.m[3][3] = 0.0f;
        break;
      case '4':
        fFar += 0.5f;
        ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
        ProjectionMatrix.m[1][1] = fFovRad;
        ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
        ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
        ProjectionMatrix.m[2][3] = 1.0f;
        ProjectionMatrix.m[3][3] = 0.0f;
        break;
      case '5':
        fNear -= 0.5f;
        ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
        ProjectionMatrix.m[1][1] = fFovRad;
        ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
        ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
        ProjectionMatrix.m[2][3] = 1.0f;
        ProjectionMatrix.m[3][3] = 0.0f;
        break;
      case '6':
        fNear += 0.5f;
        ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
        ProjectionMatrix.m[1][1] = fFovRad;
        ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
        ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
        ProjectionMatrix.m[2][3] = 1.0f;
        ProjectionMatrix.m[3][3] = 0.0f;
        break;
    }
    if(c != '\0') vp->ascii = '\0';

    if(spinning){
      rotateObjY(&obj, -0.01f);
    }

  }
  endCallback();
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_FOCUSED || event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
    }
    else if(/*event == VP_UNFOCUSED ||*/ event == VP_MINIMIZE){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
        set_schedule(ALWAYS);
    }
}

void endCallback(){
  memory_returnRegion(region, region_size);
  //freeRegion(vp_buf, WIDTH * HEIGHT * sizeof(uint32_t));
}

void rotateObjZ(Object *obj, float theta){
  MatRotZ.m[0][0] = cos(theta);
  MatRotZ.m[0][1] = sin(theta);
  MatRotZ.m[1][0] = -sin(theta);
  MatRotZ.m[1][1] = cos(theta);
  MatRotZ.m[2][2] = 1;
  MatRotZ.m[3][3] = 1;

  Vector3 workingVector;
  for(int i = 0; i < obj->numVertex; i++){
    workingVector = obj->vertices[i];
    multiplyMatrixVector(&workingVector, &obj->vertices[i], &MatRotZ);
  }
}

void rotateObjX(Object *obj, float theta){
  MatRotX.m[0][0] = 1;
  MatRotX.m[1][1] = cos(theta);
  MatRotX.m[1][2] = sin(theta);
  MatRotX.m[2][1] = -sin(theta);
  MatRotX.m[2][2] = cos(theta);
  MatRotX.m[3][3] = 1;

  Vector3 workingVector;
  for(int i = 0; i < obj->numVertex; i++){
    workingVector = obj->vertices[i];
    multiplyMatrixVector(&workingVector, &obj->vertices[i], &MatRotX);
  }
}

void rotateObjY(Object *obj, float theta){
  MatRotY.m[0][0] = cos(theta);
  MatRotY.m[2][0] = sin(theta);
  MatRotY.m[1][1] = 1;
  MatRotY.m[0][2] = -sin(theta);
  MatRotY.m[2][2] = cos(theta);

  Vector3 workingVector;
  for(int i = 0; i < obj->numVertex; i++){
    workingVector = obj->vertices[i];
    multiplyMatrixVector(&workingVector, &obj->vertices[i], &MatRotY);
  }
}

TriangleExpl transformTriangle(TriangleExpl triangle){
  TriangleExpl triTranslated;

  //multiplyMatrixVector(&triangle.v0, &triRotatedZX.v0, &MatRotZ);
  //multiplyMatrixVector(&triangle.v1, &triRotatedZX.v1, &MatRotZ);
  //multiplyMatrixVector(&triangle.v2, &triRotatedZX.v2, &MatRotZ);

  //multiplyMatrixVector(&triRotatedZX.v0, &triTranslated.v0, &MatRotX);
  //multiplyMatrixVector(&triRotatedZX.v1, &triTranslated.v1, &MatRotX);
  //multiplyMatrixVector(&triRotatedZX.v2, &triTranslated.v2, &MatRotX);
  triangle.v0.z += camera.z;
  triangle.v1.z += camera.z;
  triangle.v2.z += camera.z;

  triangle.v0.y += camera.y;
  triangle.v1.y += camera.y;
  triangle.v2.y += camera.y;

  triangle.v0.x += camera.x;
  triangle.v1.x += camera.x;
  triangle.v2.x += camera.x;

  multiplyMatrixVector(&triangle.v0, &triTranslated.v0, &MatRotY);
  multiplyMatrixVector(&triangle.v1, &triTranslated.v1, &MatRotY);
  multiplyMatrixVector(&triangle.v2, &triTranslated.v2, &MatRotY);

  multiplyMatrixVector(&triangle.v0, &triTranslated.v0, &MatRotX);
  multiplyMatrixVector(&triangle.v1, &triTranslated.v1, &MatRotX);
  multiplyMatrixVector(&triangle.v2, &triTranslated.v2, &MatRotX);

  return triTranslated;
}

TriangleExpl projectTriangle(TriangleExpl triangle){
  TriangleExpl newTriangle;
  multiplyMatrixVector(&triangle.v0, &newTriangle.v0, &ProjectionMatrix);
  multiplyMatrixVector(&triangle.v1, &newTriangle.v1, &ProjectionMatrix);
  multiplyMatrixVector(&triangle.v2, &newTriangle.v2, &ProjectionMatrix);

  newTriangle.v0.x += 1.0f; newTriangle.v0.y += 1.0f;
  newTriangle.v1.x += 1.0f; newTriangle.v1.y += 1.0f;
  newTriangle.v2.x += 1.0f; newTriangle.v2.y += 1.0f;
  newTriangle.v0.x *= 0.5f * (float) WIDTH;
  newTriangle.v0.y *= 0.5f * (float) HEIGHT;
  newTriangle.v1.x *= 0.5f * (float) WIDTH;
  newTriangle.v1.y *= 0.5f * (float) HEIGHT;
  newTriangle.v2.x *= 0.5f * (float) WIDTH;
  newTriangle.v2.y *= 0.5f * (float) HEIGHT;

  return newTriangle;  
}

Vector3 calculateNormal(TriangleExpl triangle){
  Vector3 normal, line1, line2;
  line1.x = triangle.v1.x - triangle.v0.x;
  line1.y = triangle.v1.y - triangle.v0.y;
  line1.z = triangle.v1.z - triangle.v0.z;

  line2.x = triangle.v2.x - triangle.v0.x;
  line2.y = triangle.v2.y - triangle.v0.y;
  line2.z = triangle.v2.z - triangle.v0.z;

  normal.x = line1.y*line2.z - line1.z*line2.y;
  normal.y = line1.z*line2.x - line1.x*line2.z;
  normal.z = line1.x*line2.y - line1.y*line2.x;

  float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
  normal.x /= l; normal.y /= l; normal.z /= l;

  return normal;
}

void drawTriangle(TriangleExpl triangle, uint32_t color){
  drawLine((int) triangle.v0.x, (int) triangle.v0.y, (int) triangle.v1.x, (int) triangle.v1.y, color);
  drawLine((int) triangle.v1.x, (int) triangle.v1.y, (int) triangle.v2.x, (int) triangle.v2.y, color);
  drawLine((int) triangle.v2.x, (int) triangle.v2.y, (int) triangle.v0.x, (int) triangle.v0.y, color);
}

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

float cos(float x)
{
  x = modd(x, PI2);
  return 1 - ((x * x) / (2)) + ((x * x * x * x) / (24)) - ((x * x * x * x * x * x) / (720)) + ((x * x * x * x * x * x * x * x) / (40320)) - ((x * x * x * x * x * x * x * x * x * x) / (3628800)) + ((x * x * x * x * x * x * x * x * x * x * x * x) / (479001600));

}

float sin(float x){
  return cos(x - (PI / 2));
}

int abs(int x){
  if(x < 0) return x * -1;
  return x;
}

void drawLine(int x1, int y1, int x2, int y2, uint32_t color){
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
      vp_buf[x1 + y1 * WIDTH] = color;
    }    
    if(x1 == x2 && y1 == y2) break;
    e2 = 2 * err;
    if(e2 >= dy){err += dy; x1 += sx;}
    if(e2 <= dx){err += dx; y1 += sy;}
  }
}

float calcZ(Vector3 *p1, Vector3 *p2, Vector3 *p3, float x, float y) {
        float det = (p2->y - p3->y) * (p1->x - p3->x) + (p3->x - p2->x) * (p1->y - p3->y);

        float l1 = ((p2->y - p3->y) * (x - p3->x) + (p3->x - p2->x) * (y - p3->y)) / det;
        float l2 = ((p3->y - p1->y) * (x - p3->x) + (p1->x - p3->x) * (y - p3->y)) / det;
        float l3 = 1.0f - l1 - l2;

        return l1 * p1->y + l2 * p2->y + l3 * p3->y;
}

int calculateZBuffer(int x, int y, TriangleExpl *projectedTri, TriangleExpl *unProjectedTri){
  TriangleExpl workTri;
  workTri.v0.x = projectedTri->v0.x;
  workTri.v1.x = projectedTri->v1.x;
  workTri.v2.x = projectedTri->v2.x;
  workTri.v0.y = projectedTri->v0.y;
  workTri.v1.y = projectedTri->v1.y;
  workTri.v2.y = projectedTri->v2.y;
  workTri.v0.z = unProjectedTri->v0.z;
  workTri.v1.z = unProjectedTri->v1.z;
  workTri.v2.z = unProjectedTri->v2.z;

  //ZBuffType z = (ZBuffType) (calcZ(workTri.v0, workTri.v1, workTri.v2, (float) x, (float) y) * 100);
  ZBuffType z = (ZBuffType) (((workTri.v0.z + workTri.v1.z + workTri.v2.z) / 3) * 100);
  //print_arg("Z: %x\n", z);
  if(z > obj.ZBuff[x + y * WIDTH]){
    obj.ZBuff[x + y * WIDTH] = z;
    return 1;
  }
  //print("Blocked!\n");
  return 0;
}

void drawHline(int x1, int x2, int y, uint32_t color, TriangleExpl *projectedTri, TriangleExpl *unProjectedTri){
  //if(x1 < 0) x1 = 0;
  //if(x1 > WIDTH) x1 = WIDTH;
  //if(x2 < 0) x2 = 0;
  //if(x2 > WIDTH) x2 = WIDTH;
  //if(y < 0 || y >= HEIGHT) return;
  //print_arg("Z0: %d ", (int) (tri->v0.z * 1000));
  //print_arg("Z1: %d ", (int) (tri->v1.z * 1000));
  //print_arg("Z2: %d\n", (int) (tri->v2.z * 1000));
  for(int x = x1; x < x2; x++){
    if(Zbuffering){
      if(x < WIDTH && x > 0 && y > 0 && y < HEIGHT && calculateZBuffer(x, y, projectedTri, unProjectedTri)){
        vp_buf[x + y*WIDTH] = color;
      }
    }
    else{
      if(x < WIDTH && x > 0 && y > 0 && y < HEIGHT){
        vp_buf[x + y*WIDTH] = color;
      }
    }
  }
}

#define SWAP(x,y) do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)
void fillTriangle(TriangleExpl triangle, TriangleExpl *unprojected, uint32_t color){
  int x1, y1, x2, y2, x3, y3;
  x1 = triangle.v0.x;
  y1 = triangle.v0.y;
  x2 = triangle.v1.x;
  y2 = triangle.v1.y;
  x3 = triangle.v2.x;
  y3 = triangle.v2.y;
  //if((x1 > WIDTH || x2 > WIDTH) || (x1 < 0 || x2 < 0) || (y1 > HEIGHT || y2 > HEIGHT) || (y1 < 0 || y2 < 0)) return;
  //else{
  //if(x1 < 0) x1 = 0;
  //if(x2 < 0) x2 = 0;
  //if(x3 < 0) x3 = 0;
  //if(y1 < 0) y1 = 0;
  //if(y2 < 0) y2 = 0;
  //if(y3 < 0) y3 = 0;
  //if(x1 > WIDTH) x1 = WIDTH;
  //if(x2 > WIDTH) x2 = WIDTH;
  //if(x3 > WIDTH) x3 = WIDTH;
  //if(y1 > HEIGHT) y1 = HEIGHT;
  //if(y2 > HEIGHT) y2 = HEIGHT;
  //if(y3 > HEIGHT) y3 = HEIGHT;
  //}

  int t1x,t2x,y,minx,maxx,t1xp,t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1,signx2,dx1,dy1,dx2,dy2;
	int e1,e2;
    // Sort vertices
	if (y1>y2) { SWAP(y1,y2); SWAP(x1,x2); }
	if (y1>y3) { SWAP(y1,y3); SWAP(x1,x3); }
	if (y2>y3) { SWAP(y2,y3); SWAP(x2,x3); }
  //if(abs(x3 - x1) > WIDTH) return;  
  
  int maxsteps = 5000;

	t1x=t2x=x1; y=y1;   // Starting points

	dx1 = (int)(x2 - x1); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = (int)(y2 - y1);
 
	dx2 = (int)(x3 - x1); if(dx2<0) { dx2=-dx2; signx2=-1; } else signx2=1;
	dy2 = (int)(y3 - y1);
	
	if (dy1 > dx1) {   // swap values
        SWAP(dx1,dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
        SWAP(dy2,dx2);
		changed2 = true;
	}
	
	e2 = (int)(dx2>>1);
    // Flat top, just process the second half
    if(y1==y2) goto next;
    e1 = (int)(dx1>>1);
	
	for (int i = 0; i < dx1;) {
    maxsteps--; if(!maxsteps) return;
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
        // process first line until y value is about to change
		while(i<dx1) {
      maxsteps--; if(!maxsteps) return;
			i++;			
			e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   if (changed1) t1xp=signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
	// Move line
	next1:
        // process second line until y value is about to change
		while (1) {
      maxsteps--; if(!maxsteps) return;
			e2 += dy2;		
			while (e2 >= dx2) {
        maxsteps--; if(!maxsteps) return;
				e2 -= dx2;
				if (changed2) t2xp=signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	drawHline(minx, maxx, y, color, &triangle, unprojected);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y==y2) break;
		
   }
	next:
	// Second half
	dx1 = (int)(x3 - x2); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = (int)(y3 - y2);
	t1x=x2;
 
	if (dy1 > dx1) {   // swap values
        SWAP(dy1,dx1);
		changed1 = true;
	} else changed1=false;
	
	e1 = (int)(dx1>>1);
	
	for (int i = 0; i<=dx1; i++) {
    maxsteps--; if(!maxsteps) return;
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
	    // process first line until y value is about to change
		while(i<dx1) {
      maxsteps--; if(!maxsteps) return;
      e1 += dy1;
      while (e1 >= dx1) {
        maxsteps--; if(!maxsteps) return;
        e1 -= dx1;
            if (changed1) { t1xp=signx1; break; }//t1x += signx1;
            else goto next3;
      }
			if (changed1) break;
			else   	   	  t1x += signx1;
			if(i<dx1) i++;
		}
	next3:
        // process second line until y value is about to change
		while (t2x!=x3) {
      maxsteps--; if(!maxsteps) return;
			e2 += dy2;
	   	   	while (e2 >= dx2) {
            maxsteps--; if(!maxsteps) return;
				    e2 -= dx2;
				    if(changed2) t2xp=signx2;
				    else          goto next4;
			    }
			if (changed2)     break;
			else              t2x += signx2;
		}	   	   
	next4:

		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	drawHline(minx, maxx, y, color, &triangle, unprojected);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y>y3) return;
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

TriangleExpl triangleRefToExpl(TriangleRef *tri){
  TriangleExpl triangle;
  triangle.v0 = *tri->v0;
  triangle.v1 = *tri->v1;
  triangle.v2 = *tri->v2;
  return triangle;
}

float sqrtf(float x) {
  union {
    int i;
    float x;
  } u;
  u.x = x;
  u.i = (1<<29) + (u.i >> 1) - (1 << 22);

  u.x = u.x + x/u.x;
  u.x = 0.25f*u.x + x/u.x;

  return u.x;
}

uint32_t last_alloc = 0x6000;

void *alloc(int size){
  void *buf = (void *) last_alloc;
  last_alloc += size + 0x10;
  return buf;
}

Object loadObjFile(char *filename){
  printf("Opening %s\n", filename);
  Object obj = {0};
  int obj_file = open(filename, O_READ);
  if(obj_file == -1){
    printf("File does not exist!\n");
    return obj;
  };
  int size = lseek(obj_file, 0, 2);
  lseek(obj_file, 0, 0);
  region_size = sizeof(Vertex) * 10000 + sizeof(TriangleRef) * 10000 + WIDTH * HEIGHT * sizeof(ZBuffType) + size;
  region = memory_requestRegion(region_size);
  last_alloc = (uint32_t) region;
  char *filebuf = alloc(size);
  read(obj_file, filebuf, size);
  close(obj_file);
  char *scan = filebuf;
  
  
  Vertex *vertices = alloc(sizeof(Vertex) * 10000);
  TriangleRef *triangles = alloc(sizeof(TriangleRef) * 10000);

  obj.numTriangle = 0;
  obj.numVertex = 0;
  obj.vertices = vertices;
  obj.triangles = triangles;
  obj.ZBuff = alloc(WIDTH * HEIGHT * sizeof(ZBuffType));

  float coord;
  while(*scan != 0 && (int) (scan - filebuf) < size){
    switch(*scan){
      case '#':
      case 'o':
      case 's':
        break;
      
      case 'v':{
        if(*(scan+1) != ' ') continue;
        scan += 2;
        //print_arg("Vertex %d: \n", obj.numVertex);
        //print_arg("<SCAN: %d>", (int) (scan-filebuf));
        coord = atof(scan);
        vertices[obj.numVertex].x = coord;
        //print_arg("X: %x ", *((uint32_t *) &coord));
        while(*scan != ' ' && *scan != '\n') scan++;
        scan++;
        //print_arg("<SCAN: %d>", (int) (scan-filebuf));
        coord = atof(scan);
        vertices[obj.numVertex].y = coord;
        //print_arg("Y: %x ", *((uint32_t *) &coord));
        while(*scan != ' ' && *scan != '\n') scan++;
        scan++;
        //print_arg("<SCAN: %d>", (int) (scan-filebuf));
        coord = atof(scan);
        vertices[obj.numVertex].z = coord;
        //print_arg("Z: %x\n", *((uint32_t *) &coord));
        //print("");
        write(0, NULL, 0);
        obj.numVertex++;
        break;
      }
        break;
      case 'f':{
        scan += 2;
        int idx = 0;
        idx = atoi(scan)-1;
        //print_arg("Face %d: ", obj.numTriangle);
        //print_arg("%d, ", idx);
        triangles[obj.numTriangle].v0 = &vertices[idx];
        while(*scan != ' ' && *scan != '\n') scan++;
        scan++;
        idx = atoi(scan)-1;
        triangles[obj.numTriangle].v1 = &vertices[idx];
        //print_arg("%d, ", idx);
        while(*scan != ' ' && *scan != '\n') scan++;
        scan++;
        idx = atoi(scan)-1;
        triangles[obj.numTriangle].v2 = &vertices[idx];
        //print_arg("%d\n", idx);
        //print_arg("\n%d>\n", obj.numTriangle);
        //printTriangle(triangleRefToExpl(&triangles[obj.numTriangle]));
        obj.numTriangle++;
        break;
      }
      default:
        break;
    }
    while(*scan != '\n'){
      scan++;
    }
    scan++;
    continue;
  }
  
  return obj;
}

float edgeFunction(Vector3 *a, Vector3 *b, Vector3 *c)
{ return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x); }

float min3(float a, float b, float c){
  if(a <= b && a <= c) return a;
  if(b <= a && b <= c) return b;
  if(c <= a && c <= b) return c;
}

float max3(float a, float b, float c){
  if(a >= b && a >= c) return a;
  if(b >= a && b >= c) return b;
  if(c >= a && c >= b) return c;
}

int max(int a, int b){if(a >= b) return a; return b;}
int min(int a, int b){if(a <= b) return a; return b;}
