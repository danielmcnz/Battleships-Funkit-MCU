#include <system.h>
#include <navswitch.h>
#include <pacer.h>
#include <tinygl.h>
#include <button.h>
#include <ir_uart.h>

#include <avr/io.h>

#include "defs.h"
#include "player.h"
#include "renderer.h"
#include "starting_positions.h"
#include "packet.h"


enum gameState
{
    MAINMENU,
    ATTACK,
    DEFEND,
};

static enum gameState game_state;

static int8_t buttonState = 1;

uint8_t rand;

void generate_ships(uint8_t map[35])
{
    uint8_t *location = locations[rand % 3];
    // fill map
    for (uint8_t i=0; i<MAP_WIDTH * MAP_HEIGHT; ++i)
    {
        map[i] = location[i];
    }
}

static uint8_t friendlyShips[] = 
{
    1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static uint8_t enemyGuesses[] =
{
    1, 2, 0, 0, 0, 0, 0,
    1, 2, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static uint8_t friendlyGuesses[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0,
};

static uint8_t mainmenu[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};


int Has_Player_Won()
{
    for (uint8_t i=0; i<MAP_WIDTH * MAP_HEIGHT; ++i)
    {
        if (friendlyShips[i] == 1 && enemyGuesses[i] == 0){
            return 0;
        }

    } 
    return 1;
}

void update_map(player_t player, uint8_t Map[], uint8_t RESULT){
    Map[MAP_HEIGHT * player.x + (MAP_HEIGHT-1)-player.y] = RESULT;
}


void initialize(void)
{
    system_init();

    tinygl_init(PACER_RATE);
    pacer_init(PACER_RATE);

    button_init();

    ir_uart_init();
    DDRC |= (1<<2);
}

void update(void)
{
    navswitch_update();
    button_update();
    
    update_player();

    packet_t packet = {0};

    static uint8_t recv = 0;

    static uint8_t button_pushed = 0;

    switch(game_state)
    {
        case MAINMENU:
            // if receive 1, defend, else if push pio, attack and send defend to other player
            if(ir_uart_read_ready_p())
            {
                if(ir_uart_getc() == 1)
                {
                    game_state = DEFEND;
                    generate_ships(friendlyShips);
                }
            }
            else
            {
                if(navswitch_push_event_p(NAVSWITCH_PUSH))
                {
                    game_state = ATTACK;
                    generate_ships(friendlyShips);

                    // transmit 1 as enemy defend
                    ir_uart_putc(1);
                }
            }
            break;
        case ATTACK:
            if(navswitch_push_event_p(NAVSWITCH_PUSH))
                button_pushed = 1;

            if (button_pushed == 1 && buttonState == 1) { 
                packet.coords.x = get_player().x;
                packet.coords.y = get_player().y;
                
                //SEND X/Y coords
                recv = send_coords(&packet);
                
                if(recv == 1)
                {
                    //Update Matricies
                    update_map(get_player(), friendlyGuesses, packet.result);
                    
                    //Check if enenmy guesses overlaps with friendly ships, game win if matches
                    game_state = DEFEND;

                    button_pushed = 0;
                }
            }
            
            break;
        case DEFEND:
            // Receive X/Y coords
            recv = recv_coords(&packet, friendlyShips);
            
            if(recv == 1)
            {
                //Update Matricies
                update_map(get_player(), enemyGuesses, packet.result);

                // Check if enenmy guesses overlaps with friendly ships, game win if matches
                Has_Player_Won();

                // GameState = Attack
                game_state = ATTACK;
            }
            break;
    }


    if(button_push_event_p(BUTTON1))
        buttonState = -buttonState;

    
}

void render(void)
{
    tinygl_update();
    draw_clear();

    switch(game_state)
    {
        case MAINMENU:
            draw_map(mainmenu);
            break;
        case ATTACK:
            if(buttonState == 1)
            {
                draw_map(friendlyGuesses);
                draw_flashing_pixel(get_player().x, get_player().y);
            }
            else
                draw_map(enemyGuesses);
            break;
        case DEFEND:
            if(buttonState == 1)
            {
                draw_map(friendlyShips);
            } 
            else
                draw_map(enemyGuesses);
            break;
    };   
}

int main (void)
{ 
    
    initialize();

    game_state = MAINMENU;

    while (1)
    {
        pacer_wait ();

        render();     
        update(); 
        rand++;
        if (rand >= 255){
            rand = 0;

        }
    }

    return 0;
}
