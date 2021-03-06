/*
 *  Cairim
 *  Copyright (C) 2020 Johan Norberg <lonezor@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>
 */

#pragma once

#include <memory>
#include "timer.hpp"
#include "cairo_xlib_window.hpp"
#include "rendering_context.hpp"
#include "scene.hpp"
#include "button.hpp"

#include <unordered_map>
#include <chrono>
#include <stdint.h>

enum class event_type
{
    poll_input_device,
    refresh_screen,
};

using event_id = uint64_t;

class event_loop
{
    public:
        event_loop(double target_frame_rate,
                   double window_event_poll_rate,
                   std::shared_ptr<cairo_xlib_window> window,
                   std::shared_ptr<rendering_context> rc,
                   std::shared_ptr<scene> scene);
        ~event_loop();

        std::vector<frame_context> run();

private:

    void register_refresh_timer();
    void register_input_device_timer();
    void on_poll_input_device();
    void on_refresh_screen();

    void on_expose(window_event& e);
    void on_pointer_motion(window_event& e);
    void on_button_press(window_event& e);
    void on_button_release(window_event& e);
    void on_key_press(window_event& e);
    void on_key_release(window_event& e);
    void on_focus_in(window_event& e);
    void on_focus_out(window_event& e);
    void on_enter(window_event& e);
    void on_leave(window_event& e);
    void on_close(window_event& e);

    void draw();
    void draw_scene(frame_context& fc);

    std::chrono::milliseconds rate_to_ms_period(double rate);

    int64_t get_ts();

    std::shared_ptr<cairo_xlib_window> window_;
    std::shared_ptr<rendering_context> rc_;
    std::shared_ptr<scene> scene_;
    std::unordered_map<event_id,event_type> event_map_;
    std::vector<frame_context> capture_vector_;
    timer timer_;
    double target_frame_rate_;
    double frame_rate_{0};
    double window_event_poll_rate_;
    button button_state_{button::none};
    char key_state_{0};
    double cursor_x_{0};
    double cursor_y_{0};
    bool exit_{false};
    bool cursor_visible_{false};
    
    uint64_t frame_{0};
    int64_t started_ts_{0};
    int64_t delta_time_{0};
    int64_t total_time_{0};

};