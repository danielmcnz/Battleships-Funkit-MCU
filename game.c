#include <system.h>
#include <navswitch.h>
#include <pacer.h>
#include <tinygl.h>
#include <font3x5_1.h>
#include <button.h>
#include <ir_uart.h>

#include "defs.h"
#include "player.h"
#include "renderer.h"
#include "starting_positions.h"
#include "packet.h"


enum gameState
{
    MAINMENU,
    WIN,
    LOSE,
    HIT,
    MISS,
    ATTACK,
    DEFEND,
};

static enum gameState prev_game_state;
static enum gameState game_state;

static int8_t buttonState = 1;

uint8_t rand = 0;

void generate_ships(uint8_t map[35])
{
    // uint8_t direction = rand % 2; // horizontal = 0, vertical = 1

    // // pos_t boats[] = { (1, 2), (1, 3),
    // //                     (3, 2), (3, 3),
    // //                     (2, 1), (3, 1), (4, 1)
    // //                 };

    // pos_t boats[] = {0};

    // uint8_t pixel_count = 0;

    // for(uint8_t n_boat = 0; n_boat < MAX_SHIPS; ++n_boat)
    // {
    //     for(uint8_t n_pixel = 1; n_pixel < BATTLESHIPS[n_boat]; ++n_pixel)
    //     {
    //         if(n_pixel == 0)
    //         {
    //             if(direction == 0)
    //             {
    //                 boats[pixel_count + n_pixel].x = rand % (MAP_HEIGHT + 1 - BATTLESHIPS[n_boat]);
    //                 boats[pixel_count + n_pixel].y = rand % (MAP_WIDTH + 1);
    //             }
    //             else
    //             {
    //                 boats[pixel_count + n_pixel].x = rand % (MAP_WIDTH + 1);
    //                 boats[pixel_count + n_pixel].y = rand % (MAP_HEIGHT + 1 - BATTLESHIPS[n_boat]);
    //             }
    //         }
    //         else
    //         {
    //             if(direction == 0)
    //             {
    //                 boats[pixel_count + n_pixel].x = boats[pixel_count + n_pixel-1].x++;
    //             }
    //             else
    //             {
    //                 boats[pixel_count + n_pixel].y = boats[pixel_count + n_pixel-1].y++;
    //             }
    //         }

    //         ++pixel_count;
    //     }

        
    // }

    // fill map




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


int Has_Player_Won(void)
{
    for (uint8_t i=0; i<MAP_WIDTH * MAP_HEIGHT; ++i)
    {
        if (friendlyShips[i] == 1 && enemyGuesses[i] == 0){
            return 0;
        }

    } 
    return 1;
}

void update_map(coords_t coords, uint8_t Map[], uint8_t RESULT){
    Map[MAP_HEIGHT * coords.x + (MAP_HEIGHT - 1) - coords.y] = RESULT;
}


void initialize(void)
{
    system_init();

    tinygl_init(DISPLAY_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_speed_set(MESSAGE_RATE);

    pacer_init(PACER_RATE);

    button_init();

    ir_uart_init();
}

void update(void)
{
    navswitch_update();
    button_update();
    
    update_player();

    packet_t packet = {0};

    static uint8_t recv = 0;

    static uint8_t button_pushed = 0;

    static uint16_t time = 0;

    switch(game_state)
    {
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
                    update_map(packet.coords, friendlyGuesses, packet.result);
                    
                    //Check if enenmy guesses overlaps with friendly ships, game win if matches
                    
                    game_state = DEFEND;
                    prev_game_state = game_state;
                    // if(packet.result == 1)
                    // {
                    //     game_state = HIT;
                    // }
                    // else
                    // {
                    //     game_state = MISS;
                    // }

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
                update_map(packet.coords, enemyGuesses, packet.result);

                // Check if enenmy guesses overlaps with friendly ships, game win if matches
                // Has_Player_Won()

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


    if(button_push_event_p(BUTTON1))
        buttonState = -buttonState;

    
}


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
        case MAINMENU:
            if(!title_set)
            {
                tinygl_text("BATTLESHIPS");
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

int main(void)
{ 
    initialize();

    uint8_t render_time = 0;

    game_state = MAINMENU;

    while (1)
    {
        pacer_wait ();

        if(render_time == PACER_RATE / DISPLAY_RATE)
        {
            render(); 
            render_time = 0; 
        }   

        update();

        rand++;
        if (rand >= 255){
            rand = 0;
        }

        ++render_time;
    }

    return 0;
}
