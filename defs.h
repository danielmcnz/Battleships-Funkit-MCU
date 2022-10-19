#pragma once

#include <stdint.h>

#define PACER_RATE 500
#define DISPLAY_RATE 250

// message rate for tinygl scroll text
#define MESSAGE_RATE 10

// speed to flash a pixel when rendering
#define FLASHING_FREQUENCY 200

#define MAP_WIDTH 5
#define MAP_HEIGHT 7

#define MAP_SIZE MAP_WIDTH * MAP_HEIGHT

/** number of possible starting locations for battleships */
#define N_SHIP_MAPS 3


/** position struct for x/y coordinates of a pixel */
typedef struct
{
    /** x coordinate */
    uint8_t x;
    /** y coordinate */
    uint8_t y;
} pos_t;