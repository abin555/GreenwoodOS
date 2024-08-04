#include "renderer.hpp"

Matrix4x4 MatRotZ = {0};
Matrix4x4 MatRotX = {0};
Matrix4x4 MatRotY = {0};

World::World(Window *window, int maxObjects){
    this->window = window;
    this->ZBuff = (ZBuffType *) sys.malloc(sizeof(ZBuffType) * this->window->getWidth() * this->window->getHeight());
    this->maxObjects = maxObjects;
    this->numObjects = 0;
    this->objects = (Object **) sys.malloc(maxObjects * sizeof(Object *));
    this->cam.position = {0.5f, 0.5f, 3.0f};
    this->cam.rotation = {0.0f, 0.0f, 0.0f};
    this->light = {0.5f, -1.0f, -1.0f};
    this->ZBuffering = 1;
    this->FillFaces = 1;
    this->DrawOutlines = 0;

    this->fNear = 0.1f;
    this->fFar = 10.0f;
    this->fFov = 90.0f;
    this->fAspectRatio = (float) this->window->getHeight() / (float) this->window->getWidth();
    this->fFovRad = 1.0f / (sin(this->fFov * 0.5f / 180.0f * PI) / cos(this->fFov * 0.5f / 180.0f * PI));

    this->ProjectionMatrix.m[0][0] = fAspectRatio * fFovRad;
    this->ProjectionMatrix.m[1][1] = fFovRad;
    this->ProjectionMatrix.m[2][2] = fFar / (fFar - fNear);
    this->ProjectionMatrix.m[3][3] = (-fFar * fNear) / (fFar - fNear);
    this->ProjectionMatrix.m[2][3] = 1.0f;
    this->ProjectionMatrix.m[3][3] = 0.0f;

    print_arg("World Setup Complete, max of %d Objects\n", this->maxObjects);
}

World::World(){

}

void World::addObject(Object *object){
    if(this->numObjects == this->maxObjects){
        print("World: Too many objects, can't add");
        return;
    }
    this->objects[this->numObjects] = object;
    this->numObjects++;
}

void World::render(){
    Vector3 light_normalized;
    TriangleExpl tri;
    TriangleExpl projectedTri;
    Vector3 normal;
    float light_dp;
    int color_shift;
    uint32_t color_mod;
    //print("Render Step\n");

    float l = sqrtf(this->light.x*this->light.x + this->light.y*this->light.y + this->light.z*this->light.z);
    light_normalized.x = this->light.x / l; 
    light_normalized.y = this->light.y / l; 
    light_normalized.z = this->light.z / l;

    memset(this->ZBuff, 0, sizeof(ZBuffType) * sys.window.getWidth() * sys.window.getHeight());
    for(int i = 0; i < this->numObjects; i++){
        //print_arg("Transformation Object %d\n", i);
        this->objects[i]->transform();
    }
    this->prepareRotationMatrices(&MatRotX, &MatRotY, &MatRotZ, this->cam.rotation);
    for(int i = 0; i < this->numObjects; i++){
        //print_arg("Drawing Object %d:\n", i);
        Object *obj = this->objects[i];        
        for(int j = 0; j < obj->localMesh->numTriangles; j++){
            //print_arg(" - Triangle %d\n", j);
            tri = triangleIdxToExpl(obj->localMesh, &obj->localMesh->triangles[j]);
            normal = calculateNormal(tri);
            light_dp = normal.x * light_normalized.x + normal.y * light_normalized.y + normal.z * light_normalized.z;
            tri = this->cameraTranslate(tri);
            tri = this->cameraRotate(tri);
            normal = calculateNormal(tri);
            
            if(
            normal.x * (tri.v0.x) +
            normal.y * (tri.v0.y) +
            normal.z * (tri.v0.z) >= 0.0f || tri.v0.z >= 0.0f || tri.v1.z >= 0.0f || tri.v2.z >= 0.0f 
            ) continue;
            

            //light_dp = normal.x * light_normalized.x + normal.y * light_normalized.y + normal.z * light_normalized.z;
            light_dp += 1.0f;
            light_dp *= 0.5;
            color_shift = (int) 0xFF * light_dp;
            if(color_shift < 0) color_shift = 0;
            color_mod = color_shift << 16 | color_shift << 8 | color_shift;

            projectedTri = this->projectTriangle(tri);

            //float xmin = min3(projectedTri.v0.x, projectedTri.v1.x, projectedTri.v2.x);
            //float ymin = min3(projectedTri.v0.y, projectedTri.v1.y, projectedTri.v2.y);
            //float xmax = max3(projectedTri.v0.x, projectedTri.v1.x, projectedTri.v2.x);
            //float ymax = max3(projectedTri.v0.y, projectedTri.v1.y, projectedTri.v2.y);
            //if (xmin > sys.window.window->width - 1 || xmax < 0 || ymin > sys.window.window->height - 1 || ymax < 0) continue;
            
            if(this->FillFaces) fillTriangle(this, &projectedTri, &tri, tri.color - ~color_mod);
            if(this->DrawOutlines){
              if(this->FillFaces) drawTriangle(projectedTri, 0);
              else drawTriangle(projectedTri, 0xFFFFFF);
            }
            //drawTriangle(projectedTri, 0xFFFFFF);
            //drawTriangle(projectedTri, 0);
        }
    }
}

