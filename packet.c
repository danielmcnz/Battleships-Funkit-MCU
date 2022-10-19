/**
 *  @file   packet.c
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  IR communication packet handler
 */

#include "packet.h"
#include "map.h"


void packet_init() //Initalise UART libary
{
    ir_uart_init();
}


uint8_t send_coords(packet_t *packet)
{
    static uint8_t received_value= 0; //Variable to track if received result
    static uint8_t sent_result = 0; // Variable track if sent coords

    if(sent_result == 0 && ir_uart_write_ready_p()) //If coords havnt been sent, wait until transmitter is ready and send X/Y coords
    {
        ir_uart_putc(packet->coords.x + packet->coords.y * 10); //Coordernates are sent as a two digit number in YX order. Eg 35 is X=5, Y=3. 
        sent_result = 1;
    }

    if(ir_uart_read_ready_p() && sent_result == 1 && received_value== 0)//If coords have been sent, and result has not been received, wait until transmitter is ready and receive result
    {
        packet->result = ir_uart_getc();
        received_value= 1;
    }

    if(received_value== 1) 
    {
        received_value= 0;
        sent_result = 0;
        return 1; //Return 1 when both result has been received and coords sent
    }

    return 0;
}


uint8_t recv_coords(packet_t *packet, uint8_t friendly_ships[])
{
    static uint8_t received_value= 0; // Variable track if coords recveived
    static uint8_t sent_result = 0; //Variable to check if result has been sent

    
    if(ir_uart_read_ready_p() && received_value== 0) //Receive Coords. Decodes the two digit number into 2, single digit numbers, Y and X. 
    {
        uint8_t in_val = ir_uart_getc();
        packet->coords.x = in_val % 10;
        packet->coords.y = (in_val - packet->coords.x) / 10;
        received_value= 1;
    }

    
    if (received_value== 1 && sent_result == 0 && ir_uart_write_ready_p()) 
    {
        uint8_t result = get_position_value(friendly_ships, packet->coords); // send hit or miss dependent on coordinates
        if(result == 0) //If a miss is sent, change result to 2, as this is what the map uses to render a miss
        {
            result = MISS;
        }

        packet->result = result; //Pack has been 'filled' with X/Y/Result
        
        ir_uart_putc(result); //Send result back to other player

        sent_result = 1;
    }

    if(sent_result == 1)
    {
        received_value= 0;
        sent_result = 0;
        return 1; //Return 1 when both result has been received and coords sent
    }

    return 0;
}