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

void update_player(void);

pos_t get_player(void);