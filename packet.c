#include "packet.h"

void send_coords(packet_t *packet)
{
    uint8_t result;

    ir_uart_putc(packet->coords.x);
    ir_uart_putc(packet->coords.y);

    packet->result = ir_uart_getc();
}

void recv_coords(packet_t *packet, uint8_t friendly_ships[])
{
    // receive enemy attack coordinates
    packet->coords.x = ir_uart_getc();
    packet->coords.y = ir_uart_getc();

    // send hit or miss dependent on coordinates
    for(uint8_t x=0;x<MAP_WIDTH; ++x)
    {
        for(uint8_t y=0; y<MAP_HEIGHT; ++y)
        {
            uint8_t result = friendly_ships[MAP_HEIGHT * x + (MAP_HEIGHT-1)-y];
            packet->result = result;
            ir_uart_putc(result);
        }
    }
}