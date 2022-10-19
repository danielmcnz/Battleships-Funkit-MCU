/**
 *  @file   player.h
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  Updates the player postion (Cursor when guessing), and also checks if player has won the game.
 */

#pragma once

#include <stdint.h>

#include <system.h>
#include <navswitch.h>

#include "defs.h"


/** Updates the player (cursor) position */
void update_player(void);


/** Checks if there are any ships without being fully hit. 
 *  @param  friendly_ships array map of friendly ship locations
 *  @param  enemy_guesses array map of all guesses from the opposing player
 *  @return 1 if enemy guesses fully encapulates friendly ships, else 0
 */
uint8_t has_player_won(uint8_t friendly_ships[], uint8_t enemy_guesses[]);


/** Gets player coordinates
 * @return position vector containing (x, y) coordinates of player
*/
pos_t get_player(void);