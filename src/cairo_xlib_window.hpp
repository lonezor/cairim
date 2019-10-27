#pragma once


#include <stdint.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <vector>
#include <memory>
#include <unistd.h>
#include <stdint.h>
#include "button.hpp"

enum class window_event_type
{
    none,
    expose,
    pointer_motion,
    button_press,
    button_release,
    key_press,
    key_release,
    focus_in,
    focus_out,
    enter,
    leave,
    close,
};





class window_event
{
public:
   window_event(window_event_type type) : type_(type) {}
  
   window_event(window_event_type type, int x, int y)
   : type_(type)
   , x_(x)
   , y_(y)
   {}

   window_event(window_event_type type, int x, int y, button button_state)
   : type_(type)
   , x_(x)
   , y_(y)
   , button_state_(button_state)
   {}
    
   window_event(window_event_type type, char c)
   : type_(type)
   , c_(c)
   {}

   window_event_type get_type() { return type_;}
   int get_x() { return x_;}
   int get_y() { return y_;}
   char get_c() { return c_;}
   button get_button_state() { return button_state_; }

private:
    window_event_type type_{window_event_type::none};
    int x_{-1};
    int y_{-1};
    char c_{0};
    button button_state_{button::none};
};

/*
Hierarcy:
display -> screen -> root window -> window -> cairo surface -> cairo context

*/
class cairo_xlib_window
{
public:
    cairo_xlib_window(int width, int height, int xpos, int ypos, std::string title, bool fullscreen);

    ~cairo_xlib_window();

    cairo_t* get_context();
    cairo_surface_t* get_surface();

    std::vector<std::shared_ptr<window_event>> poll_events();

    void button_event(button flag, bool pressed);

    void close();

private:
    int width_;
    int height_;
    int xpos_;
    int ypos_;
    std::string title_;
    bool fullscreen_;
    Display* display_{nullptr};
    int screen_{-1};
    Window window_{0};
    Atom wm_delete_{0};
    cairo_surface_t* surface_{nullptr};
    cairo_t* ctx_{nullptr};
    bool closed_{false};
    button button_state_{button::none};
};