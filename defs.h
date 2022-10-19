#pragma once

#include <stdint.h>

#define PACER_RATE 800
#define DISPLAY_RATE 400

#define MESSAGE_RATE 10

#define CURSOR_FREQUENCY (uint16_t)(PACER_RATE / 2)
#define MISS_FREQUENCY (uint16_t)(CURSOR_FREQUENCY / 2)

#define MAP_WIDTH 5
#define MAP_HEIGHT 7

#define MAP_SIZE MAP_WIDTH * MAP_HEIGHT

#define BATTLESHIPS {2, 2, 3, 4}
#define MAX_SHIPS 4

/** number of possible starting locations for battleships */
#define N_SHIP_MAPS 3

#define HIGH 1
#define LOW 0
#define HIT 1
#define MISS 2


/** position struct for x/y coordinates of a pixel */
typedef struct
{
    /** x coordinate */
    uint8_t x;
    /** y coordinate */
    uint8_t y;
} pos_t;