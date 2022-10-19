/**
 *  @file   game.c
 *  @author D. McGregor, C. Hall-Fernandez
 *  @date   18 October 2022
 *  @brief  Battleships main game logic
 */

//Include for C & UCFunkit supplied modules
#include <system.h>
#include <navswitch.h>
#include <pacer.h>
#include <tinygl.h>
#include <button.h>
#include <ir_uart.h>

// Custom Modules
#include "defs.h"
#include "player.h"
#include "renderer.h"
#include "packet.h"
#include "map.h"
#include "random.h"


/** Creating arrays which will become maps */
uint8_t friendly_ships[] = 
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};


uint8_t enemy_guesses[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

uint8_t friendly_guesses[] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

//Possible maps for game
static uint8_t locations[N_SHIP_MAPS][35] = 
{
    {
        1, 0, 1, 1, 1, 1, 0,
        1, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 1, 0, 0,
    },
    {
        1, 1, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1, 0,
        0, 0, 1, 1, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1,
    },
    {
        1, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0,
    },
};

enum gameState
{
    MAIN_MENU_SCREEN,
    WIN_SCREEN,
    LOSE_SCREEN,
    ATTACK,
    DEFEND,
};

static enum gameState prev_game_state;
static enum gameState game_state;

static int8_t enemy_map_val = 0;

static int8_t top_button = 1;


/**Initialising various modules
*/
void initialize(void)
{
    system_init();

    renderer_init();

    packet_init();

    pacer_init(PACER_RATE);

    button_init();
}

/**Updates the internal game logic
*/
void update(void)
{
    navswitch_update();
    button_update();
    update_player();
    update_rand();

    packet_t packet = {0};

    static uint8_t recv = 0;

    static uint8_t nav_push_down = 0; //Has the navswitch button been pressed down

    switch(game_state)
    {
        case ATTACK: //ATTACK SCREEN GAME LOGIC
            if(navswitch_push_event_p(NAVSWITCH_PUSH)) //Updating nav pressed down
            {
                nav_push_down = 1;
            }

            if (nav_push_down == 1 && top_button == 1) { //If a shot is fired, get X/Y position of cursor
                packet.coords.x = get_player().x;
                packet.coords.y = get_player().y;
                
                
                recv = send_coords(&packet); //Send X&Y coords. Packet.result gets updated with hit or miss
                
                if(recv == 1)
                {
                    
                    update_map(packet.coords, friendly_guesses, packet.result); //Update Map
                    
                    game_state = DEFEND; //Switch to defend once fired                    
                    prev_game_state = game_state;

                    
                    if(has_player_won(locations[enemy_map_val], friendly_guesses)) //Checks if player has one, sends to 'Win' screen if true 
                    {
                        game_state = WIN_SCREEN;
                    }                              

                    nav_push_down = 0;                    
                }

            }
            
            break;
        case DEFEND:
            
            recv = recv_coords(&packet, friendly_ships); // Receive X&Y coords of attack fire. Sets recv to 1 if 
            
            if(recv == 1)
            {
                
                update_map(packet.coords, enemy_guesses, packet.result); //Update Map

                game_state = ATTACK;
                prev_game_state = game_state;

                if(has_player_won(friendly_ships, enemy_guesses)) //Checks if player has lost, goes to lose screen if true 
                {
                    game_state = LOSE_SCREEN;
                }
                
            }
            break;
        case MAIN_MENU_SCREEN:
            if(ir_uart_read_ready_p()) // Wait until receive 'enemy map',  as other player has pressed button first. Therefore set to Defend.
            {
                int8_t res = ir_uart_getc();
                if(res > -1 && res < N_SHIP_MAPS) //If received enemy map, start the game. Set as defend
                {
                    enemy_map_val = res;
                    game_state = DEFEND; 
                    generate_ships(friendly_ships, locations);
                }
            }
            else
            {
                if(navswitch_push_event_p(NAVSWITCH_PUSH)) //Send 'Enemy Map' to other player, telling them to defend and you set to attack first. 
                {
                    game_state = ATTACK;
                    enemy_map_val = generate_ships(friendly_ships, locations); //Sends the enemy map value, used for checking win/loss state
                    ir_uart_putc(enemy_map_val);
                }
            }
            break;
        case WIN_SCREEN:
            break;
        case LOSE_SCREEN:
            break;
    }

    if(button_push_event_p(BUTTON1)) //Update top button toggle, which is used in the renderer function to update screen/ 
        top_button = -top_button;

    
}

/**Deterimes what to draw to the screen
*/
void render(uint16_t *time)
{
    tinygl_update();
        
    if(game_state > LOSE_SCREEN) //Clears screen if its an attack or defend
        draw_clear();

    static uint8_t title_set = 0;
    static uint8_t win_set = 0;
    static uint8_t lose_set = 0;
    
    switch(game_state)
    {
        case ATTACK: //ATTACK MODE
            if(top_button == 1) 
            {
                draw_map(time, friendly_guesses); //If top Button toggled to one, draw your guesses map
                draw_flashing_pixel(time, get_player().x, get_player().y); //Draw 'aiming' cursor over top
            }
            else
                draw_map(time, enemy_guesses); //If top Button toggled to 0, draw enemy guesses map
            break;
        case DEFEND: //DEFEND MODE
            if(top_button == 1)
            {
                draw_map(time, friendly_ships); //If top Button toggled to one, draw your ships map
            } 
            else
                draw_map(time, enemy_guesses); //If top Button toggled to 0, draw enemy guesses map
            break;
        case MAIN_MENU_SCREEN: //MAIN MENU
            if(!title_set)
            {
                tinygl_text("BATTLESHIPS"); //Draw main scroll
                title_set = 1;
            }
            break;
        case WIN_SCREEN:
            if(!win_set) //Only run once when win_screen is set. Show W to player
            {
                draw_clear();
                tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
                tinygl_text("W");
                win_set = 1;
            }
            break;
        case LOSE_SCREEN:
            if(!lose_set)//Only run once when lose_screen is set. Show L to player
            {
                draw_clear();
                tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
                tinygl_text("L");
                lose_set = 1;
            }
            break;
    };
}
/** Main Loop, all major functions are called here */
int main(void)
{ 
    initialize(); //Initalise everything

    uint16_t time = 0;
    uint8_t render_time = 0;

    game_state = MAIN_MENU_SCREEN; //Set gamestate to main menu to start 

    while (1)
    {
        pacer_wait();

        update(); // Update the game logic

        if(render_time == PACER_RATE / DISPLAY_RATE) //Draw screen at slower rate than game logic updates as it is not needed to very fast.
        {
            render(&time); 
            render_time = 0; 
            ++time; //Itterate time each render tick. Used to generate the flashing lights 
        }

        ++render_time;
    }

    return 0;
}
