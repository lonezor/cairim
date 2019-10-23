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