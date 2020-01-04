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

#include "cairo_svg_surface.hpp"
#include "constants.hpp"


#include <fstream>

#include <memory>


cairo_svg_surface::cairo_svg_surface(std::string path, double req_width, double req_height)
: path_(path)
, req_width_(req_width)
, req_height_(req_height)
{
    read_from_svg_file(path);

   // std::cout << "cairo_svg_surface::cairo_svg_surface: " << path << std::endl;
}

cairo_t* cairo_svg_surface::get_context()
{
    return cr_;
}

cairo_surface_t* cairo_svg_surface::get_surface()
{
    return surface_;
}

double cairo_svg_surface::get_center_x()
{
    return center_x_;
}

double cairo_svg_surface::get_center_y()
{
    return center_y_;
}

void cairo_svg_surface::read_from_svg_file(std::string path)
{
    std::ifstream f(path, std::ios::in|std::ios::binary|std::ios::ate);
    if (!f.is_open()) {
        return; // todo: exception
    }

    auto size = f.tellg();
    std::unique_ptr<char> buffer(new char[size]);

    f.seekg (0, std::ios::beg);
    f.read (buffer.get(), size);
    f.close();

    GError* error = nullptr;
     
    rsvg_ = rsvg_handle_new_from_data((const guint8*)buffer.get(), (gsize)size, &error);
    
    if (error != nullptr)
    {
        printf("Error: %s\n", error->message);
        return;
    }

    // Set high DPI to accomodate any screen resolution up to 8k
    // Scale it with respect to the actually needed resolution since
    // this has a very significant effect on performance.
    double prop = req_width_ / static_cast<double>(scale_ref_width);
    double dpi = 660 * prop;
    if (dpi < 1) {
        dpi = 1;
    }
    if (dpi > 660) {
        dpi = 660;
    }
    rsvg_handle_set_dpi(rsvg_, dpi);

    // Assumption: SVG object scaled to full width of the document
    rsvg_handle_get_dimensions(rsvg_, &dim_);

    // Create full size surface
    double width = (double)dim_.width;
    double height = (double)dim_.height;
    surface_ = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cr_ = cairo_create(surface_);

    // Before rendering, set scaling to requested resolution
    double ar = req_width_ / req_height_;
    double sx = req_width_ / width;
    double sy = req_height_ / (width / ar);

    center_x_ = (width * sx) / 2;
    center_y_ = center_x_; // currently, this is more useful

    cairo_scale(cr_, sx, sy);

    // Render SVG to cairo surface
    rsvg_handle_render_cairo(rsvg_, cr_);

    auto w =  cairo_image_surface_get_width (surface_);
    auto h = cairo_image_surface_get_height (surface_);
}

