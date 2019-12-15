#pragma once

#include "button.hpp"

#include <stdint.h>

struct frame_context
{
    int64_t timestamp; // unit: microseconds
    double frame_rate;
    double scene_width;
    double scene_height;
    double cursor_x;
    double cursor_y;
    button button_state;
    bool cursor_visible;
    char key_state; // zero for no key pressed
    bool osd_visible;
    bool screen_border;
};