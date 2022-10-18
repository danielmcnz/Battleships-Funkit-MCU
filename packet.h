/**
 *  @file   packet.h
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  IR communication packet handler
 */

#pragma once

#include <stdint.h>

#include <ir_uart.h>

#include "defs.h"

typedef struct
{
    uint8_t x;
    uint8_t y;
} coords_t;

typedef struct
{
    uint8_t result; // whether attack/defend was a hit or miss
    coords_t coords;
} packet_t;


/** Handles ir communications in attack mode
 *  @param packet ir communication packet for sending and receiving data
 *  @return returns 1 when finished communications and filled the packet, otherwise 0
*/
uint8_t send_coords(packet_t *packet);


/** Handles ir communication in defend mode
 *  @param packet ir communication packet for sending and receiving data
 *  @param friendly_ships map of friendly ships
 *  @return returns 1 when finished communications and filled the packet, otherwise 0
*/
uint8_t recv_coords(packet_t *packet, uint8_t friendly_ships[]);