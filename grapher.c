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
            
        )
    }
}

void draw_graph(){
    for(int x = settings_data.left_bound; x <= settings_data.right_bound; x++){
        fb_setPixel(
            axis_center_x+settings_data.xscale*x,
            settings_data.yscale*((x+10)*x-2)+axis_center_y,
            0xFF00FF
        );
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

    draw_regions();
    draw_axis();
    draw_graph();
    while(1){
    }
}