/**
 * @todo this
*/

#include "map.h"
#include "random.h"


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
    }
};


void update_map(pos_t coords, uint8_t Map[], uint8_t RESULT){
    Map[MAP_HEIGHT * coords.x + (MAP_HEIGHT - 1) - coords.y] = RESULT;
}


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




    uint8_t *location = locations[rand() % 3];

    // fill map
    for (uint8_t i=0; i<MAP_WIDTH * MAP_HEIGHT; ++i)
    {
        map[i] = location[i];
    }
}