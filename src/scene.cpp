#include "scene.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <iostream>

scene::scene(double width, double height, std::shared_ptr<rendering_context> rc,
                uint64_t level_of_detail)
: width_(width)
, height_(height)
, rc_(rc)
, level_of_detail_(level_of_detail)
{

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
    std::cout << "scene::draw_main()" << std::endl;
}

void scene::draw_fg(frame_context& fc)
{
    // On screen display
    if (fc.osd_visible) {
        rc_->set_source_rgba(1, 1, 1, 1);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0);
        oss << "fps: " << fc.frame_rate;
        oss << ", elapsed time: " << elapsed_time_str(fc.timestamp);

        rc_->move_to(10,15);
        rc_->font_size(12);
        rc_->show_text(oss.str());
    }

    if (fc.screen_border) {

        auto offset = rc_->scale(0.01);
        auto one_unit = rc_->scale(1.1);// 8k: 0.3

        rc_->set_source_rgba(1, 1, 1, 1);
        rc_->move_to(offset,offset);
        rc_->line_to(fc.scene_width - offset, offset);
        rc_->line_to(fc.scene_width - offset,fc.scene_height - one_unit - offset);
        rc_->line_to(offset,fc.scene_height - one_unit - offset);
        rc_->line_to(offset,offset);
        rc_->stroke();
    }

    // Mouse cursor
    if (fc.cursor_visible) {
        double radius = 8;
        double alpha = 0.5;
        double gray = 0.4;
        if (button_active(fc.button_state, button::left)) {
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



std::string scene::elapsed_time_str(int64_t elapsed_time) {
    elapsed_time /= 1000000; // to seconds

    auto days = elapsed_time / (3600 * 24);
    elapsed_time -= days * (3600 * 24);
    
    auto hours = elapsed_time / 3600;
    elapsed_time -= hours * (3600);

    auto minutes = elapsed_time / 60;
    auto seconds = elapsed_time % 60;

    std::ostringstream oss;
    oss << std::setfill('0');
    oss << std::setw(2);
    oss << days << ":";
    oss << std::setw(2);
    oss << hours << ":";
    oss << std::setw(2);
    oss << minutes << ":";
    oss << std::setw(2);
    oss << seconds;

    return oss.str();
}

void scene::write_png(std::string path)
{
    rc_->write_png(path);
}