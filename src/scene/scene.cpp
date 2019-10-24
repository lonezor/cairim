#include "scene/scene.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <iostream>

scene::scene(double width, double height, std::shared_ptr<rendering_context> rc,
                double level_of_detail)
: width_(width)
, height_(height)
, rc_(rc)
, level_of_detail_(level_of_detail)
{
        objects_.emplace_back(object(width/2,height/2,width/2, width/2, rc_));
}

scene::~scene()
{

}

void scene::draw(frame_context& fc)
{
    draw_begin();
    draw_bg();
    draw_main(fc);
    draw_fg(fc);
    draw_end();
}

void scene::draw_begin()
{
    rc_->push_group();
}

void scene::draw_end()
{
    rc_->pop_group_to_source();
    rc_->paint();
}

void scene::draw_bg()
{
    // Black background
    rc_->set_source_rgb(0, 0, 0);
    rc_->paint();
}

void scene::draw_main(frame_context& fc)
{
    rc_->set_source_rgb(0, 0, 0);
    rc_->paint();

    fc.scene_width = width_;
    fc.scene_height = height_;

    for(auto&& obj : objects_) {
        obj.draw(fc);
    }
}

void scene::draw_fg(frame_context& fc)
{
    // On screen display
    if (fc.osd_visible) {
        rc_->set_source_rgba(1, 1, 1, 1);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0);
        oss << "fps: " << fc.frame_rate;
        oss << ", elapsed time: " << fc.elapsed_time;
    
        rc_->move_to(10,15);
        rc_->font_size(12);
        rc_->show_text(oss.str());
    }

    if (fc.screen_border) {
        rc_->set_source_rgba(1, 1, 1, 1);
        rc_->move_to(0,0);
        rc_->line_to(fc.scene_width,0);
        rc_->line_to(fc.scene_width,fc.scene_height);
        rc_->line_to(0,fc.scene_height);
        rc_->line_to(0,0);
        rc_->stroke();
    }

    // Mouse cursor
    if (fc.cursor_visible) {
        double radius = 8;
        double alpha = 0.5;
        double gray = 0.4;
        if (fc.button_pressed) {
            radius = 9;
            alpha = 0.8;
            gray = 0.8;
        }

        rc_->set_source_rgba(gray, gray, gray, alpha);
        rc_->arc(fc.cursor_x, fc.cursor_y, radius, 0, 2 * m_pi);
        rc_->fill();

        rc_->set_source_rgba(1, 0, 0, alpha);
        rc_->arc(fc.cursor_x, fc.cursor_y, radius, 0, 2 * m_pi);
        rc_->stroke();
    }
}



