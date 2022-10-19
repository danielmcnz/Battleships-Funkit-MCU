/**
 *  @file   player.c
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  Updates the player postion (Cursor when guessing), and also checks if player has won the game.
 */

#include "player.h"


static pos_t player = {0};

void update_player()
{
    int8_t x = (int8_t)player.x; //Casts to a signed interger
    int8_t y = (int8_t)player.y;
    
    if(navswitch_push_event_p(NAVSWITCH_NORTH)) //Increase/decrease X or Y value. Includes wrap around logic 
    {
        y--;
        if(y < 0)
        {
            y = MAP_HEIGHT -1;
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
            x = MAP_WIDTH -1;
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

    player.x = (uint8_t)x; //Converts back to unsigned ints, which is what game.c is expecting
    player.y = (uint8_t)y;
}


uint8_t has_player_won(uint8_t friendly_ships[], uint8_t enemy_guesses[])
{
    for (uint8_t i=0; i<MAP_WIDTH * MAP_HEIGHT; ++i) //Loops through all positions 
    {
        if (friendly_ships[i] == 1 && enemy_guesses[i] == 0){ //If there are any ship areas that have not been hit, return 0 (Player has not won)
            return 0;
        }

    } 
    return 1; //Return 1 if enemy guesses has all of friendly ships locations
}


pos_t get_player(void) //returns the player position. 
{
    return player;
}