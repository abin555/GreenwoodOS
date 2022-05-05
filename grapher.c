#include "grapher.h"

void draw_regions(){
    gfx_vline(0,fb_height-fb_height/4, fb_width-fb_width/Proportion_Right, 0xFFFFFF);
    gfx_hline(0, fb_width,fb_height-fb_height/Proportion_Bottom, 0xFFFFFF);
}

void draw_axis(){
    fb_setPixel(axis_center_x, axis_center_y, 0xFFFFFF);
    gfx_hline(
        axis_center_x-(fb_width/Axis_Proportion), //Left X
        axis_center_x+(fb_width/Axis_Proportion), //Right X
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
            axis_center_x+(x*(fb_width/Axis_Proportion/settings_data.right_bound)),
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

void plot_point(int x, int y){
    pixelScaled(
        axis_center_x+(x*(fb_width/Axis_Proportion/settings_data.right_bound)),
        axis_center_y+(y*(fb_height/Axis_Proportion/settings_data.top_bound)),
        3,
        0xFF00FF
    );
}

void draw_graph(){
    for(int x = settings_data.left_bound; x <= settings_data.right_bound; x++){
        int y = (x-2)*(x-2)+2;
        if(y > settings_data.bottom_bound && y < settings_data.top_bound){
            plot_point(x, y);
        }
    }
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


    draw_regions();
    draw_axis();
    draw_graph();
    while(1){
        if(keyboard_ASCIIBuffer[keyboard_ascii_pointer-1] == '/'){
            return;
        }
    }
}