#include <system.h>
#include <navswitch.h>
#include <pacer.h>
#include <tinygl.h>
#include <button.h>

#include "defs.h"
#include "player.h"
#include "renderer.h"


enum gameState
{
    MAINMENU,
    ATTACK,
    DEFEND,
};

static enum gameState game_state;

static int8_t buttonState = 1;

void generate_ships(uint8_t map[35])
{
    uint8_t locations[] = 
    {
        1, 2, 0, 0, 0, 0, 0,
        0, 0, 2, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 2,
        0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 1, 2, 1, 2,
    };


    // fill map
    for (uint8_t i=0; i<MAP_WIDTH * MAP_HEIGHT; ++i)
    {
        map[i] = locations[i];
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


            if(navswitch_push_event_p(NAVSWITCH_PUSH))
            {
                game_state = ATTACK;
                // transmit 1 as enemy defend
            }
            break;
        case ATTACK:
            break;
        case DEFEND:
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

    game_state = ATTACK;

    generate_ships(friendlyShips);

    while (1)
    {
        pacer_wait ();

        render();     
        update(); 
    }

    return 0;
}
