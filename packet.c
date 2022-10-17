#include "packet.h"

static uint8_t recv = 0;

uint8_t send_coords(packet_t *packet)
{
    recv = 0;

    ir_uart_putc(packet->coords.x);
    ir_uart_putc(packet->coords.y);

    if(ir_uart_read_ready_p())
    {
        packet->result = ir_uart_getc();
        recv = 1;
    }

    return recv;
}

uint8_t recv_coords(packet_t *packet, uint8_t friendly_ships[])
{
    recv = 0;

    // receive enemy attack coordinates
    if(ir_uart_read_ready_p())
    {
        packet->coords.x = ir_uart_getc();
        recv = 1;
    }
    if(ir_uart_write_ready_p())
    {
        packet->coords.y = ir_uart_getc();
        recv = 1;
    }

    // send hit or miss dependent on coordinates
    for(uint8_t x=0;x<MAP_WIDTH; ++x)
    {
        for(uint8_t y=0; y<MAP_HEIGHT; ++y)
        {
            uint8_t result = friendly_ships[MAP_HEIGHT * x + (MAP_HEIGHT-1)-y];
            if(result == 0)
                result = 2;
            packet->result = result;
            ir_uart_putc(result);
        }
    }

    return recv;
}