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