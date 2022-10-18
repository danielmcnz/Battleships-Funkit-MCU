#pragma once

#define PACER_RATE 900
#define DISPLAY_RATE 450

#define MESSAGE_RATE 10

#define FLASHING_RATE PACER_RATE / 2
#define DIMMED_FREQUENCY 3

#define MAP_WIDTH 5
#define MAP_HEIGHT 7

#define MAP_SIZE MAP_WIDTH * MAP_HEIGHT

#define BATTLESHIPS {2, 2, 3, 4}
#define MAX_SHIPS 4


typedef struct
{
    uint8_t x;
    uint8_t y;
} pos_t;