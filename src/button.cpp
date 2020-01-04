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

#include "button.hpp"

#include <stdio.h>

button operator|(button lhs, button rhs)
{
    
    auto l = static_cast<uint16_t>(lhs);
    auto r = static_cast<uint16_t>(rhs);
    auto value = l | r;
    return static_cast<button>(value);
}

button operator|=(button& lhs, button rhs)
{
    return lhs = lhs | rhs;
}

button operator&(button lhs, button rhs)
{
    auto l = static_cast<uint16_t>(lhs);
    auto r = static_cast<uint16_t>(rhs);
    return static_cast<button>(l & r);
}

button operator&=(button& lhs, button rhs)
{
    return lhs = lhs & rhs;
}

button operator~(button lhs)
{
    auto l = static_cast<uint16_t>(lhs);
    return static_cast<button>(~l);
}

bool button_active(button button_state, button flag)
{
    auto state = static_cast<uint16_t>(button_state);
    auto f = static_cast<uint16_t>(flag);
    return (state & f) > 0;
}