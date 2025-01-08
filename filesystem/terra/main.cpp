#include "libc.h"
#include "application.hpp"
#include "renderer.hpp"
#include "utilities.hpp"
#include "terrain.hpp"
#include "game.hpp"

System sys;
World world;
Chunk chunk;
Player player;

//Object testObject;
//Object ball;
//Object ship;

void keyboard_handler(unsigned char keycode);
void mouse_handler();

int main(int argc, char **argv){
    print_serial((char *) "Opening Terrain Application\n");
    sys.setup();
    sys.window = Window((char *)"Terra");
    sys.window.clear(0xFF0000);
    sys.window.update();

    set_schedule(ONFOCUS);
    addKbdEventHandler(keyboard_handler);
    //addMouseEventHandler(mouse_handler);
    
    print_arg("Width: %d\n", sys.window.getWidth());
    print_arg("Height: %d\n", sys.window.getHeight());

    world = World(&sys.window, 5);
    //testObject = Object("/A/3D/CUBE.OBJ");
    //ball = Object("/A/3D/BALL.OBJ");
    //ship = Object("/A/3D/SHIP.OBJ");
    //cow = Object("/A/3D/MCCOW.OBJ");
    //Object teapot = Object("/A/3D/TEACUP.OBJ");

    chunk = Chunk();
    print_arg("Blocks are at 0x%x\n", (uint32_t) chunk.getBlocksAddr());
    Terrain terra = Terrain(1000, 1000);

    //world.addObject(&testObject);
    //world.addObject(&ball);
    
    //world.addObject(&ship);

    terra.generateMesh(&chunk);
    terra.getMesh()->translate({-CHUNK_WIDTH/2, 0, -CHUNK_WIDTH/2});
    print("Generated Mesh\n");
    //terra.getObject()->worldPosition.z = -4;
    //terra.getObject()->worldRotation.x = -(PI/4);
    //terra.getObject()->worldRotation.z = -(PI/4);
    //terra.getObject()->worldRotation.y = -0.7f;
    world.addObject(terra.getObject());
    //world.addObject(&cow);

    //world.addObject(&teapot);
    //world.ZBuffering = 0;

    //testObject.worldRotation.z = 0;
    //testObject.worldPosition.x = -1.5f;
    //ship.worldPosition.z = -6;
    //ship.worldPosition.y = -3;
    //ship.worldRotation.x = 0.75f;
    //ball.worldPosition.y = 4;


    player = Player({0, 5.4f, 0}, {0, 0, 0});
    //world.getCamera()->position.y += 5;
    //world.getCamera()->position.z -= 2;
    //world.getCamera()->position.x -= 2;
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
    print_serial("Starting Game\n");
    while(1){
        (*world.getCamera()) = player.getCamera();
        //terra.getMesh()->rotateX(0.02f);
        //terra.getMesh()->rotateY(0.01f);
        //teapot.masterMesh->rotateY(0.1f);
        terra.generateMesh(&chunk);
        terra.getMesh()->translate({-CHUNK_WIDTH/2, 0, -CHUNK_WIDTH/2});
        //terra.getMesh()->rotateY(0.1f);

        //ship.masterMesh->rotateX(0.1f);
        //ball.worldPosition.z -= amt;
        //if(ball.worldPosition.z >= -0.1 || ball.worldPosition.z <= -5.0f) amt *= -1;

        sys.window.clear(0x0000AA);
        world.render();
        sys.window.update();
        
    }
    print((char *)"Exit!\n");
    
    return 0;
}

void mouse_handler(){
    struct MouseStatus *mouse = getMouse();
    player.getRotation()->x -= 0.01f * ((float) mouse->lastDelta.y);
    player.getRotation()->y -= 0.01f * ((float) mouse->lastDelta.x);
    return;
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

        case ' ':{
            struct IntVector3 feet = player.getPlayerFootChunkPosition();
            chunk.blocks[feet.x][feet.y][feet.z-1] = 0;
            break;
        }

        case 'w':
            //world.getCamera()->position.z -= 0.1f;
            player.getPosition()->z -= 0.1f * cos(player.getRotation()->y);
            player.getPosition()->x += 0.1f * sin(player.getRotation()->y);
            break;
        case 's':
            player.getPosition()->z += 0.1f * cos(player.getRotation()->y);
            player.getPosition()->x -= 0.1f * sin(player.getRotation()->y);
            break;
        case 'a':
            player.getPosition()->z -= 0.1f * cos(player.getRotation()->y + (PI/2));
            player.getPosition()->x += 0.1f * sin(player.getRotation()->y + (PI/2));
            break;
        case 'd':
            player.getPosition()->z += 0.1f * cos(player.getRotation()->y + (PI/2));
            player.getPosition()->x -= 0.1f * sin(player.getRotation()->y + (PI/2));    
            break;

        case 0x11://Left Arrow
            player.getRotation()->y += 0.05f;
            break;
        case 0x12://Right Arrow
            player.getRotation()->y -= 0.05f;
            break;
        case 0x13://Up Arrow
            player.getRotation()->x -= 0.05f;
            break;
        case 0x14://Down Arrow
            player.getRotation()->x += 0.05f;
            break;
        case 'X':

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