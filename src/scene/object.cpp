#include "object.hpp"

#include <unistd.h>
#include <stdlib.h>

object::object(double x, double y, double width, double height, std::shared_ptr<rendering_context> rc)
 : x_(x)
 , y_(y)
 , width_(width)
 , height_(height)
 , rc_(rc)
{
    // Load SVG and render it with high DPI and scale it down based on current need (heavy operation)
    svg_surface_ = new cairo_svg_surface("/home/lonezor/project/cairim/svg/low_detail/water_wheel.svg", 
        rc_->scale(width_), rc_->scale(height_));
}

void object::draw(frame_context& fc, double scene_width, double scene_height)
{
    if (intersects(fc.cursor_x, fc.cursor_y)) {
        if (fc.button_pressed) {
        }
    }

    x_ = fc.cursor_x - svg_surface_->get_center_x();
    y_ = fc.cursor_y - svg_surface_->get_center_y();

    rc_->draw_surface(svg_surface_->get_surface(), x_ , y_);
}

bool object::intersects(double x, double y)
{
 
    return ((x >= x_ && x <= x_ + width_) &&
            (y >= y_ && y <= y_ + height_));

}

//bool object::intersects(const object& other)
//{
//
//}