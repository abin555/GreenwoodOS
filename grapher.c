#include "grapher.h"

void draw_regions(){
    gfx_vline(0,fb_height-fb_height/4, fb_width-fb_width/Proportion_Right, 0xFFFFFF);
    gfx_hline(0, fb_width,fb_height-fb_height/Proportion_Bottom, 0xFFFFFF);
}

void draw_axis(){
    fb_setPixel(axis_center_x, axis_center_y, 0xFFFFFF);
    gfx_hline(
        axis_center_x-(fb_height/Axis_Proportion), //Left X
        axis_center_x+(fb_height/Axis_Proportion), //Right X
        axis_center_y, //Y
        0xFFFFFF);//Color
    gfx_vline(
        axis_center_y-(fb_height/Axis_Proportion),
        axis_center_y+(fb_height/Axis_Proportion),
        axis_center_x,
        0xFFFFFF
    );
    for(int x = settings_data.left_bound; x <= settings_data.right_bound; x++){
        gfx_vline(
            axis_center_y-10,
            axis_center_y+10,
            axis_center_x+(x*(fb_height/Axis_Proportion/settings_data.right_bound)),
            0xFFFFFF
        );
    }
    for(int y = settings_data.bottom_bound; y <= settings_data.top_bound; y++){
        gfx_hline(
            axis_center_x-10,
            axis_center_x+10,
            axis_center_y+(y*(fb_height/Axis_Proportion/settings_data.top_bound)),
            0xFFFFFF
        );
    }
}

void plot_point(float x, float y){
    if(settings_data.settings & 0b00000001){
        pixelScaled(
            axis_center_x+(x*(fb_height/Axis_Proportion/settings_data.right_bound)),
            axis_center_y+((-1 * y)*(fb_height/Axis_Proportion/settings_data.top_bound)),
            3,
            0xFF00FF
        );
    }
    else{
        fb_setPixel(
            axis_center_x+(x*(fb_height/Axis_Proportion/settings_data.right_bound)),
            axis_center_y+((-1 * y)*(fb_height/Axis_Proportion/settings_data.top_bound)),
            0xFF00FF
        );
    }
}

void draw_graph(){
    for(float x = settings_data.left_bound; x <= settings_data.right_bound; x += settings_data.step){
        //float y = x*x*(0.5 * x);
        //float y = 1/sqrt(-1*x*x+2);
        float y;

        y = x+((-1*x*x*x)/6)+((x*x*x*x*x)/120);

        if(y > settings_data.bottom_bound && y < settings_data.top_bound){
            plot_point(x, y);
        }
    }
}

void clear_region(){
    
}

void grapher_entry(){
    fb_clear(0);
    axis_center_x = (fb_width-fb_width/Proportion_Right)/2;
    axis_center_y = (fb_height-fb_height/Proportion_Bottom)/2;
    settings_data.xscale = 1;
    settings_data.yscale = 1;
    settings_data.left_bound = -10;
    settings_data.right_bound = 10;
    settings_data.top_bound = 10;
    settings_data.bottom_bound = -10;
    settings_data.settings |= 0b00000000;
    settings_data.step = 0.0001;

    draw_regions();
    draw_axis();
    draw_graph();
    while(1){
        if(keyboard_ASCIIBuffer[keyboard_ascii_pointer-1] == '/'){
            return;
        }
    }
}

float sqrt(float x){
    float xhalf = 0.5f * x;
    int i = *(int*)&x;            // store floating-point bits in integer
    i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
    x = *(float*)&i;              // convert new bits into float
    x = x*(1.5f - xhalf*x*x);     // One round of Newton's method
    return x;
}