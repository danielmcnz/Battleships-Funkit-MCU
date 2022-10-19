/**
 *  @file   map.c
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  map module and map support functions
 */

#include "map.h"
#include "random.h"


void update_map(pos_t coords, uint8_t Map[], uint8_t RESULT){ //Updates a given pixel position with the RESULT value 
    Map[MAP_HEIGHT * coords.x + (MAP_HEIGHT - 1) - coords.y] = RESULT;
}


uint8_t generate_ships(uint8_t map[35], uint8_t locations[4][35]) //Generates a random map
{
    uint8_t location = 3;// rand() % 3; //Gets the adress of a random map 

    
    for (uint8_t i=0; i<MAP_WIDTH * MAP_HEIGHT; ++i) // fills map
    {
        map[i] = locations[location][i];
    }

    return location;
}


uint8_t get_position_value(uint8_t map[], pos_t position) //returns the value of a pixel at a position 
{
    return map[MAP_HEIGHT * position.x + (MAP_HEIGHT - 1 ) - position.y];
}