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
    draw();
}

void event_loop::on_pointer_motion(window_event& e)
{
    cursor_x_ = static_cast<double>(e.get_x());
    cursor_y_ = static_cast<double>(e.get_y());
}

void event_loop::on_button_press(window_event& e)
{
    button_state_ = e.get_button_state();
}

void event_loop::on_button_release(window_event& e)
{
    button_state_ = e.get_button_state();
}

void event_loop::on_key_press(window_event& e)
{
    auto c = e.get_c();

    key_state_ = c;

    if (c == 27) { // ESC
        std::cout << "User requested stop" << std::endl;
        exit_ = true;
    }

}

void event_loop::on_key_release(window_event& e)
{
    key_state_ = 0;
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

void event_loop::draw_scene(frame_context& fc)
{
    fc.screen_border = false;
    
    scene_->draw(fc);
    
    fc.osd_visible = true;
    fc.screen_border = true;
    capture_vector_.emplace_back(fc);
}

void event_loop::draw()
{
    frame_context fc;
    memset(&fc, 0, sizeof(fc));

    auto ts1 = get_ts();

    fc.timestamp = ts1 - started_ts_;
    fc.cursor_x = cursor_x_;
    fc.cursor_y = cursor_y_;
    fc.cursor_visible = cursor_visible_;
    fc.osd_visible = true;
    fc.screen_border = true;
    fc.button_state = button_state_;
    fc.key_state = key_state_;
    fc.frame_rate = frame_rate_;

    draw_scene(fc);

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

int64_t event_loop::get_ts()
{
    auto now = std::chrono::steady_clock::now();
    return static_cast<int64_t>(std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count());
}
