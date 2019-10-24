#pragma once

#include <stdint.h>

struct frame_context
{
    uint64_t frame;
    double delta_time;
    double scene_width;
    double scene_height;
    double cursor_x;
    double cursor_y;
    bool cursor_visible;
    bool osd_visible;
    bool screen_border;
    bool button_pressed;
    double frame_rate;
    std::string elapsed_time;
};