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

#include "cairo_image_surface.hpp"

cairo_image_surface::cairo_image_surface(int width, int height)
: width_(width)
, height_(height)
{
    surface_ = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width_, height_);
    ctx_ =  cairo_create(surface_);
}

cairo_image_surface::~cairo_image_surface()
{
    cairo_destroy(ctx_);
    cairo_surface_destroy(surface_);
}

cairo_t* cairo_image_surface::get_context()
{
    return ctx_;
}

cairo_surface_t* cairo_image_surface::get_surface()
{
    return surface_;
}
