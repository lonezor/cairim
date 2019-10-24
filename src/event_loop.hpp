#pragma once

#include <memory>
#include "timer.hpp"
#include "cairo_xlib_window.hpp"
#include "rendering_context.hpp"
#include "scene.hpp"

#include <unordered_map>
#include <chrono>
#include <stdint.h>

enum class event_type
{
    poll_input_device,
    refresh_screen,
    refresh_timestamp,
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
    void register_timestamp_timer();
    void on_poll_input_device();
    void on_refresh_screen();
    void on_refresh_timestamp();

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

    std::shared_ptr<cairo_xlib_window> window_;
    std::shared_ptr<rendering_context> rc_;
    std::shared_ptr<scene> scene_;
    std::unordered_map<event_id,event_type> event_map_;
    std::vector<frame_context> capture_vector_;
    timer timer_;
    double target_frame_rate_;
    double frame_rate_{0};
    double window_event_poll_rate_;
    bool button_pressed_{false};
    double cursor_x_{0};
    double cursor_y_{0};
    bool exit_{false};
    bool cursor_visible_{false};
    
    int64_t started_ts_{0};
    std::string elapsed_time_;

    double get_ts();
    uint64_t frame_{0};
    double delta_time_{0};

};