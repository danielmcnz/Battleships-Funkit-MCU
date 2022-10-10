#include <stdint.h>

typedef struct cursor
{
    int8_t x;
    int8_t y;

    int8_t prev_x;
    int8_t prev_y;
} cursor_t;

void update_cursor();