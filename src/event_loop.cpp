#include "event_loop.hpp"
#include "rendering_context.hpp"

#include <stdint.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <iostream>

event_loop::event_loop(double target_frame_rate,
                       double window_event_poll_rate,
                       std::shared_ptr<cairo_xlib_window> window,
                       std::shared_ptr<rendering_context> rc,
                       std::shared_ptr<scene> scene)
                       : target_frame_rate_(target_frame_rate)
                       , window_event_poll_rate_(window_event_poll_rate)
                       , window_(window)
                       , rc_(rc)
                       , scene_(scene)
{
    // Initial framerate
    frame_rate_ = target_frame_rate_;
    started_ts_ = get_ts();

    register_input_device_timer();
    register_refresh_timer();
    register_timestamp_timer();

    

  
}

event_loop::~event_loop()
{

}

std::vector<frame_context> event_loop::run()
{
    while(!exit_) {
        auto events = timer_.wait_for_events();

        for (auto&& event : events) {
            auto id = event->get_id();
            auto ev_type = event_map_[id];
            if (ev_type == event_type::refresh_screen) {
                on_refresh_screen();
            }
            else if (ev_type == event_type::poll_input_device) {
                on_poll_input_device();
            }
            else if (ev_type == event_type::refresh_timestamp) {
                on_refresh_timestamp();
            }
            event_map_.erase(id);
        }
    }

    return capture_vector_;
}

std::chrono::milliseconds event_loop::rate_to_ms_period(double rate)
{
    double period_ms = 1000.0 / rate;
    return std::chrono::milliseconds(static_cast<size_t>(period_ms));
}

void event_loop::register_refresh_timer()
{
    auto id = timer_.register_one_shot_timer(rate_to_ms_period(frame_rate_));
    event_map_[id] = event_type::refresh_screen;
}

void event_loop::register_input_device_timer()
{
    auto id = timer_.register_one_shot_timer(rate_to_ms_period(window_event_poll_rate_));
    event_map_[id] = event_type::poll_input_device;
}

void event_loop::register_timestamp_timer()
{
    auto id = timer_.register_one_shot_timer(rate_to_ms_period(2));
    event_map_[id] = event_type::refresh_timestamp;
}

void event_loop::on_refresh_timestamp()
{
    auto now = get_ts();
    auto diff = static_cast<int64_t>(now - started_ts_);
    diff /= 1000000;

    auto days = diff / (3600 * 24);
    diff -= days * (3600 * 24);
    
    auto hours = diff / 3600;
    diff -= hours * (3600);

    auto minutes = diff / 60;
    auto seconds = diff % 60;

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

    elapsed_time_ = oss.str();

    register_timestamp_timer();
}

void event_loop::on_poll_input_device()
{
    auto window_events = window_->poll_events();
    for (auto&& event : window_events) {
        switch(event->get_type()) {
            case window_event_type::expose:
                on_expose(*event);
                break;
            case window_event_type::pointer_motion:
                on_pointer_motion(*event);
                break;
            case window_event_type::button_press:
                on_button_press(*event);
                break;
            case window_event_type::button_release:
                on_button_release(*event);
                break;
            case window_event_type::key_press:
                on_key_press(*event);
                break;
            case window_event_type::key_release:
                on_key_release(*event);
                break;
            case window_event_type::focus_in:
                on_focus_in(*event);
                break;
            case window_event_type::focus_out:
                on_focus_out(*event);
                break;
            case window_event_type::enter:
                on_enter(*event);
                break;
            case window_event_type::leave:
                on_leave(*event);
                break;
            case window_event_type::close:
                on_close(*event);
                break;
            default:
                break;
        }
    }

    register_input_device_timer();
}

void event_loop::on_refresh_screen()
{
     // determine elapsed time, if we are blocking too much and timer is delayed
     // we need to reduce speed...

     draw();
     
     register_refresh_timer();
}

void event_loop::on_expose(window_event& e)
{
    printf("expose\n");
    draw();
}

void event_loop::on_pointer_motion(window_event& e)
{
    cursor_x_ = static_cast<double>(e.get_x());
    cursor_y_ = static_cast<double>(e.get_y());
}

void event_loop::on_button_press(window_event& e)
{
    button_pressed_ = true;
}

void event_loop::on_button_release(window_event& e)
{
    button_pressed_ = false;
}

void event_loop::on_key_press(window_event& e)
{
    auto c = e.get_c();

    if (c == 'q') {
        std::cout << "User requested stop" << std::endl;
        exit_ = true;
    }

}

void event_loop::on_key_release(window_event& e)
{

}

void event_loop::on_focus_in(window_event& e)
{
    std::cout << "Focus in" << std::endl;
}

void event_loop::on_focus_out(window_event& e)
{
    std::cout << "Focus out" << std::endl;
}

void event_loop::on_enter(window_event& e)
{
    std::cout << "Enter" << std::endl;
    cursor_visible_ = true;
}

void event_loop::on_leave(window_event& e)
{
    std::cout << "Leave" << std::endl;
    cursor_visible_ = false;
}

void event_loop::on_close(window_event& e)
{
    exit_ = true;
}

void event_loop::draw_background()
{
    // Black background
    rc_->set_source_rgb(0, 0, 0);
    rc_->paint();
}

void event_loop::draw_main()
{
    frame_context fc;
    memset(&fc, 0, sizeof(fc));

    fc.frame = frame_;
    fc.delta_time = delta_time_;
    fc.cursor_x = cursor_x_;
    fc.cursor_y = cursor_y_;
    fc.button_pressed = button_pressed_;
    
    scene_->draw(fc);
    capture_vector_.emplace_back(fc);
}

void event_loop::draw_foreground()
{
    // Debug info
    rc_->set_source_rgba(1, 1, 1, 1);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0);
    oss << "fps: " << frame_rate_;
    oss << ", elapsed time: " << elapsed_time_;
    
    rc_->move_to(10,15);
    rc_->font_size(12);
    rc_->show_text(oss.str());

    // Mouse cursor
    if (cursor_visible_) {
        double radius = 8;
        double alpha = 0.5;
        double gray = 0.4;
        if (button_pressed_) {
            radius = 9;
            alpha = 0.8;
            gray = 0.8;
        }

        rc_->set_source_rgba(gray, gray, gray, alpha);
        rc_->arc(cursor_x_, cursor_y_, radius, 0, 2 * m_pi);
        rc_->fill();

        rc_->set_source_rgba(1, 0, 0, alpha);
        rc_->arc(cursor_x_, cursor_y_, radius, 0, 2 * m_pi);
        rc_->stroke();


    }
}

void event_loop::draw()
{
    int64_t ts1 = get_ts();
    
    rc_->push_group();

    draw_background();
    draw_main();
    draw_foreground(); // OSD and cursor

    rc_->pop_group_to_source();
    rc_->paint();

    auto ts2 = get_ts();
    auto diff = ts2 - ts1;

    double expected_period = 1000000.0 / frame_rate_;
    if (diff > expected_period * 1.1) {
        frame_rate_ /= diff / expected_period;
            if (frame_rate_ < 1) {
                frame_rate_ = 1;
            }
        } else {
            if (frame_rate_ < target_frame_rate_) {
                frame_rate_ *= expected_period / diff;
            }
            if (frame_rate_ > target_frame_rate_) {
                frame_rate_ = target_frame_rate_;
            }
        }

    frame_++;
    delta_time_ = diff;
}

double event_loop::get_ts()
{
    auto now = std::chrono::steady_clock::now();
    return static_cast<double>(std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count());
}
