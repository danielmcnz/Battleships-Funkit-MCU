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


enum gameState
{
    MAIN_MENU,
    WIN,
    LOSE,
    HIT,
    MISS,
    ATTACK,
    DEFEND,
};

static enum gameState prev_game_state;
static enum gameState game_state;

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

    static uint16_t time = 0;

    switch(game_state)
    {
        case ATTACK: //ATTACK SCREEN GAME LOGIC
            if(navswitch_push_event_p(NAVSWITCH_PUSH)) //Updating nav pressed down
                nav_push_down = 1;

            if (nav_push_down == 1 && top_button == 1) { //If a shot is fired, get X/Y position of cursor
                packet.coords.x = get_player().x;
                packet.coords.y = get_player().y;
                
                
                recv = send_coords(&packet); //Send X&Y coords. Packet.result gets updated with hit or miss
                
                if(recv == 1)
                {
                    
                    update_map(packet.coords, friendly_guesses, packet.result); //Update Map
                    
                    /** @todo GAME WINNING
                    */
                    
                    game_state = DEFEND; //Switch to defend once fired
                    prev_game_state = game_state;
                    // if(packet.result == 1)
                    // {
                    //     game_state = HIT;
                    // }
                    // else
                    // {
                    //     game_state = MISS;
                    // }

                    nav_push_down = 0;                    
                }
            }
            
            break;
        case DEFEND:
            
            recv = recv_coords(&packet, friendly_ships); // Receive X&Y coords of attack fire. Sets recv to 1 if 
            
            if(recv == 1)
            {
                
                update_map(packet.coords, enemy_guesses, packet.result); //Update Map

                /** @todo GAME WINNING
                                    */

                game_state = ATTACK;
                prev_game_state = game_state;
                // if(packet.result == 1)
                // {
                //     game_state = HIT;
                // }
                // else
                // {
                //     game_state = MISS;
                // }
            }
            break;
        case MAIN_MENU:
            
            if(ir_uart_read_ready_p()) // Wait until receive '1',  as other player has pressed button first. Therefore set to Defend.
            {
                if(ir_uart_getc() == 1)
                {
                    game_state = DEFEND; 
                    generate_ships(friendly_ships);
                }
            }
            else
            {
                if(navswitch_push_event_p(NAVSWITCH_PUSH)) //Send '1' to other player, telling them to defend and you set to attack first. 
                {
                    game_state = ATTACK;
                    generate_ships(friendly_ships);
                    ir_uart_putc(1);
                }
            }
            break;

        case HIT:
            if(time >= PACER_RATE * 4)
            {
                time = 0;
                game_state = prev_game_state;
            }
            time++;
            break;
        case MISS:
            if(time >= PACER_RATE * 4)
            {
                time = 0;
                game_state = prev_game_state;
            }
            time++;
            break;
        case WIN:
            break;
        case LOSE:
            break;
    }


    if(button_push_event_p(BUTTON1)) //Update top button toggle, which is used in the renderer function to update screen/ 
        top_button = -top_button;

    
}

/**Deterimes what to draw to the screen
*/
void render(void)
{
    tinygl_update();

    if(game_state > MISS)
        draw_clear();

    static uint8_t title_set = 0;
    static uint8_t hit_set = 0;
    static uint8_t miss_set = 0;
    static uint8_t win_set = 0;
    static uint8_t lose_set = 0;
    
    switch(game_state)
    {
        case ATTACK: //ATTACK MODE
            if(top_button == 1) 
            {
                draw_map(friendly_guesses); //If top Button toggled to one, draw your guesses map
                draw_flashing_pixel(get_player().x, get_player().y); //Draw 'aiming' cursor over top
            }
            else
                draw_map(enemy_guesses); //If top Button toggled to 0, draw enemy guesses map
            break;
        case DEFEND: //DEFEND MODE
            if(top_button == 1)
            {
                draw_map(friendly_ships); //If top Button toggled to one, draw your ships map
            } 
            else
                draw_map(enemy_guesses); //If top Button toggled to 0, draw enemy guesses map
            break;
        case MAIN_MENU: //MAIN MENU
            if(!title_set)
            {
                tinygl_text("BATTLESHIPS"); //Draw main scroll
                title_set = 1;
            }
            break;
        case HIT:
            if(!hit_set)
            {
                tinygl_text("HIT");
                hit_set = 1;
            }
            break;
        case MISS:
            if(!miss_set)
            {
                tinygl_text("MISS");
                miss_set = 1;
            }
            break;
        case WIN:
            if(!win_set)
            {
                tinygl_text("YOU WON");
                win_set = 1;
            }
            break;
        case LOSE:
            if(!lose_set)
            {
                tinygl_text("YOU LOSE");
                lose_set = 1;
            }
            break;
    };
}
/**Main Loop, all major functions are called here
*/
int main(void)
{ 
    initialize(); //Initalise everything

    uint8_t render_time = 0;

    game_state = MAIN_MENU; //Set gamestate to main menu to start 

    while (1)
    {
        pacer_wait ();

        if(render_time == PACER_RATE / DISPLAY_RATE) //Draw screen at slower rate than game logic updates as it is not needed to very fast.
        {
            render(); 
            render_time = 0; 
        }   

        update(); // Update the game logic

        ++render_time;
    }

    return 0;
}
