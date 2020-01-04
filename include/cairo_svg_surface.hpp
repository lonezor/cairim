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

#include <string>
#include <cairo.h>
#include <librsvg/rsvg.h>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <memory>

class cairo_svg_surface
{
public:
    cairo_svg_surface(std::string path, double req_width, double req_height);

    cairo_t* get_context();
    cairo_surface_t* get_surface();

    double get_center_x();
    double get_center_y();

private:
    void read_from_svg_file(std::string path);
    
    std::string path_;
    RsvgHandle* rsvg_;
    RsvgDimensionData dim_;
    cairo_surface_t* surface_;
    cairo_t* cr_;
    double req_width_;
    double req_height_;
    double center_x_;
    double center_y_;
};

