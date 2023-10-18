#include "libc.h"

#define PI 3.14159263f
#define PI2 6.283

struct WINDOW *window;
uint32_t *win_buf;

typedef struct {
  float x;
  float y;
} Vector2;

typedef struct {
  float x;
  float y;
  float z;
} Vector3;

typedef Vector3 Vertex;

typedef struct {
  float m[4][4];
} Matrix4x4;

typedef struct {
  Vertex *v0;
  Vertex *v1;
  Vertex *v2;
} TriangleRef;

typedef struct {
  Vertex v0;
  Vertex v1;
  Vertex v2;
} TriangleExpl;

Vertex cube[] = {
  -1.0,-1.0,-1.0,
  -1.0,1.0,-1.0,
  1.0,1.0,-1.0,
  1.0,-1.0,-1.0,
  -1.0,-1.0,1.0,
  -1.0,1.0,1.0,
  1.0,1.0,1.0,
  1.0,-1.0,1.0,
};

TriangleRef cubeTri[12] = {
  &cube[0], &cube[1], &cube[2],
  &cube[0], &cube[2], &cube[3],
  &cube[0], &cube[4], &cube[7],
  &cube[0], &cube[7], &cube[3],
  &cube[4], &cube[5], &cube[6],
  &cube[4], &cube[6], &cube[7],
  &cube[1], &cube[5], &cube[6],
  &cube[1], &cube[6], &cube[2],
  &cube[0], &cube[1], &cube[5],
  &cube[0], &cube[5], &cube[4],
  &cube[3], &cube[2], &cube[6],
  &cube[3], &cube[6], &cube[7]
};

Matrix4x4 ProjectionMatrix = {0};
Matrix4x4 MatRotZ = {0};
Matrix4x4 MatRotX = {0};


void multiplyMatrixVector(Vector3 *vi, Vector3 *vo, Matrix4x4 *matrix);
TriangleExpl triangleRefToExpl(TriangleRef *tri);
void renderShape(TriangleRef *triangles, int numTriangles);
TriangleExpl computeTriangle(TriangleRef *triangle);
void drawTriangle(TriangleExpl triangle, uint32_t color);
void fillTriangle(TriangleExpl triangle, uint32_t color);

void memset(void *mem, char v, int size);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
float sin(float x);
float cos(float x);

int main(int argc, char** argv){
  window = window_open("3D");
  win_buf = window->backbuffer;
  window_update();
  set_schedule(ONFOCUS);

  float fNear = 0.1f;
  float fFar = 10.0f;
  float fFov = 90.0f;
  float fAspectRatio = (float) window->height / (float) window->width;
  float fFovRad = 1.0f / (sin(fFov * 0.5f / 180.0f * PI) / cos(fFov * 0.5f / 180.0f * PI));

  ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
  ProjectionMatrix.m[1][1] = fFovRad;
  ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
  ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
  ProjectionMatrix.m[2][3] = 1.0f;
  ProjectionMatrix.m[3][3] = 0.0f;

  
  float theta;
  int direction = 1;
  while(1){
    theta += 0.01f * direction;
    if(theta >= PI || theta <= 0){ direction *= -1;}

    MatRotZ.m[0][0] = cos(theta);
    MatRotZ.m[0][1] = sin(theta);
    MatRotZ.m[1][0] = -sin(theta);
    MatRotZ.m[1][1] = cos(theta);
    MatRotZ.m[2][2] = 1;
    MatRotZ.m[3][3] = 1;

    MatRotX.m[0][0] = 1;
    MatRotX.m[1][1] = cos(theta*0.5f);
    MatRotX.m[1][2] = sin(theta*0.5f);
    MatRotX.m[2][1] = -sin(theta*0.5f);
    MatRotX.m[2][2] = cos(theta * 0.5f);
    MatRotX.m[3][3] = 1;

    memset(win_buf, 0, window->width * window->height * sizeof(uint32_t));

    for(int i = 0; i < 12; i++){
      drawTriangle(computeTriangle(&cubeTri[i]), 0xFFFFFF);
    }

    /*
    int ver = rand() % 8;
    float amt = (rand() % 2 ? 0.05f : -0.05f);
    switch(rand() % 3){
      case 0:
        cube[ver].x += amt;
      break;
      case 1:
        cube[ver].y += amt;
      break;
      case 2:
        cube[ver].z += amt;
      break;
    }
    */

    window_update();

    //char c = getc();
    //switch(c){
    //  
    //}
    
  }
}

TriangleExpl computeTriangle(TriangleRef *triangle){
  TriangleExpl localTri = triangleRefToExpl(triangle);
  TriangleExpl triRotatedZX, triTranslated;
  TriangleExpl newTriangle;

  multiplyMatrixVector(&localTri.v0, &triRotatedZX.v0, &MatRotZ);
  multiplyMatrixVector(&localTri.v1, &triRotatedZX.v1, &MatRotZ);
  multiplyMatrixVector(&localTri.v2, &triRotatedZX.v2, &MatRotZ);

  multiplyMatrixVector(&triRotatedZX.v0, &triTranslated.v0, &MatRotX);
  multiplyMatrixVector(&triRotatedZX.v1, &triTranslated.v1, &MatRotX);
  multiplyMatrixVector(&triRotatedZX.v2, &triTranslated.v2, &MatRotX);


  triTranslated.v0.z += 3.0f;
  triTranslated.v1.z += 3.0f;
  triTranslated.v2.z += 3.0f;

  multiplyMatrixVector(&triTranslated.v0, &newTriangle.v0, &ProjectionMatrix);
  multiplyMatrixVector(&triTranslated.v1, &newTriangle.v1, &ProjectionMatrix);
  multiplyMatrixVector(&triTranslated.v2, &newTriangle.v2, &ProjectionMatrix);

  newTriangle.v0.x += 1.0f; newTriangle.v0.y += 1.0f;
  newTriangle.v1.x += 1.0f; newTriangle.v1.y += 1.0f;
  newTriangle.v2.x += 1.0f; newTriangle.v2.y += 1.0f;
  newTriangle.v0.x *= 0.5f * (float) window->width;
  newTriangle.v0.y *= 0.5f * (float) window->height;
  newTriangle.v1.x *= 0.5f * (float) window->width;
  newTriangle.v1.y *= 0.5f * (float) window->height;
  newTriangle.v2.x *= 0.5f * (float) window->width;
  newTriangle.v2.y *= 0.5f * (float) window->height;

  drawTriangle(newTriangle);  
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
  if(!(x1 >= 0 && x1 <= window->width && y1 >= 0 && y1 <= window->height)){
    return;
  }
  int dx = abs(x2 - x1);
  int sx = x1 < x2 ? 1 : -1;
  int dy = -abs(y2 - y1);
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;
  int e2;

  for(;;){
    if(x1 >= 0 && x1 <= window->width && y1 >= 0 && y1 <= window->height){
      win_buf[x1 + y1 * window->width] = color;
    }    
    if(x1 == x2 && y1 == y2) break;
    e2 = 2 * err;
    if(e2 >= dy){err += dy; x1 += sx;}
    if(e2 <= dx){err += dx; y1 += sy;}
  }
}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
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