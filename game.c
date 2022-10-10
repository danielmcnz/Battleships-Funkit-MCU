#include "system.h"
#include "navswitch.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


// #define BUTTON_1 PD7_PIO
// #define BUTTON_IN PB7_PIO
// #define BUTTON_LEFT PC4_PIO
// #define BUTTON_RIGHT PC5_PIO
// #define BUTTON_UP PC6_PIO
// #define BUTTON_DOWN PC7_PIO

/** Define PIO pins driving LED matrix rows.  */
// static const pio_t rows[] =
// {
//     LEDMAT_ROW7_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW5_PIO, 
//     LEDMAT_ROW4_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW2_PIO,
//     LEDMAT_ROW1_PIO
// };


// /** Define PIO pins driving LED matrix columns.  */
// static const pio_t cols[] =
// {
//     LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
//     LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
// };

// static const uint8_t nRows = 7;
// static const uint8_t nCols = 5;

// top left of current camera position
// static const uint8_t x = 1;
// static const uint8_t y = 0;

static const uint8_t friendlyShips[] = 
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static const uint8_t friendlyShots[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static const uint8_t enemyShots[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

typedef struct cursor_pos
{
    int8_t x;
    int8_t y;
    
    int8_t prev_x;
    int8_t prev_y;

} cursor_pos_t;

cursor_pos_t cursor = {.x=3, .y=3};

// static void init_led_matrix(void)
// {
//     /* Initialise LED matrix pins.  */
//     uint8_t j=0;
//     for(uint8_t i=0;i<7;++i)
//     {
//         pio_config_set(rows[i], PIO_OUTPUT_HIGH);
//         if(j < 5)
//             pio_config_set(cols[j], PIO_OUTPUT_HIGH);
//         ++j;
//     }
// }

// static void init_input(void)
// {
//     pio_config_set(BUTTON_1, PIO_INPUT);
//     pio_config_set(BUTTON_IN, PIO_INPUT);
//     pio_config_set(BUTTON_LEFT, PIO_INPUT);
//     pio_config_set(BUTTON_RIGHT, PIO_INPUT);
//     pio_config_set(BUTTON_UP, PIO_INPUT);
//     pio_config_set(BUTTON_DOWN, PIO_INPUT);
// }

void initialize(void)
{
    system_init();

    tinygl_init(500);
    pacer_init(500);

    // init_led_matrix();
    // init_input();

}

void update(void)
{
    static uint8_t time = 0;
    static uint16_t time2 = 0;

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

    // tinygl_point_t point = {.x=1, .y=0};
    // tinygl_draw_point(point, 1);

    // if(time == 40)
    // {
    //     tinygl_point_t point = {.x=0, .y=0};
    //     tinygl_draw_point(point, 1);
    //     time = 0;
    // } else {
    //     tinygl_point_t point = {.x=0, .y=0};
    //     tinygl_draw_point(point, 0);
    // }

    if(time2 >= 250)
    {
        tinygl_point_t point = {.x=cursor.x, .y=cursor.y};
        tinygl_draw_point(point, 1);
        if(time2 == 500)
            time2 = 0;
    } else {
        tinygl_point_t point = {.x=cursor.x, .y=cursor.y};
        tinygl_draw_point(point, 0);
    }

    if(cursor.x != cursor.prev_x || cursor.y != cursor.prev_y)
        tinygl_draw_point((tinygl_point_t){.x=cursor.prev_x, .y=cursor.prev_y}, 0);

    tinygl_update();
    ++time;
    ++time2;

    cursor.prev_x = cursor.x;
    cursor.prev_y = cursor.y;
}

int main (void)
{ 
    // initialize();
    system_init();

    tinygl_init(1000);
    pacer_init(1000);

    while (1)
    {
        pacer_wait ();

        update();      
    }

    return 0;
}
