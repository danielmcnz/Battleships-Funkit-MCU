#include "cursor.h"

static cursor_t cursor = (cursor_t) {.x=0, .y=0};

void update_cursor()
{
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

const cursor_t get_cursor(void)
{
    return cursor;
}