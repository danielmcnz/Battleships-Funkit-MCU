#pragma once

#include <stdint.h>

#include <system.h>
#include <navswitch.h>

typedef struct
{
    uint8_t x;
    uint8_t y;
} player_t;

void update_player(void);

player_t get_player(void);