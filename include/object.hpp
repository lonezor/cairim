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

#include "rendering_context.hpp"
#include "frame_context.hpp"
#include "cairo_svg_surface.hpp"
#include <memory>

class object
{
public:
    object(double x, double y, double width, double height, std::shared_ptr<rendering_context> rc, std::string svg_path);
    virtual ~object() {}
    virtual void draw(frame_context& fc) = 0;

    double get_x();
    double get_y();
    double get_width();
    double get_height();

    void set_x(double x);
    void set_y(double y);

    virtual bool intersects(double x, double y);
    virtual bool intersects(const object& other);

protected:
    void draw_svg_surface(double x_center, double y_center, double alpha, double angle);

    double x_;
    double y_;
    double width_;
    double height_;
    std::shared_ptr<rendering_context> rc_;
    cairo_svg_surface* svg_surface_;
    std::string svg_path_;
    
};