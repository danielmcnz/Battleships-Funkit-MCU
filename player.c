/**
 *  @file   player.c
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  player module
 */

#include "player.h"

static pos_t player = {0};

void update_player()
{
    int8_t x = (int8_t)player.x;
    int8_t y = (int8_t)player.y;
    
    if(navswitch_push_event_p(NAVSWITCH_NORTH))
    {
        y--;
        if(y < 0)
        {
            y = 6;
        }
    }
    
    if(navswitch_push_event_p(NAVSWITCH_SOUTH))
    {
        y++;
        if(y > 6)
        {
            y = 0;
        }
    }

    if(navswitch_push_event_p(NAVSWITCH_WEST))
    {
        x--;
        if(x < 0)
        {
            x = 4;
        }
    }
    
    if(navswitch_push_event_p(NAVSWITCH_EAST))
    {
        x++;
        if(x > 4)
        {
            x = 0;
        }
    }

    player.x = (uint8_t)x;
    player.y = (uint8_t)y;
}

pos_t get_player(void)
{
    return player;
}