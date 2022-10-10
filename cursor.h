#pragma once

#include <stdint.h>

typedef struct cursor
{
    int8_t x;
    int8_t y;
} cursor_t;

void update_cursor(void);