struct TriangleExpl World::projectTriangle(TriangleExpl triangle){
  TriangleExpl newTriangle;
  multiplyMatrixVector(&triangle.v0, &newTriangle.v0, &this->ProjectionMatrix);
  multiplyMatrixVector(&triangle.v1, &newTriangle.v1, &this->ProjectionMatrix);
  multiplyMatrixVector(&triangle.v2, &newTriangle.v2, &this->ProjectionMatrix);

  newTriangle.v0.x += 1.0f; newTriangle.v0.y += 1.0f;
  newTriangle.v1.x += 1.0f; newTriangle.v1.y += 1.0f;
  newTriangle.v2.x += 1.0f; newTriangle.v2.y += 1.0f;
  newTriangle.v0.x *= 0.5f * (float) sys.window.getWidth();
  newTriangle.v0.y *= 0.5f * (float) sys.window.getHeight();
  newTriangle.v1.x *= 0.5f * (float) sys.window.getWidth();
  newTriangle.v1.y *= 0.5f * (float) sys.window.getHeight();
  newTriangle.v2.x *= 0.5f * (float) sys.window.getWidth();
  newTriangle.v2.y *= 0.5f * (float) sys.window.getHeight();

  return newTriangle;
}

struct TriangleExpl World::cameraTranslate(TriangleExpl triangle){
  triangle.v0.x -= this->cam.position.x;
  triangle.v1.x -= this->cam.position.x;
  triangle.v2.x -= this->cam.position.x;
  
  triangle.v0.y -= this->cam.position.y;
  triangle.v1.y -= this->cam.position.y;
  triangle.v2.y -= this->cam.position.y;
  
  triangle.v0.z -= this->cam.position.z;
  triangle.v1.z -= this->cam.position.z;
  triangle.v2.z -= this->cam.position.z;
  return triangle;
}

struct TriangleExpl World::cameraRotate(TriangleExpl triangle){
  TriangleExpl newTriangle;
  
  multiplyMatrixVector(&triangle.v0, &newTriangle.v0, &MatRotY);
  multiplyMatrixVector(&triangle.v1, &newTriangle.v1, &MatRotY);
  multiplyMatrixVector(&triangle.v2, &newTriangle.v2, &MatRotY);

  multiplyMatrixVector(&newTriangle.v0, &triangle.v0, &MatRotX);
  multiplyMatrixVector(&newTriangle.v1, &triangle.v1, &MatRotX);
  multiplyMatrixVector(&newTriangle.v2, &triangle.v2, &MatRotX);
  
