#pragma once

#include <stdint.h>

#include <system.h>
#include <navswitch.h>

typedef struct
{
    int8_t x;
    int8_t y;
} cursor_t;

void update_cursor(void);

cursor_t get_cursor(void);