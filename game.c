#include <system.h>
#include <navswitch.h>
#include <pacer.h>
#include <tinygl.h>
#include <button.h>
#include <ir_uart.h>

#include "defs.h"
#include "player.h"
#include "renderer.h"
#include "starting_positions.h"


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
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static uint8_t enemyGuesses[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static uint8_t friendlyGuesses[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static uint8_t mainmenu[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};


int Has_Player_Won(friendlyShips,enemyGuesses)
{
    for (uint8_t i=0; i<MAP_WIDTH * MAP_HEIGHT; ++i)
    {
        if (friendlyShips[i] == 1 & enemyGuesses[i] == 2 || friendlyShips[i] == 1 & enemyGuesses[i] == 0){
            return 0;
        }

    } 
    return 1;
}


void initialize(void)
{
    system_init();

    tinygl_init(PACER_RATE);
    pacer_init(PACER_RATE);

    button_init();
}

void update(void)
{
    navswitch_update();
    button_update();
    
    update_player();

    switch(game_state)
    {
        case MAINMENU:
            // if receive 1, defend, else if push pio, attack and send defend to other player
            if(ir_get(1))
            {
                game_state = DEFEND;
                generate_ships(friendlyShips);
            }
            else
            {
                if(navswitch_push_event_p(NAVSWITCH_PUSH))
                {
                    game_state = ATTACK;
                    generate_ships(friendlyShips);

                    ir_put(1);
                    // transmit 1 as enemy defend
                }
            }
            break;
        case ATTACK:
            if (navswitch_push_event_p(NAVSWITCH_PUSH) && buttonState == 1) { 
            //SEND X/Y coords 
            //Recive If hit or miss
            //Update Matricies
            //Check if enenmy guesses overlaps with friendly ships, game win if matches
            GameState = DEFEND

            }
            
            break;
        case DEFEND:
            // If Receive X/Y coords {
            // CheckIfHit -> Return 1 if hit, 0 if miss
            // Send 1 or 0
            // GameState = Attack
            // Check if enenmy guesses overlaps with friendly ships, game win if matches
            // GameState = DEFEND
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
