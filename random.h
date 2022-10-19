/**
 *  @file   random.h
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  Random variable module, used for generating random ship maps
 */

#pragma once

#include <stdint.h>

static uint8_t _rand = 0;


/** updated random counter */
static inline void update_rand(void)
{
    _rand++; //Increasing value that resets when overflows. Used to create random seed for maps
        if (_rand >= 255){
            _rand = 0;
        }
}

/** Returns random counter
 *  @return random counter variable
*/
static inline uint8_t rand(void)
{
    return _rand;
}