#pragma once

#include "button.hpp"

#include <stdint.h>

struct frame_context
{
    uint64_t frame;
    int64_t timestamp; // unit: microseconds
    double frame_rate;
    double scene_width;
    double scene_height;
    double cursor_x;
    double cursor_y;
    bool cursor_visible;
    bool osd_visible;
    bool screen_border;
    button button_state;
};