  multiplyMatrixVector(&triangle.v0, &newTriangle.v0, &MatRotZ);
  multiplyMatrixVector(&triangle.v1, &newTriangle.v1, &MatRotZ);
  multiplyMatrixVector(&triangle.v2, &newTriangle.v2, &MatRotZ);

  return newTriangle;
}

Camera *World::getCamera(){
  return &this->cam;
}

void World::prepareRotationMatrices(Matrix4x4 *RotX, Matrix4x4 *RotY, Matrix4x4 *RotZ, Vector3 rot){
  RotX->m[0][0] = 1;
  RotX->m[1][1] = cos(rot.x);
  RotX->m[1][2] = sin(rot.x);
  RotX->m[2][1] = -sin(rot.x);
  RotX->m[2][2] = cos(rot.x);
  RotX->m[3][3] = 1;

  RotY->m[0][0] = cos(rot.y);
  RotY->m[2][0] = sin(rot.y);
  RotY->m[1][1] = 1;
  RotY->m[0][2] = -sin(rot.y);
  RotY->m[2][2] = cos(rot.y);

  RotZ->m[0][0] = cos(rot.z);
  RotZ->m[0][1] = sin(rot.z);
  RotZ->m[1][0] = -sin(rot.z);
  RotZ->m[1][1] = cos(rot.z);
  RotZ->m[2][2] = 1;
  RotZ->m[3][3] = 1;
}


void Object::copyMesh(){
    //memfcpy(this->localMesh->vertex, this->masterMesh->vertex, sizeof(Vertex) * this->masterMesh->numVertex);
    //memfcpy(this->localMesh->triangles, this->masterMesh->triangles, sizeof(Vertex) * this->masterMesh->numTriangles);
    memfcpy(this->localMesh->vertex, this->masterMesh->vertex, sizeof(Vertex) * this->masterMesh->numVertex);
    memfcpy(this->localMesh->triangles, this->masterMesh->triangles, sizeof(struct TriangleIdx) * this->masterMesh->numTriangles);
}

void Object::transform(){
    this->copyMesh();
    this->localMesh->rotateX(this->worldRotation.x);
    this->localMesh->rotateY(this->worldRotation.y);
    this->localMesh->rotateZ(this->worldRotation.z);
    this->localMesh->translate(this->worldPosition);
}

Object::Object(char *filename){
    this->masterMesh = (Mesh *) sys.malloc(sizeof(Mesh));
    this->masterMesh->loadObj(filename);
    this->localMesh = (Mesh *) sys.malloc(sizeof(Mesh));
    
    this->localMesh->numVertex = this->masterMesh->numVertex;
    this->localMesh->numTriangles = this->masterMesh->numTriangles;

    this->localMesh->vertex = (Vertex *) sys.malloc(sizeof(Vertex) * this->localMesh->numVertex);
    this->localMesh->triangles = (struct TriangleIdx *) sys.malloc(sizeof(struct TriangleIdx) * this->localMesh->numTriangles);
    this->worldRotation = {0};
    this->worldPosition = {0};
}

Object::Object(Mesh *masterMesh){
    this->masterMesh = masterMesh;
    this->localMesh = (Mesh *) sys.malloc(sizeof(Mesh));
    
    this->localMesh->numVertex = this->masterMesh->numVertex;
    this->localMesh->numTriangles = this->masterMesh->numTriangles;

    this->localMesh->vertex = (Vertex *) sys.malloc(sizeof(Vertex) * this->localMesh->numVertex);
    this->localMesh->triangles = (struct TriangleIdx *) sys.malloc(sizeof(struct TriangleIdx) * this->localMesh->numTriangles);
    this->worldRotation = {0};
    this->worldPosition = {0};
}

