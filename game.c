#include <system.h>
#include <navswitch.h>
#include <pacer.h>
#include <tinygl.h>
#include <../fonts/font5x7_1.h>

#include "defs.h"
#include "cursor.h"
#include "renderer.h"


// static uint8_t friendlyShips[] = 
// {
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0,
// };

// static uint8_t friendlyShots[] =
// {
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0,
// };

static uint8_t enemyShots[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

void initialize(void)
{
    system_init();

    tinygl_init(PACER_RATE);
    pacer_init(PACER_RATE);
}

void update(void)
{
    navswitch_update();
    
    update_cursor();
}

void render(void)
{
    tinygl_update();
    draw_clear();

    draw_map(enemyShots);
    draw_flashing_pixel(get_cursor().x, get_cursor().y);
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
