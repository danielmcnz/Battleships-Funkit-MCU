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


void send_coords(packet_t *packet);

void recv_coords(packet_t *packet, uint8_t friendly_ships[]);