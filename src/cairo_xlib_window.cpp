#include "cairo_xlib_window.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/X.h>

cairo_xlib_window::cairo_xlib_window(int width, int height, int xpos, int ypos, std::string title, bool fullscreen)
: width_(width)
, height_(height)
, xpos_(xpos)
, ypos_(ypos)
, title_(title)
, fullscreen_(fullscreen)
{
    // Create window using default screen
    display_ = XOpenDisplay("");
    
    screen_ = DefaultScreen(display_);

    XSizeHints hints;
    memset(&hints, 0, sizeof(hints));
    hints.x = xpos;
    hints.y = ypos;
    hints.width = width;
    hints.height = height;
    hints.flags = PPosition|PSize;

    auto background = BlackPixel(display_, screen_);
    auto foreground = WhitePixel(display_, screen_);

    window_ = XCreateSimpleWindow(display_, DefaultRootWindow(display_),
                                 hints.x, hints.y,
                                 hints.width, hints.height,
                                 5, foreground, background);

    XSetStandardProperties(display_, window_, title.c_str(), title.c_str(),
                         None, NULL, 0, &hints);

    // Connect cairo xlib surface to window
    surface_ = cairo_xlib_surface_create(display_, window_, DefaultVisual(display_, screen_), width, height);
    cairo_xlib_surface_set_size(surface_, width, height);
    ctx_ = cairo_create(surface_);

    // Subscribe to input events
    long event_mask = ExposureMask;
    event_mask |= PointerMotionMask;
    event_mask |= ButtonPressMask;
    event_mask |= ButtonReleaseMask;
    event_mask |= KeyPressMask;
    event_mask |= KeyReleaseMask;
    event_mask |= FocusChangeMask;
    event_mask |= EnterWindowMask;
    event_mask |= LeaveWindowMask;
    XSelectInput(display_,window_,event_mask);

    // Raise window
    XMapRaised(display_, window_);
 
    if (fullscreen) {
        // Enable fullscreen
        Atom atoms[2] = { XInternAtom(display_, "_NET_WM_STATE_FULLSCREEN", False), None };
        XChangeProperty(display_, window_, XInternAtom(display_, "_NET_WM_STATE", False),
                    XA_ATOM, 32, PropModeReplace, (unsigned char*) atoms, 1);
        fullscreen_ = true;
    } else {
        // Subscribe to delete window event
        wm_delete_ = XInternAtom(display_, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(display_, window_, &wm_delete_, 1);
    }

    // Disable default cursor
    Cursor invisible_cursor;
    Pixmap bitmap;
    XColor black;
    memset(&black, 0, sizeof(black));
    static char no_data[] = { 0,0,0,0,0,0,0,0 };

    bitmap = XCreateBitmapFromData(display_, window_, no_data, 8, 8);
    invisible_cursor = XCreatePixmapCursor(display_, bitmap, bitmap, 
                                     &black, &black, 0, 0);
    XDefineCursor(display_,window_, invisible_cursor);
    XFreeCursor(display_, invisible_cursor);
    XFreePixmap(display_, bitmap);
}

cairo_xlib_window::~cairo_xlib_window()
{
    if (!closed_) {
        close();
    }
}

cairo_t* cairo_xlib_window::get_context()
{
    return ctx_;
}

std::vector<std::shared_ptr<window_event>> cairo_xlib_window::poll_events()
{
  XEvent event;
  memset(&event, 0, sizeof(event));

  KeySym key;
  memset(&key, 0, sizeof(key));

  char text[10];
  memset(text, 0, sizeof(text));

  std::vector<std::shared_ptr<window_event>> events;

  while(true) {
      auto event_in_queue = XCheckMaskEvent(display_, -1, &event);
      if (!event_in_queue) {
          break;
      }

      switch(event.type) {
        case Expose: {
            auto e = std::shared_ptr<window_event>(new window_event(window_event_type::expose));
            events.emplace_back(e);
            break;
        }
        case MotionNotify: {
            auto e = std::shared_ptr<window_event>(new window_event(window_event_type::pointer_motion,
                                  event.xbutton.x,
                                  event.xbutton.y));
            events.emplace_back(e);
            break;
        }
        case ButtonPress: {
            auto e = std::shared_ptr<window_event>(new window_event(window_event_type::button_press,
                                  event.xbutton.x,
                                  event.xbutton.y));
            events.emplace_back(e);
            break;
        }
        case ButtonRelease: {
            auto e = std::shared_ptr<window_event>(new window_event(window_event_type::button_release,
                                  event.xbutton.x,
                                  event.xbutton.y));
            events.emplace_back(e);
            break;
        }
        case KeyPress: {
            auto i = XLookupString((XKeyEvent*)&event, text, sizeof(text), &key, 0);
            if (i == 1) {
                auto e = std::shared_ptr<window_event>(new window_event(window_event_type::key_press,
                                      text[0]));
                events.emplace_back(e);
            }
            break;
        }
        case KeyRelease: {
            auto i = XLookupString((XKeyEvent*)&event, text, sizeof(text), &key, 0);
            if (i == 1) {
                auto e = std::shared_ptr<window_event>(new window_event(window_event_type::key_release,
                                      text[0]));
                events.emplace_back(e);
            }
            break;
        }
        case FocusIn: {
            auto e = std::shared_ptr<window_event>(new window_event(window_event_type::focus_in));
            events.emplace_back(e);
            break;
        }
        case FocusOut: {
            auto e = std::shared_ptr<window_event>(new window_event(window_event_type::focus_out));
            events.emplace_back(e);
            break;
        }
        case EnterNotify: {
            auto e = std::shared_ptr<window_event>(new window_event(window_event_type::enter));
            events.emplace_back(e);
            break;
        }
        case LeaveNotify: {
            auto e = std::shared_ptr<window_event>(new window_event(window_event_type::leave));
            events.emplace_back(e);
            break;
        }
        case ClientMessage: // TODO: currently is acquired only with XNextEvent() but not XCheckWindowEvent()
            if (event.xclient.data.l[0] == wm_delete_) {
                auto e = std::shared_ptr<window_event>(new window_event(window_event_type::close,
                                      text[0]));
                events.emplace_back(e);
            }
        default:
            break;
    }
  }
    return events;
}

void cairo_xlib_window::close()
{
    if (!closed_) {

        XDestroyWindow(display_, window_);
        XCloseDisplay(display_);
        cairo_destroy(ctx_);
        cairo_surface_destroy(surface_);
        closed_ = true;
    }
}
