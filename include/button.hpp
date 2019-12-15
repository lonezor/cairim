#pragma once

#include <stdint.h>

enum class button : uint16_t
{
    none = 0x0000, // No button
    left = 0x0001, // left button
    middle = 0x0002, // middle button (scroll wheel pressed)
    right = 0x0004, // right button
    scroll_up = 0x0008, // up scroll wheel button
    scroll_down = 0x0010, // down scroll wheel button
    scroll_left = 0x0020, // left scroll wheel button
    scroll_right = 0x0040, // right scroll wheel button
    nav_back = 0x0080, // backward button
    nav_forward = 0x0100, // forward button
};

button operator|(button lhs, button rhs);
button operator|=(button& lhs, button rhs);
button operator&(button lhs, button rhs);
button operator&=(button& lhs, button rhs);
button operator~(button lhs);
bool button_active(button button_state, button flag);