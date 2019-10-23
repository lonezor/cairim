#include "object.hpp"
#include "test.hpp"

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
        width_, height_);
}

void object::draw_rotated_svg_surface(double x, double y, double angle)
{
    auto cr = rc_->get_ctx();
cairo_save(cr);

cairo_matrix_t matrix;

cairo_matrix_init(&matrix,
1,
0,
0,
1,
0,
0);

// Final results
cairo_matrix_translate(&matrix,
                        x/2,
                        y/2);

cairo_transform (cr, &matrix);

cairo_matrix_rotate (&matrix, angle);

cairo_transform (cr, &matrix);

rc_->draw_surface(svg_surface_->get_surface(), -svg_surface_->get_center_x() , -svg_surface_->get_center_y());

cairo_restore(cr);
}

void object::draw(frame_context& fc, double scene_width, double scene_height)
{
    static double rate = 0.001;
    if (intersects(fc.cursor_x, fc.cursor_y)) {
        if (fc.button_pressed) {
            rate -= 0.0001;
        }
    } else {
        rate = 0.001;
    }



static double a = 0;
a -= rate;
if (a < 0){
    a = 1;
}

draw_rotated_svg_surface(x_, y_, a * 2 * m_pi);


}

bool object::intersects(double x, double y)
{
    double comp_x = x_ - svg_surface_->get_center_x();
    double comp_y = y_ - svg_surface_->get_center_y();
 
    return ((rc_->scale(x) >= comp_x && rc_->scale(x) <= comp_x + width_) &&
            (rc_->scale(y) >= comp_y && rc_->scale(y) <= comp_y + height_));

}

//bool object::intersects(const object& other)
//{
//
//}