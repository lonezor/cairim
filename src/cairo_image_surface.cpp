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

void cairo_image_surface::write_png(std::string path)
{
    
cairo_surface_write_to_png (surface_,
                            path.c_str());
}

cairo_t* cairo_image_surface::get_context()
{
    return ctx_;
}