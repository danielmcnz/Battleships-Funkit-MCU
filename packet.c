/**
 *  @file   packet.c
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  IR communication packet handler
 */

#include "packet.h"


uint8_t send_coords(packet_t *packet)
{
    static uint8_t recv = 0;

    static uint8_t sent_coords = 0;
    
    if(sent_coords == 0 && ir_uart_write_ready_p())
    {
        ir_uart_putc(packet->coords.x + packet->coords.y * 10);
        sent_coords = 1;
    }

    if(ir_uart_read_ready_p() && sent_coords == 1 && recv == 0)
    {
        packet->result = ir_uart_getc();
        recv = 1;
    }

    if(recv == 1)
    {
        recv = 0;
        sent_coords = 0;
        return 1;
    }

    return 0;
}


uint8_t recv_coords(packet_t *packet, uint8_t friendly_ships[])
{
    static uint8_t recv = 0;

    static uint8_t sent_coords = 0;

    // receive enemy attack coordinates
    if(ir_uart_read_ready_p() && recv == 0)
    {
        uint8_t in_val = ir_uart_getc();
        packet->coords.x = in_val % 10;
        packet->coords.y = (in_val - packet->coords.x) / 10;
        recv = 1;
    }

    // send hit or miss dependent on coordinates
    if (recv == 1 && sent_coords == 0 && ir_uart_write_ready_p()) 
    {
        uint8_t result = friendly_ships[MAP_HEIGHT * packet->coords.x + (MAP_HEIGHT-1)-packet->coords.y];
        if(result == 0)
        {
            result = 2;
        }

        packet->result = result;
        
        ir_uart_putc(result);

        sent_coords = 1;
    }

    if(sent_coords == 1)
    {
        recv = 0;
        sent_coords = 0;
        return 1;
    }

    return 0;
}