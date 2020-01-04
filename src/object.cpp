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

#include "object.hpp"

#include <unistd.h>
#include <stdlib.h>

object::object(double x, double y, double width, double height, std::shared_ptr<rendering_context> rc, std::string svg_path)
 : x_(x)
 , y_(y)
 , width_(width)
 , height_(height)
 , rc_(rc)
 , svg_path_(svg_path)
{
    if (!svg_path.empty()) {
        // Load SVG and render it with high DPI and scale it down based on current need (heavy operation)
        // Scale width/height since cairo operations are delegated to third library instead of rendering ctx
        svg_surface_ = new cairo_svg_surface(svg_path,
                                             rc_->scale(width_),
                                             rc_->scale(height_));
    }
}

void object::draw_svg_surface(double x, double y, double alpha, double angle)
{
    // Restoration point before transformations
    rc_->save();

    // Initialise matrix
    cairo_matrix_t matrix;
    rc_->matrix_init(&matrix,
        1,
        0,
        0,
        1,
        0,
        0);

    // Translation to target coordinates
    rc_->matrix_translate(&matrix,
                            rc_->scale(x)/2,
                            rc_->scale(y)/2);
    rc_->transform(&matrix);

    // Rotation around center of object
    rc_->matrix_rotate(&matrix, angle);
    rc_->transform(&matrix);

    // Render SVG surface 
    rc_->draw_surface(svg_surface_->get_surface(), 
                      -svg_surface_->get_center_x(),
                      -svg_surface_->get_center_y(),
                      alpha);

    // Revert transformation settings
    rc_->restore();
}

bool object::intersects(double x, double y)
{
    // Determine object intersection with cursor
    // Compensate for scaling (replay)
    double sf = rc_->scale(1);

    x *= sf;
    y *= sf;
    
    double comp_x = x_*sf - svg_surface_->get_center_x();
    double comp_y = y_*sf - svg_surface_->get_center_y();
 
    return ((x >= comp_x && x <= comp_x + width_*sf) &&
            (y >= comp_y && y <= comp_y + height_*sf));
}

bool object::intersects(const object& other)
{
    return false; // not purely virtual to make implementation optional
}

double object::get_x()
{
    return x_;
}

double object::get_y()
{
    return y_;
}

double object::get_width()
{
    return width_;
}

double object::get_height()
{
    return height_;
}

void object::set_x(double x)
{
    x_ = x;
}

void object::set_y(double y)
{
    y_ = y;
}

