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
    // Load SVG and render it with high DPI and scale it down based on current need (heavy operation)
    // Scale width/height since cairo operations are delegated to third library instead of rendering ctx
    svg_surface_ = new cairo_svg_surface(svg_path, rc_->scale(width_), rc_->scale(height_));
}

void object::draw_svg_surface(double x, double y, double angle)
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
    rc_->draw_surface(svg_surface_->get_surface(), -svg_surface_->get_center_x() , -svg_surface_->get_center_y());

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

//bool object::intersects(const object& other)
//{
//
//}