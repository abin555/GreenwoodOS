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
  0.0,0.0,0.0,
  0.0,1.0,0.0,
  1.0,1.0,0.0,
  1.0,0.0,0.0,
  0.0,0.0,1.0,
  0.0,1.0,1.0,
  1.0,1.0,1.0,
  1.0,0.0,1.0,
};

TriangleRef cubeTri[12] = {
  &cube[0], &cube[1], &cube[2],
  &cube[0], &cube[2], &cube[3],

  &cube[3], &cube[2], &cube[6],
  &cube[3], &cube[6], &cube[7],
  
  &cube[7], &cube[6], &cube[5],
  &cube[7], &cube[5], &cube[4],
  
  &cube[4], &cube[5], &cube[1],
  &cube[4], &cube[1], &cube[0],
  
  &cube[1], &cube[5], &cube[6],
  &cube[1], &cube[6], &cube[2],
  
  &cube[7], &cube[4], &cube[0],
  &cube[7], &cube[0], &cube[3]
};

Matrix4x4 ProjectionMatrix = {0};
Matrix4x4 MatRotZ = {0};
Matrix4x4 MatRotX = {0};

float fNear;
float fFar;
float fFov;
float fAspectRatio;
float fFovRad;

void multiplyMatrixVector(Vector3 *vi, Vector3 *vo, Matrix4x4 *matrix);
TriangleExpl triangleRefToExpl(TriangleRef *tri);
void renderShape(TriangleRef *triangles, int numTriangles);
TriangleExpl computeTriangle(TriangleRef *triangle);
TriangleExpl transformTriangle(TriangleExpl triangle);
TriangleExpl projectTriangle(TriangleExpl triangle);
Vector3 calculateNormal(TriangleExpl triangle);

void drawTriangle(TriangleExpl triangle, uint32_t color);
void fillTriangle(TriangleExpl triangle, uint32_t color);

void memset(void *mem, char v, int size);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
float sin(float x);
float cos(float x);
float sqrtf(float x);

int main(int argc, char** argv){
  window = window_open("3D");
  win_buf = window->backbuffer;
  window_update();
  set_schedule(ONFOCUS);

  fNear = 0.1f;
  fFar = 10.0f;
  fFov = 90.0f;
  fAspectRatio = (float) window->height / (float) window->width;
  fFovRad = 1.0f / (sin(fFov * 0.5f / 180.0f * PI) / cos(fFov * 0.5f / 180.0f * PI));

  ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
  ProjectionMatrix.m[1][1] = fFovRad;
  ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
  ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
  ProjectionMatrix.m[2][3] = 1.0f;
  ProjectionMatrix.m[3][3] = 0.0f;

  
  const Vector3 camera = {0.0f, 0.0f, 0.0f};
  const Vector3 light = {0.0f, 0.0f, 1.0f};

  float theta;
  int direction = 1;
  Vector3 light_normalized;
  while(1){
    theta += 0.05f * direction;
    if(theta >= PI || theta <= -PI){ direction *= -1;}

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

    float l = sqrtf(light.x*light.x + light.y*light.y + light.z*light.z);
    light_normalized.x = light.x / l; 
    light_normalized.y = light.y / l; 
    light_normalized.z = light.z / l;

    TriangleExpl tri;
    Vector3 normal;
    float light_dp;
    int color_shift;
    uint32_t color_mod;
    for(int i = 0; i < 12; i++){
      tri = triangleRefToExpl(&cubeTri[i]);
      tri = transformTriangle(tri);
      normal = calculateNormal(tri);
      
      if(
        normal.x * (tri.v0.x - camera.x) +
        normal.y * (tri.v0.y - camera.y) +
        normal.z * (tri.v0.z - camera.z) >= 0.0f
        ) continue;

      light_dp = normal.x * light_normalized.x + normal.y * light_normalized.y + normal.z * light_normalized.z;
      light_dp += 1.0f;
      light_dp *= 0.5;
      color_shift = (int) 0xFF * light_dp;
      color_mod = color_shift << 16 | color_shift << 8 | color_shift;

      tri = projectTriangle(tri);
      fillTriangle(tri, 0xFFFFFF - color_mod);
      drawTriangle(tri, 0);
      //drawTriangle(computeTriangle(&cubeTri[i]), 0xFFFFFF);
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
  }
}

TriangleExpl transformTriangle(TriangleExpl triangle){
  TriangleExpl triRotatedZX, triTranslated;
  TriangleExpl newTriangle;

  multiplyMatrixVector(&triangle.v0, &triRotatedZX.v0, &MatRotZ);
  multiplyMatrixVector(&triangle.v1, &triRotatedZX.v1, &MatRotZ);
  multiplyMatrixVector(&triangle.v2, &triRotatedZX.v2, &MatRotZ);

  multiplyMatrixVector(&triRotatedZX.v0, &triTranslated.v0, &MatRotX);
  multiplyMatrixVector(&triRotatedZX.v1, &triTranslated.v1, &MatRotX);
  multiplyMatrixVector(&triRotatedZX.v2, &triTranslated.v2, &MatRotX);


  triTranslated.v0.z += 3.0f;
  triTranslated.v1.z += 3.0f;
  triTranslated.v2.z += 3.0f;
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
  newTriangle.v0.x *= 0.5f * (float) window->width;
  newTriangle.v0.y *= 0.5f * (float) window->height;
  newTriangle.v1.x *= 0.5f * (float) window->width;
  newTriangle.v1.y *= 0.5f * (float) window->height;
  newTriangle.v2.x *= 0.5f * (float) window->width;
  newTriangle.v2.y *= 0.5f * (float) window->height;

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

void drawHline(int x1, int x2, int y, uint32_t color){
  if(x1 < 0) x1 = 0;
  if(x1 > window->width) x1 = window->width;
  if(x2 < 0) x2 = 0;
  if(x2 > window->width) x2 = window->width;
  if(y < 0) y = 0;
  if(y > window->height) y = window->height;

  for(int x = x1; x < x2; x++) win_buf[x + y*window->width] = color;
}

#define SWAP(x,y) do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)
void fillTriangle(TriangleExpl triangle, uint32_t color){
  int x1, y1, x2, y2, x3, y3;
  x1 = triangle.v0.x;
  y1 = triangle.v0.y;
  x2 = triangle.v1.x;
  y2 = triangle.v1.y;
  x3 = triangle.v2.x;
  y3 = triangle.v2.y;

  int t1x,t2x,y,minx,maxx,t1xp,t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1,signx2,dx1,dy1,dx2,dy2;
	int e1,e2;
    // Sort vertices
	if (y1>y2) { SWAP(y1,y2); SWAP(x1,x2); }
	if (y1>y3) { SWAP(y1,y3); SWAP(x1,x3); }
	if (y2>y3) { SWAP(y2,y3); SWAP(x2,x3); }

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
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
        // process first line until y value is about to change
		while(i<dx1) {
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
			e2 += dy2;		
			while (e2 >= dx2) {
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
	   	drawHline(minx, maxx, y, color);    // Draw line from min to max points found on the y
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
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
	    // process first line until y value is about to change
		while(i<dx1) {
    		e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   	if (changed1) { t1xp=signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if(i<dx1) i++;
		}
	next3:
        // process second line until y value is about to change
		while (t2x!=x3) {
			e2 += dy2;
	   	   	while (e2 >= dx2) {
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
	   	drawHline(minx, maxx, y, color);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y>y3) return;
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