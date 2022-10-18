/**
 * @todo this
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