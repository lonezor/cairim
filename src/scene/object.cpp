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
    x_velocity_ = drand48() * 0.5;
    y_velocity_ = drand48() * 0.5;

    double x_dir = drand48();
    double y_dir = drand48();
    if (x_dir > 0.5) {
        x_velocity_ *= -1;
    }
    if (y_dir > 0.5) {
        y_velocity_ *= -1;
    }

    rnd_ = drand48()*20;
}

void object::draw(frame_context& fc, double scene_width, double scene_height)
{
    if (intersects(fc.cursor_x, fc.cursor_y)) {
        if (fc.button_pressed) {
            r_ = drand48();
            g_ = drand48();
            b_ = drand48();
            rc_->set_source_rgba(r_,g_,b_,drand48());
        } else {
            rc_->set_source_rgba(1,1,1, 0.5);
        }
    } else {
        rc_->set_source_rgba(r_,g_,b_, 0.5);
    }

    rc_->rectangle(x_, y_, width_, height_);
    rc_->arc(x_, y_,width_,0,2*m_pi);
    rc_->fill();

    if (x_ + width_ > scene_width) {
        x_velocity_ *= -1;
    }
    if (x_ < 0) {
        x_velocity_ *= -1;
    }

    if (y_ + height_ > scene_height) {
        y_velocity_ *= -1;
    }
    if (y_ < 0) {
        y_velocity_ *= -1;
    }

    x_ += (x_velocity_);
    y_ += (y_velocity_);
}

bool object::intersects(double x, double y)
{
 
    return ((x >= x_ && x-rnd_ <= x_ + width_+rnd_) &&
            (y >= y_ && y-rnd_ <= y_ + height_+rnd_));

}

//bool object::intersects(const object& other)
//{
//
//}