#pragma once

#include <cairo.h>
#include <string>

class cairo_image_surface
{
public:
    cairo_image_surface(int width, int height);

    ~cairo_image_surface();

    cairo_t* get_context();
    void write_png(std::string path);

private:
    cairo_surface_t* surface_;
    cairo_t* ctx_;
    int width_;
    int height_;
};