/**
 *  @file   player.h
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  player module
 */

#pragma once

#include <stdint.h>

#include <system.h>
#include <navswitch.h>

#include "defs.h"

/** Updates the player (cursor) position */
void update_player(void);

/** Gets player coordinates
 * @return position vector containing (x, y) coordinates of player
*/
pos_t get_player(void);