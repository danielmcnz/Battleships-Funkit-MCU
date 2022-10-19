#pragma once

#define PACER_RATE 800
#define DISPLAY_RATE 600

#define MESSAGE_RATE 10

#define FLASHING_RATE PACER_RATE / 2
#define DIMMED_FREQUENCY 3

#define MAP_WIDTH 5
#define MAP_HEIGHT 7

#define MAP_SIZE MAP_WIDTH * MAP_HEIGHT

#define BATTLESHIPS {2, 2, 3, 4}
#define MAX_SHIPS 4

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