/**
 * @param
*/

#pragma once

#include <stdint.h>

static uint8_t _rand = 0;


static inline void update_rand(void)
{
    _rand++; //Increasing value that resets when overflows. Used to create random seed for maps
        if (_rand >= 255){
            _rand = 0;
        }
}

static inline uint8_t rand(void)
{
    return _rand;
}