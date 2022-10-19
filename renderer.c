/**
 *  @file   renderer.c
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  renderer support for drawing pixels and map arrays
 */

#include "renderer.h"


void renderer_init(void)
{
    tinygl_init(DISPLAY_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_speed_set(MESSAGE_RATE);
}


void draw_flashing_pixel(uint16_t frequency, uint8_t x, uint8_t y)
{

    static uint16_t time = 0;

    tinygl_point_t point = {.x=x, .y=y};
    if(time >= frequency / 2)
    {
        tinygl_draw_point(point, HIGH);
        if(time == frequency)
            time = 0;
    } 
    else 
    {
        tinygl_draw_point(point, LOW);
    }

    ++time;
}


void draw_pixel(uint8_t x, uint8_t y)
{
    tinygl_draw_point((tinygl_point_t){.x=x, .y=y}, HIGH);
}


void draw_map(uint8_t map[])
{
    for(uint8_t x=0; x<MAP_WIDTH; ++x)
    {
        for(uint8_t y=0; y<MAP_HEIGHT; ++y)
        {
            switch(get_position_value(map, (pos_t){.x=x, .y=y}))
            {
                case HIT:
                    draw_pixel(x, y);
                    break;
                case MISS:
                    draw_flashing_pixel(MISS_FREQUENCY, x, y);
                    break;
            }
        }
    }
}


void draw_clear(void)
{
    tinygl_clear();
}