#pragma once

#include "rendering_context.hpp"
#include "frame_context.hpp"
#include "cairo_svg_surface.hpp"
#include <memory>

class object
{
public:
    object(double x, double y, double width, double height, std::shared_ptr<rendering_context> rc);

    void draw(frame_context& fc, double scene_width, double scene_height);

    void draw_rotated_svg_surface(double x_center, double y_center, double angle);

    bool intersects(double x, double y);
   // bool intersects(const object& other);

private:
    double x_;
    double y_;
    double width_;
    double height_;
    std::shared_ptr<rendering_context> rc_;
    cairo_svg_surface* svg_surface_;
};