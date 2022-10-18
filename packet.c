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
    static uint8_t updated = 0;

    // receive enemy attack coordinates
    if(ir_uart_read_ready_p() && updated != 1)
    {
        packet->coords.x = ir_uart_getc();
        updated = 1;
    }
    if(ir_uart_read_ready_p() && updated == 1)
    {
        packet->coords.y = ir_uart_getc();
        recv = 1;
        updated = 0;
    }

    // send hit or miss dependent on coordinates
    if (recv == 1) 
    {
        uint8_t result = friendly_ships[MAP_HEIGHT * packet->coords.x + (MAP_HEIGHT-1)-packet->coords.y];
        if(result == 0)
            result = 2;

        packet->result = result;
        
        ir_uart_putc(result);
    }
    
    
    return recv;
}