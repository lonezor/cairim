
#include "cairo_svg_surface.hpp"


#include <fstream>

#include <memory>


cairo_svg_surface::cairo_svg_surface(std::string path, double req_width, double req_height)
: path_(path)
, req_width_(req_width)
, req_height_(req_height)
{
    read_from_svg_file(path);
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
     
    // Set high to accomodate any screen resolution
    rsvg_set_default_dpi(600.0);

    rsvg_ = rsvg_handle_new_from_data((const guint8*)buffer.get(), (gsize)size, &error);
    
    if (error != nullptr)
    {
        printf("Error: %s\n", error->message);
        return;
    }

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
    rsvg_handle_render_cairo (rsvg_, cr_);
}

