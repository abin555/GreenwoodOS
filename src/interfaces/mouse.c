#include "mouse.h"
#include "multitasking.h"
#include "vfs.h"
#include "sysfs.h"

struct MouseStatus mouseStatus;

void mouse_init(){
    mouseStatus.pos.x = fb_width / 2;
    mouseStatus.pos.y = fb_height / 2;
    mouseStatus.buttons.right = 0;
    mouseStatus.buttons.left = 0;
    mouseStatus.buttons.middle = 0;
}

int mouseTxt_read(void *dev, void *buf, int roffset, int nbytes, int *head){
    struct SysFS_Chardev *mousedev = dev;
    int n = snprintf(
        mousedev->buf,
        mousedev->buf_size,
        "X: %d\nY: %d\n",
        mouseStatus.pos.x,
        mouseStatus.pos.y
    );
    mousedev->buf[n] = '\0';
    int step = roffset;
    int i;
    for(i = 0; i < nbytes && step < mousedev->buf_size && i < n; i++){
        ((char *) buf)[i] = mousedev->buf[step++];
    }
    //print_serial("Wrote %d bytes with snprintf of %d bytes at offset %d\n", i, n, roffset);
    *head += i;
    return i;
}

void *mouseTxt_createCDEV(){
    print_serial("[Mouse] Generating Text CDEV\n");
    struct SysFS_Chardev *mouse_cdev = sysfs_createCharDevice(
        malloc(20),
        20,
        CDEV_READ
    );
    sysfs_setCallbacks(
        mouse_cdev,
        NULL,
        NULL,
        NULL,
        mouseTxt_read
    );
    return (void *) mouse_cdev;
}

void *mouse_createCDEV(){
    print_serial("[Mouse] Generating CDEV\n");
    struct SysFS_Chardev *mouse_cdev = sysfs_createCharDevice(
        (char *) &mouseStatus,
        sizeof(struct MouseStatus),
        CDEV_READ
    );
    return (void *) mouse_cdev;
}

void mouse_update(int deltaX, int deltaY, struct MouseButtons btns){
    mouseStatus.pos.x += deltaX;
    mouseStatus.pos.y -= deltaY;
    mouseStatus.buttons = btns;
    if(mouseStatus.pos.x < 0) mouseStatus.pos.x = 0;
    else if(mouseStatus.pos.x > (int) fb_width) mouseStatus.pos.x = fb_width - 1;

    if(mouseStatus.pos.y < 0) mouseStatus.pos.y = 0;
    else if(mouseStatus.pos.y > (int) fb_height) mouseStatus.pos.y = fb_height - 1;

    mouseStatus.lastDelta.x = deltaX;
    mouseStatus.lastDelta.y = deltaY;
    //mouse_callEventHandler();
}

struct IVec2 mouse_getPos(){
    return mouseStatus.pos;
}