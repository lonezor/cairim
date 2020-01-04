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