Object::Object(){
    this->masterMesh = NULL;
    this->localMesh = NULL;
    this->worldRotation = {0};
    this->worldPosition = {0};
}

void Object::cloneMaster(Mesh *original){
  print("Cloning Mesh!\n");
  this->masterMesh = (Mesh *) sys.malloc(sizeof(Mesh));
  this->masterMesh->numVertex = original->numVertex;
  this->masterMesh->numTriangles = original->numTriangles;
  this->masterMesh->vertex = (Vertex *) sys.malloc(sizeof(Vertex) * this->masterMesh->numVertex);
  this->masterMesh->triangles = (struct TriangleIdx *) sys.malloc(sizeof(struct TriangleIdx) * this->masterMesh->numTriangles);
  memfcpy(this->masterMesh->vertex, original->vertex, sizeof(Vertex) * this->masterMesh->numVertex);
  memfcpy(this->masterMesh->triangles, original->triangles, sizeof(struct TriangleIdx) * this->masterMesh->numTriangles);

  this->localMesh = (Mesh *) sys.malloc(sizeof(Mesh));
  this->localMesh->numVertex = this->masterMesh->numVertex;
  this->localMesh->numTriangles = this->masterMesh->numTriangles;

  this->localMesh->vertex = (Vertex *) sys.malloc(this->localMesh->numVertex);
  this->localMesh->triangles = (struct TriangleIdx *) sys.malloc(this->localMesh->numTriangles);
  this->worldRotation = {0};
  this->worldPosition = {0};
}

