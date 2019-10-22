#pragma once

#include <stdint.h>

struct frame_context
{
    uint64_t frame;
    double delta_time;
    double cursor_x;
    double cursor_y;
    bool button_pressed;
};