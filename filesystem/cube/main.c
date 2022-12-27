#include "../libc/libc.h"
#include <math.h>

void draw_line(int x1, int y1, int x2, int y2, unsigned int color);
void put_pixel(int x, int y, unsigned int color);

struct point{
    int X;
    int Y;
    int Z;
};

struct edge{
    struct point* point1;
    struct point* point2;
};

void create_point(struct point* point, int x, int y, int z);
void create_edge(struct edge* edge, struct point* point1, struct point* point2);
void draw_edge(struct edge* edge);

unsigned int *fb;
struct display Display;


int move_x = 0;
int move_y = 0;
float scale = 1;
struct point point_list[8];
struct edge edge_list[12];
double anglez = 0.01;
int PIVX = 50;
int PIVY = 50;

int main(){
    toggle_console();
    set_backbuffer(1);
    Display = get_display();
    fb = getFramebuffer();
    get_keycode();

    //point_list = (struct point**) malloc(8 * sizeof(struct point));
    //edge_list = (struct edge**) malloc(12 * sizeof(struct edge));

    create_point(&point_list[0], 0,0,0);
    create_point(&point_list[1], 100,0,0);
    create_point(&point_list[2], 0,100,0);
    create_point(&point_list[3], 100,100,0);

    create_edge(&edge_list[0], &point_list[0], &point_list[1]);
    create_edge(&edge_list[1], &point_list[2], &point_list[3]);
    create_edge(&edge_list[2], &point_list[2], &point_list[0]);
    create_edge(&edge_list[3], &point_list[3], &point_list[1]);

    /*
    create_point(&point_list[4], 0,0,50);
    create_point(&point_list[5], 100,0,50);
    create_point(&point_list[6], 0,100,50);
    create_point(&point_list[7], 100,100,50);
    

    create_edge(&edge_list[0], &point_list[0], &point_list[1]);
    create_edge(&edge_list[1], &point_list[2], &point_list[3]);
    create_edge(&edge_list[2], &point_list[2], &point_list[0]);
    create_edge(&edge_list[3], &point_list[3], &point_list[1]);
    
    create_edge(&edge_list[4], &point_list[4], &point_list[5]);
    create_edge(&edge_list[5], &point_list[6], &point_list[7]);
    create_edge(&edge_list[6], &point_list[6], &point_list[4]);
    create_edge(&edge_list[7], &point_list[7], &point_list[5]);

    create_edge(&edge_list[8], &point_list[0], &point_list[4]);
    create_edge(&edge_list[9], &point_list[1], &point_list[5]);
    create_edge(&edge_list[10], &point_list[2], &point_list[6]);
    create_edge(&edge_list[11], &point_list[3], &point_list[7]);
    */

    char run = 1;
    while(run){
        anglez+=0.01;
        if(anglez >= (3.1415/2)) anglez = 0.01;
        clear_screen(0);
        for(int i = 0; i < 4; i++){
            draw_edge(&edge_list[i]);
        }
        put_pixel(PIVX+move_x, PIVY+move_y, 0xFF0000);

        switch((uint8_t) get_keycode()){
            case 0x81://Escape
                run = 0;
                break;
            case 0x4B://Left Arrow
                move_x-=5;
                break;
            case 0x4D://Right Arrow
                move_x+=5;
                break;
            case 0x48://Up Arrow
                move_y-=5;
                break;
            case 0x50://Down Arrow
                move_y+=5;
                break;
            case 0x1A://[
                scale *= 1.02;
                break;
            case 0x1B://]
                scale /= 1.02;
                break;
        }
        switch(get_key()){
            case '1':
                anglez -= .01;
                break;
            case '2':
                anglez += .01;
                break;
        }
        swap_buffers();
    }    
    clear_screen(0);
    swap_buffers();
    set_backbuffer(0);
    toggle_console();
}

void create_point(struct point* point, int x, int y, int z){
    point->X = x;
    point->Y = y;
    point->Z = z;
}

void create_edge(struct edge* edge, struct point* point1, struct point* point2){
    edge->point1 = point1;
    edge->point2 = point2;
}

void draw_edge(struct edge* edge){
    int XD_1 = edge->point1->X - PIVX;
    int YD_1 = edge->point1->Y - PIVY;
    double X_1_ROTOFFSET = XD_1*cos(anglez) - YD_1*sin(anglez) - XD_1;
    double Y_1_ROTOFFSET = XD_1*sin(anglez) + YD_1*cos(anglez) - YD_1;

    int XD_2 = edge->point2->X - PIVX;
    int YD_2 = edge->point2->Y - PIVY;
    double X_2_ROTOFFSET = XD_2*cos(anglez) - YD_2*sin(anglez) - XD_2;
    double Y_2_ROTOFFSET = XD_2*sin(anglez) + YD_2*cos(anglez) - YD_2;

    draw_line(
        ((edge->point1->X + X_1_ROTOFFSET) / scale) + move_x,
        ((edge->point1->Y + Y_1_ROTOFFSET) / scale) + move_y,
        ((edge->point2->X + X_2_ROTOFFSET) / scale) + move_x,
        ((edge->point2->Y + Y_2_ROTOFFSET) / scale) + move_y,
        0x00FF00
    );
}

void put_pixel(int x, int y, unsigned int color){
    if(x < 0 || x > Display.width || y < 0 || y > Display.height) return;
    fb[Display.width*y+x] = color;
}

float abs(float value){
    if(value < 0){
        return value*-1;
    }
    else{
        return value;
    }
}

void draw_line(int x1, int y1, int x2, int y2, unsigned int color){
    float x;
    float y;
    float dx;
    float dy;
    float step;
    int i;
    int x_switch = 1;
    int y_switch = 1;
    if(x2 < x1){
        x_switch = -1;
    }
    if(y2 < y1){
        y_switch = -1;
    }

    dx = abs(x2-x1);
    dy = abs(y2-y1);

    if(dx>=dy)
        step=dx;
    else
        step=dy;
    
    dx = dx/step;
    dy = dy/step;

    x = x1;
    y = y1;
    i = 1;
    while(i <= step){
        put_pixel(x,y, color);
        x+=x_switch*dx;
        y+=y_switch*dy;
        i+=1;
    }
}