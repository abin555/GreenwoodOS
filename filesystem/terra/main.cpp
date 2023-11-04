#include "libc.h"
#include "application.hpp"
#include "renderer.hpp"
#include "utilities.hpp"
#include "terrain.hpp"


System sys;
World world;
Chunk chunk;

Object testObject;
Object ball;
Object ship;

void keyboard_handler(unsigned char keycode);

int main(int argc, char **argv){
    print((char *) "Opening Terrain Application\n");
    sys.setup();
    sys.window = Window((char *)"Terra");
    sys.window.clear(0xFF0000);
    sys.window.update();
    set_schedule(ONFOCUS);
    addKbdEventHandler(keyboard_handler);
    
    print_arg("Width: %d\n", sys.window.getWidth());
    print_arg("Height: %d\n", sys.window.getHeight());

    world = World(&sys.window, 5);
    testObject = Object("/A/3D/CUBE.OBJ");
    ball = Object("/A/3D/BALL.OBJ");
    ship = Object("/A/3D/SHIP.OBJ");
    //Object teapot = Object("/A/3D/TEACUP.OBJ");

    chunk = Chunk();
    print_arg("Blocks are at 0x%x\n", (uint32_t) chunk.getBlocksAddr());
    Terrain terra = Terrain(1000, 1000);

    //world.addObject(&testObject);
    world.addObject(&ball);
    //world.addObject(&ship);

    terra.generateMesh(&chunk);
    terra.getMesh()->translate({-CHUNK_WIDTH/2, 0, -CHUNK_WIDTH/2});
    //terra.getObject()->worldPosition.z = -4;
    //terra.getObject()->worldRotation.x = -(PI/4);
    //terra.getObject()->worldRotation.z = -(PI/4);
    //terra.getObject()->worldRotation.y = -0.7f;
    world.addObject(terra.getObject());

    //world.addObject(&teapot);
    //world.ZBuffering = 0;

    testObject.worldRotation.z = 0;
    testObject.worldPosition.x = -1.5f;
    ship.worldPosition.z = -6;
    ship.worldPosition.y = -3;
    ship.worldRotation.x = 0.75f;
    ball.worldPosition.y = 4;

    world.getCamera()->position.y += 5;
    world.getCamera()->position.z -= 2;
    world.getCamera()->position.x -= 2;
    //terra.getObject()->worldPosition.z = -12;
    //terra.getObject()->worldPosition.y = -5;
    //teapot.worldPosition.z = -10;
    //teapot.worldPosition.y = -2;

    //ball.worldPosition.x = -1;
    //ball.worldPosition.y = -1;
    //testObject.masterMesh->rotateZ(1.5f);

    //testObject.transform();
    //world.render();
    //sys.window.update();
    
    char c;
    float amt = 0.1;
    while(1){
        //terra.getMesh()->rotateX(0.02f);
        //terra.getMesh()->rotateY(0.01f);
        //teapot.masterMesh->rotateY(0.1f);
        terra.generateMesh(&chunk);
        terra.getMesh()->translate({-CHUNK_WIDTH/2, 0, -CHUNK_WIDTH/2});
        //terra.getMesh()->rotateY(0.1f);

        //ship.masterMesh->rotateX(0.1f);
        ball.worldPosition.z -= amt;
        if(ball.worldPosition.z >= -0.1 || ball.worldPosition.z <= -5.0f) amt *= -1;

        sys.window.clear(0);
        world.render();
        sys.window.update();
        
    }
    print((char *)"Exit!\n");
    
    return 0;
}

void keyboard_handler(unsigned char keycode){
    //print_arg("Keyboard %x\n", keycode);
    switch(keycode){
        case 'z':
            world.ZBuffering = !world.ZBuffering;
            break;
        case 'x':
            world.FillFaces = !world.FillFaces;
            break;
        case 'c':
            world.DrawOutlines = !world.DrawOutlines;
            break;


        case 'w':
            world.getCamera()->position.z -= 0.1f;
            break;
        case 's':
            world.getCamera()->position.z += 0.1f;
            break;
        case 'a':
            world.getCamera()->position.x += 0.1f;
            break;
        case 'd':
            world.getCamera()->position.x -= 0.1f;
            break;

        case 0x11://Left Arrow
            world.getCamera()->rotation.y += 0.05f;
            break;
        case 0x12://Right Arrow
            world.getCamera()->rotation.y -= 0.05f;
            break;
        case 0x13://Up Arrow
            world.getCamera()->rotation.x -= 0.05f;
            break;
        case 0x14://Down Arrow
            world.getCamera()->rotation.x += 0.05f;
            break;
    }
}

void System::setup(){
    this->memory_region = requestRegion(0xD00000);
    this->nextAlloc = this->memory_region;
    print_arg("Memory Region is 0x%x\n", (uint32_t) this->memory_region);
}

void *System::malloc(unsigned int size){
    void *addr = (void *) this->nextAlloc;
    this->nextAlloc = (void *) ((unsigned int) this->nextAlloc + size);
    //print_arg("Malloc Addr: 0x%x\n", (uint32_t) nextAlloc);
    return addr;
}