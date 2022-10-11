#include "player.h"

static player_t player = (player_t) {.x=0, .y=0};

void update_player()
{
    /* TODO: Increment character if NORTH is pressed.  */
    if(navswitch_push_event_p(NAVSWITCH_NORTH))
    {
        player.y--;
        if(player.y < 0)
            player.y = 6;
    }
    
    /* TODO: Decrement character if SOUTH is pressed.  */
    if(navswitch_push_event_p(NAVSWITCH_SOUTH))
    {
        player.y++;
        if(player.y > 6)
            player.y = 0;
    }

    if(navswitch_push_event_p(NAVSWITCH_WEST))
    {
        player.x--;
        if(player.x < 0)
            player.x = 4;
    }
    
    /* TODO: Decrement character if SOUTH is pressed.  */
    if(navswitch_push_event_p(NAVSWITCH_EAST))
    {
        player.x++;
        if(player.x > 4)
            player.x = 0;
    }
}

player_t get_player(void)
{
    return player;
}