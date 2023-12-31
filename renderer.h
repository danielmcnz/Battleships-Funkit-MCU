/**
 *  @file   renderer.h
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  renderer support for drawing pixels and map arrays
 */

#pragma once

#include <tinygl.h>

#include <font3x5_1.h>

#include "defs.h"
#include "map.h"


/** initializes renderer module */
void renderer_init(void);


/** Draws a flashing pixel
 *  @param frequency frequency to flash the pixel
 *  @param x x coordinate on the map
 *  @param y y coordinate on the map
*/
void draw_flashing_pixel(uint16_t *time, uint8_t x, uint8_t y);


/** Draws a pixel
 *  @param x x coordinate on the map
 *  @param y y coordinate on the map
*/
void draw_pixel(uint8_t x, uint8_t y);


/** Draws the given map array onto the board
 *  @param map array of pixel values to draw
*/
void draw_map(uint16_t *time, uint8_t map[]);


/** Clears the pixels on the board */
void draw_clear(void);