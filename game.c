#include <system.h>
#include <navswitch.h>
#include <pacer.h>
#include <tinygl.h>
#include <../fonts/font5x7_1.h>

#include "defs.h"
#include "cursor.h"
#include "renderer.h"


static uint8_t friendlyShips[] = 
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static uint8_t friendlyShots[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static uint8_t enemyShots[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

cursor_t cursor = {.x=3, .y=1};

void initialize(void)
{
    system_init();

    tinygl_init(PACER_RATE);
    pacer_init(PACER_RATE);
}

void update(void)
{
    navswitch_update();
        
    /* TODO: Increment character if NORTH is pressed.  */
    if(navswitch_push_event_p(NAVSWITCH_NORTH))
    {
        cursor.y--;
        if(cursor.y < 0)
            cursor.y = 6;
    }
    
    /* TODO: Decrement character if SOUTH is pressed.  */
    if(navswitch_push_event_p(NAVSWITCH_SOUTH))
    {
        cursor.y++;
        if(cursor.y > 6)
            cursor.y = 0;
    }

    if(navswitch_push_event_p(NAVSWITCH_WEST))
    {
        cursor.x--;
        if(cursor.x < 0)
            cursor.x = 4;
    }
    
    /* TODO: Decrement character if SOUTH is pressed.  */
    if(navswitch_push_event_p(NAVSWITCH_EAST))
    {
        cursor.x++;
        if(cursor.x > 4)
            cursor.x = 0;
    }
}

void render(void)
{
    tinygl_update();
    draw_clear();

    draw_map(enemyShots);
    draw_flashing_pixel(cursor.x, cursor.y);
}

int main (void)
{ 
    initialize();

    while (1)
    {
        pacer_wait ();

        render();     
        update(); 
    }

    return 0;
}
