#include "libc.h"

#define PI 3.1415
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

typedef struct {
  Vector3 *v1;
  Vector3 *v2;
  Vector3 *v3;
} triangleVptr;

typedef struct {
  Vector3 *v1;
  Vector3 *v2;
  Vector3 *v3;
} Triangle;

typedef struct {
  Vector3 position;
  int numVerticies;
  Vector3 *verticies;
  int numTriangles;
  Triangle *triangles;
} Object;

typedef struct {
  float x;
  float y;
  float z;
  float focal;
} Camera;




void memset(void *mem, char v, int size);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
float sin(float x);
float cos(float x);
Vector2 CalculatePlane(Camera camera, Vector3 offset, Vector3 vector);
void drawTriangle(Camera camera, Vector3 offset, Triangle triangle);
void drawObject(Camera camera, Object *object);

Vector3 points[] = {
  {0, 0, 0},//LDF
  {0, 4, 0},//LUF
  {4, 0, 0},//RDF
  {4, 4, 0},//RUF
  {0, 0, 4},//LDB
  {0, 4, 4},//LUB
  {4, 0, 4},//RDB
  {4, 4, 4}//RUB
};

Triangle triangle[12] = {
    &points[0],
    &points[3],
    &points[2],

    &points[0],
    &points[1],
    &points[3],

    &points[2],
    &points[6],
    &points[7],

    &points[2],
    &points[3],
    &points[7],

    &points[0],
    &points[2],
    &points[6],

    &points[0],
    &points[4],
    &points[6],

    &points[1],
    &points[3],
    &points[5],

    &points[1],
    &points[7],
    &points[5],
  };


int main(int argc, char** argv){
  Camera camera;
  window = window_open("3D");
  win_buf = window->backbuffer;
  window_update();
  set_schedule(ONFOCUS);

  camera.x = 0;
  camera.y = 0;
  camera.z = 0;
  camera.focal = 0;

  Object box = {
    0, 0, 0,
    8,
    points,
    8,
    triangle
  };

  int vertex = 0;
  while(1){
    char c = getc();
    switch(c){
      case 'a':
        camera.x-=.1;
        break;
      case 'd':
        camera.x+=.1;
        break;
      case 'w':
        camera.y-=.1;
        break;
      case 's':
        camera.y+=.1;
        break;
      case 'q':
        camera.focal-=.1;
        break;
      case 'e':
        camera.focal+=.1;
        break;
      case 'r':
        camera.z-=.1;
        break;
      case 'f':
        camera.z+=.1;
        break;

      case 'y':
        vertex--;
        if(vertex < 0) vertex = 0;
        break;
      case 'h':
        vertex++;
        if(vertex > sizeof(points) / sizeof(Vector3)) vertex = 0;
        break;
      case 'i':
        points[vertex].y-=.1;
        break;
      case 'k':
        points[vertex].y+=.1;
        break;
      case 'j':
        points[vertex].x-=.1;
        break;
      case 'l':
        points[vertex].x+=.1;
        break;
      case 'u':
        points[vertex].z-=.1;
        break;
      case 'o':
        points[vertex].z+=.1;
        break;
      case ',':
        box.position.z-=.1;
        break;
      case '.':
        box.position.z+=.1;
        break;
      case 'n':
        box.position.x-=.1;
        break;
      case 'm':
        box.position.x+=.1;
        break;
      case '<':
        box.position.y-=.1;
        break;
      case '>':
        box.position.y+=.1;
        break;
    }
    memset(win_buf, 0, window->width * window->height * sizeof(uint32_t));
    /*
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        if(i == j) continue;
        drawLine((int)plots[i].x,(int) plots[i].y,(int) plots[j].x,(int) plots[j].y, 0xFFFFFF);
      }
    }
    */
    /*
    for(int i = 0; i < 7; i++){
      drawTriangle(camera, cube[i]);
    }
    */
    drawObject(camera, &box);
    window_update();
  }
}

Vector2 CalculatePlane(Camera camera, Vector3 offset, Vector3 vector){
  Vector2 result;
  result.x = camera.focal * ((vector.x - camera.x) / (camera.focal + vector.z - camera.z)) + camera.x;
  result.y = camera.focal * ((vector.y - camera.y) / (camera.focal + vector.z - camera.z)) + camera.y;
  
  //result.x = camera.x - (camera.focal - camera.z) * ((camera.x - vector.x) / (vector.z - camera.z + offset.z))  + offset.x;
  //result.y = camera.y - (camera.focal - camera.z) * ((camera.y - vector.y) / (vector.z - camera.z + offset.z))  + offset.y;
  result.x += window->width / 2;
  result.y += window->height / 2;
  return result;
}

void drawTriangle(Camera camera, Vector3 offset, Triangle triangle){
  Vector2 points[3];
  points[0] = CalculatePlane(camera, offset, *triangle.v1);
  points[1] = CalculatePlane(camera, offset, *triangle.v2);
  points[2] = CalculatePlane(camera, offset, *triangle.v3);
  drawLine((int)points[0].x,(int) points[0].y,(int) points[1].x,(int) points[1].y, 0xFFFFFF);
  drawLine((int)points[1].x,(int) points[1].y,(int) points[2].x,(int) points[2].y, 0xFFFFFF);
  drawLine((int)points[2].x,(int) points[2].y,(int) points[0].x,(int) points[0].y, 0xFFFFFF);
}

void drawObject(Camera camera, Object *object){
  for(int i = 0; i < object->numTriangles; i++){
    drawTriangle(camera, object->position, object->triangles[i]);
  }
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
  int dx = abs(x2 - x1);
  int sx = x1 < x2 ? 1 : -1;
  int dy = -abs(y2 - y1);
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;
  int e2;

  for(;;){
    win_buf[x1 + y1 * window->width] = color;
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