/**
 *  @file   map.h
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  map module and map support functions
 */

#pragma once


#include <stdint.h>

#include "defs.h"


/** @todo this */
// void map_init(void);


/** Updates a map 
 *  @param coords X/Y coordernate struct, 
 *  @param Map Map that needs to be updated
 *  @param RESULT The value to be written (0 = No guess/off, 1 = Hit/Fully on, 2 = Miss/Dimmed)
*/
void update_map(pos_t coords, uint8_t Map[], uint8_t RESULT);


/** Generate random locations for friendly ships
 *  @param map friendly ships map
*/
void generate_ships(uint8_t map[35]);


/** Returns value in the given map of the given x and y position
 *  @param map uint8_t array of pixels
 *  @param position x and y coordinates of the wanted pixel value
 *  @return value of the pixel at the given x and y position
 */
uint8_t get_position_value(uint8_t map[], pos_t position);