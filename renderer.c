#include "renderer.h"

void draw_flashing_pixel(uint8_t x, uint8_t y)
{
    static uint16_t time = 0;

    tinygl_point_t point = {.x=x, .y=y};
    if(time >= FLASHING_RATE/2)
    {
        tinygl_draw_point(point, 1);
        if(time == FLASHING_RATE)
            time = 0;
    } else {
        tinygl_draw_point(point, 0);
    }

    ++time;
}

void draw_dimmed_pixel(uint8_t x, uint8_t y)
{
    static uint8_t time = 0;

    tinygl_point_t point = {.x=x, .y=y};
    if(time >= 9)
    {
        tinygl_draw_point(point, 1);
        time = 0;
    } else {
        tinygl_draw_point(point, 0);
    }

    ++time;
}

void draw_pixel(uint8_t x, uint8_t y)
{
    tinygl_draw_point((tinygl_point_t){.x=x, .y=y}, 1);
}

void draw_map(uint8_t map[35])
{
    for(uint8_t x=0; x<MAP_WIDTH; ++x)
    {
        for(uint8_t y=0; y<MAP_HEIGHT; ++y)
        {
            switch(map[MAP_HEIGHT * x + (MAP_HEIGHT-1)-y])
            {
                case 1:
                    draw_pixel(x, y);
                    break;
                case 2:
                    draw_dimmed_pixel(x, y);
                    break;
            }
        }
    }
}

void draw_clear(void)
{
    tinygl_clear();
}