#pragma once

#include <tinygl.h>

#include "defs.h"

void draw_flashing_pixel(uint8_t x, uint8_t y);
void draw_dimmed_pixel(uint8_t x, uint8_t y);
void draw_pixel(uint8_t x, uint8_t y);
void draw_map(uint8_t map[35]);
void draw_clear(void);