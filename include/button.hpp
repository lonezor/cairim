/*
 *  Cairim
 *  Copyright (C) 2020 Johan Norberg <lonezor@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>
 */

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