void Mesh::loadObj(char *filename){
    print_arg("Loading Obj Mesh File %s\n", (uint32_t) filename);
    //void *base_alloc = sys.malloc(sizeof(Vertex) * 0x1000);
    this->numVertex = 0;
    this->numTriangles = 0;

    int numTri = 0;
    int numVert = 0;


    

    struct FILE *obj_file = fopen(filename);
    if(!obj_file){
        print("File does not exist!\n");
        return;
    }
    int size = fsize(obj_file);
    print_arg("File is %d bytes\n", size);
    char *filebuf = (char *) sys.malloc(size);
    fcopy(obj_file, filebuf, size);
    fclose(obj_file);
    char *scan = filebuf;

    while(*scan != 0 && (int) (scan - filebuf) < size){
      switch(*scan){
        case '#':
        case 'o':
        case 's':
          break;
        case 'v':
          if(*(scan+1) != ' ') break;
          numVert++;
          break;
        case 'f':
          numTri++;
          break;
      }

      while(*scan != '\n'){
          scan++;
      }
      scan++;
    }
    scan = filebuf;

    this->vertex = (Vertex *) sys.malloc(sizeof(Vertex) * numVert);

    float coord;
    //Compute Verticies
    while(*scan != 0 && (int) (scan - filebuf) < size){
        switch(*scan){
            case '#':
            case 'o':
            case 's':
            case 'f':
                break;
            case 'v':{
                if(*(scan+1) != ' ') continue;
                scan += 2;
                //print_arg("Vertex %d: \n", obj.numVertex);
                //print_arg("<SCAN: %d>", (int) (scan-filebuf));
                coord = atof(scan);
                this->vertex[this->numVertex].x = coord;
                //print_arg("X: %x ", ((uint32_t) &coord));
                while(*scan != ' ' && *scan != '\n') scan++;
                scan++;
                //print_arg("<SCAN: %d>", (int) (scan-filebuf));
                coord = atof(scan);
                this->vertex[this->numVertex].y = coord;
                //print_arg("Y: %x ", ((uint32_t) &coord));
                while(*scan != ' ' && *scan != '\n') scan++;
                scan++;
                //print_arg("<SCAN: %d>", (int) (scan-filebuf));
                coord = atof(scan);
                this->vertex[this->numVertex].z = coord;
                //print_arg("Z: %x\n", ((uint32_t) &coord));
                print("");
                this->numVertex++;
                break;
            }
            
        }
        while(*scan != '\n'){
            scan++;
        }
        scan++;
        continue;
    }
    //Compute Faces
    //this->triangles = (struct TriangleIdx *) ((uint32_t) base_alloc + this->numVertex);
    this->triangles = (struct TriangleIdx *) sys.malloc(sizeof(struct TriangleIdx) * numTri);
    scan = filebuf;
    while(*scan != 0 && (int) (scan - filebuf) < size){
        switch(*scan){
            case '#':
            case 'o':
            case 's':
            case 'v':
                break;
            
            case 'f':{
                scan += 2;
                int idx = 0;
                idx = atoi(scan)-1;
                //print_arg("Face %d: ", this->numTriangles);
                //print_arg("%d, ", idx);
                this->triangles[this->numTriangles].v0 = idx;
                while(*scan != ' ' && *scan != '\n') scan++;
                scan++;
                idx = atoi(scan)-1;
                this->triangles[this->numTriangles].v1 = idx;
                //print_arg("%d, ", idx);
                while(*scan != ' ' && *scan != '\n') scan++;
                scan++;
                idx = atoi(scan)-1;
                this->triangles[this->numTriangles].v2 = idx;

                this->triangles[this->numTriangles].color = 0;
                //print_arg("%d\n", idx);
                //print_arg("\n%d>\n", obj.numTriangle);
                //printTriangle(triangleRefToExpl(&triangles[obj.numTriangle]));
                //printTriangle(triangleIdxToExpl(this, &this->triangles[this->numTriangles]));
                this->numTriangles++;
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
    print("Loaded object!\n");
    print_arg(" - %d verticies\n", this->numVertex);
    print_arg(" - %d triangles\n", this->numTriangles);
}

void Mesh::rotateX(float theta){
    MatRotX.m[0][0] = 1;
    MatRotX.m[1][1] = cos(theta);
    MatRotX.m[1][2] = sin(theta);
    MatRotX.m[2][1] = -sin(theta);
    MatRotX.m[2][2] = cos(theta);
    MatRotX.m[3][3] = 1;
    Vector3 workingVector;
    for(int i = 0; i < this->numVertex; i++){
        workingVector = this->vertex[i];
        multiplyMatrixVector(&workingVector, &this->vertex[i], &MatRotX);
    }
}

void Mesh::rotateY(float theta){
    MatRotY.m[0][0] = cos(theta);
    MatRotY.m[2][0] = sin(theta);
    MatRotY.m[1][1] = 1;
    MatRotY.m[0][2] = -sin(theta);
    MatRotY.m[2][2] = cos(theta);
    Vector3 workingVector;
    for(int i = 0; i < this->numVertex; i++){
        workingVector = this->vertex[i];
        multiplyMatrixVector(&workingVector, &this->vertex[i], &MatRotY);
    }
}

void Mesh::rotateZ(float theta){
    MatRotZ.m[0][0] = cos(theta);
    MatRotZ.m[0][1] = sin(theta);
    MatRotZ.m[1][0] = -sin(theta);
    MatRotZ.m[1][1] = cos(theta);
    MatRotZ.m[2][2] = 1;
    MatRotZ.m[3][3] = 1;
    Vector3 workingVector;
    for(int i = 0; i < this->numVertex; i++){
        workingVector = this->vertex[i];
        multiplyMatrixVector(&workingVector, &this->vertex[i], &MatRotZ);
    }
}

void Mesh::translate(Vector3 offset){
    for(int i = 0; i < this->numVertex; i++){
        this->vertex[i].x += offset.x;
        this->vertex[i].y += offset.y;
        this->vertex[i].z += offset.z;
    }
}

Vertex *Mesh::findVertex(float x, float y, float z){
  for(int i = 0; i < this->numVertex; i++){
    if(this->vertex[i].x == x && this->vertex[i].y == y && this->vertex[i].z == z){
      return &this->vertex[i];
    }
  }
  return NULL;
}

Vertex *Mesh::matchVertex(Vertex vert){
  for(int i = 0; i < this->numVertex; i++){
    if(this->vertex[i].x == vert.x && this->vertex[i].y == vert.y && this->vertex[i].z == vert.z){
      //print("Match!\n");
      return &this->vertex[i];
    }
  }
  return NULL;
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

struct TriangleExpl triangleIdxToExpl(Mesh *mesh, struct TriangleIdx *tri){
    struct TriangleExpl expl = {0};
    expl.v0 = mesh->vertex[tri->v0];
    expl.v1 = mesh->vertex[tri->v1];
    expl.v2 = mesh->vertex[tri->v2];
    expl.color = tri->color;

    //expl.v0 = {0.0f, 1.0f, -1.0f};
    //expl.v1 = {1.0f, 0.0f, 0.0f};
    //expl.v2 = {0.0f, 0.0f, 0.0f};

    //expl.v0 = mesh->vertex[0];
    //expl.v1 = mesh->vertex[1];
    //expl.v2 = mesh->vertex[2];

    //printTriangle(expl);
    //print_arg("V0 IDX: %d | ", tri->v0);
    //print_arg("V0 ADDR: 0x%x\n", (uint32_t) &mesh->vertex[tri->v0]);
    //print_arg("V1 IDX: %d | ", tri->v1);
    //print_arg("V1 ADDR: 0x%x\n", (uint32_t) &mesh->vertex[tri->v1]);
    //print_arg("V2 IDX: %d | " , tri->v2);
    //print_arg("V2 ADDR: 0x%x\n", (uint32_t) &mesh->vertex[tri->v2]);
    //printTriangle(expl);
    return expl;
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
    //print("Drawing Triangle...\n");
    drawLine((int) triangle.v0.x, (int) triangle.v0.y, (int) triangle.v1.x, (int) triangle.v1.y, color);
    drawLine((int) triangle.v1.x, (int) triangle.v1.y, (int) triangle.v2.x, (int) triangle.v2.y, color);
    drawLine((int) triangle.v2.x, (int) triangle.v2.y, (int) triangle.v0.x, (int) triangle.v0.y, color);
}

void drawLine(int x1, int y1, int x2, int y2, uint32_t color){
  if(!(x1 >= 0 && x1 <= sys.window.window->width && y1 >= 0 && y1 <= sys.window.window->height)){
    return;
  }
  int dx = abs(x2 - x1);
  int sx = x1 < x2 ? 1 : -1;
  int dy = -abs(y2 - y1);
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;
  int e2;

  for(;;){
    if(x1 >= 0 && x1 <= sys.window.window->width && y1 >= 0 && y1 <= sys.window.window->height){
      sys.window.window->backbuffer[x1 + y1 * sys.window.window->width] = color;
    }    
    if(x1 == x2 && y1 == y2) break;
    e2 = 2 * err;
    if(e2 >= dy){err += dy; x1 += sx;}
    if(e2 <= dx){err += dx; y1 += sy;}
  }
}

void printTriangle(TriangleExpl tri){
  print_arg("v0: 0x%x ", ((uint32_t) &tri.v0.x));
  print_arg("0x%x ", ((uint32_t) &tri.v0.y));
  print_arg("0x%x\n", ((uint32_t) &tri.v0.z));
  
  print_arg("v1: 0x%x ", ((uint32_t) &tri.v1.x));
  print_arg("0x%x ", ((uint32_t) &tri.v1.y));
  print_arg("0x%x\n", ((uint32_t) &tri.v1.z));
  
  print_arg("v2: 0x%x ", ((uint32_t) &tri.v2.x));
  print_arg("0x%x ", ((uint32_t) &tri.v2.y));
  print_arg("0x%x\n", ((uint32_t) &tri.v2.z));
}

int calculateZBuffer(World *world, int x, int y, TriangleExpl *projectedTri, TriangleExpl *unProjectedTri){
  //ZBuffType z = (ZBuffType) (calcZ(workTri.v0, workTri.v1, workTri.v2, (float) x, (float) y) * 100);
  ZBuffType z = (ZBuffType) (((unProjectedTri->v0.z + unProjectedTri->v1.z + unProjectedTri->v2.z) / 3) * 100);
  //print_arg("Z: %x\n", z);
  if(z > world->ZBuff[x + y * 640]){
    world->ZBuff[x + y * 640] = z;
    return 1;
  }
  //print("Blocked!\n");
  return 0;
}

void drawHline(World *world, int x1, int x2, int y, uint32_t color, TriangleExpl *projectedTri, TriangleExpl *unProjectedTri){
  //if(x1 < 0) x1 = 0;
  //if(x1 > sys.window->width) x1 = sys.window->width;
  //if(x2 < 0) x2 = 0;
  //if(x2 > sys.window->width) x2 = sys.window->width;
  //if(y < 0 || y >= sys.window->height) return;
  //print_arg("Z0: %d ", (int) (tri->v0.z * 1000));
  //print_arg("Z1: %d ", (int) (tri->v1.z * 1000));
  //print_arg("Z2: %d\n", (int) (tri->v2.z * 1000));
  for(int x = x1; x < x2; x++){
    if(world->ZBuffering){
      if(x < sys.window.window->width && x > 0 && y > 0 && y < sys.window.window->height && calculateZBuffer(world, x, y, projectedTri, unProjectedTri)){
        sys.window.window->backbuffer[x + y*sys.window.window->width] = color;
      }
    }
    else{
      if(x < sys.window.window->width && x > 0 && y > 0 && y < sys.window.window->height){
        sys.window.window->backbuffer[x + y*sys.window.window->width] = color;
      }
    }
  }
}

#define SWAP(x,y) do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)
void fillTriangle(World *world, TriangleExpl *triangle, TriangleExpl *unprojected, uint32_t color){
  int x1, y1, x2, y2, x3, y3;
  x1 = triangle->v0.x;
  y1 = triangle->v0.y;
  x2 = triangle->v1.x;
  y2 = triangle->v1.y;
  x3 = triangle->v2.x;
  y3 = triangle->v2.y;
  //if((x1 > sys.window.window->width || x2 > sys.window.window->width) || (x1 < 0 || x2 < 0) || (y1 > sys.window.window->height || y2 > sys.window.window->height) || (y1 < 0 || y2 < 0)) return;
  //else{
  //if(x1 < 0) x1 = 0;
  //if(x2 < 0) x2 = 0;
  //if(x3 < 0) x3 = 0;
  //if(y1 < 0) y1 = 0;
  //if(y2 < 0) y2 = 0;
  //if(y3 < 0) y3 = 0;
  //if(x1 > sys.window.window->width) x1 =  sys.window.window->width;
  //if(x2 > sys.window.window->width) x2 =  sys.window.window->width;
  //if(x3 > sys.window.window->width) x3 =  sys.window.window->width;
  //if(y1 > sys.window.window->height) y1 = sys.window.window->height;
  //if(y2 > sys.window.window->height) y2 = sys.window.window->height;
  //if(y3 > sys.window.window->height) y3 = sys.window.window->height;
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
  //if(abs(x3 - x1) > sys.window->width) return;  
  
  int maxsteps = 1000;

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
	   	drawHline(world, minx, maxx, y, color, triangle, unprojected);    // Draw line from min to max points found on the y
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
	   	drawHline(world, minx, maxx, y, color, triangle, unprojected);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y>y3) return;
	}
}

bool operator==(Vector3 &a, Vector3 &b){
  if(a.x == b.x && a.y == b.y && a.z == b.z) return true;
  return false;
}