#include "capture_handler.hpp"

#include "event_loop.hpp"
#include "cairo_xlib_window.hpp"
#include "cairo_image_surface.hpp"
#include "rendering_context.hpp"
#include "scene/scene.hpp"
#include "common.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <iostream>

capture_handler::capture_handler()
{

}

std::vector<frame_context> capture_handler::run()
{
    // Create main window
    auto window = std::shared_ptr<cairo_xlib_window>(new cairo_xlib_window(capture_width,
                                                                           capture_height,
                                                                           window_xpos,
                                                                           window_ypos,
                                                                           window_title,
                                                                           false));

    // Define rendering context
    auto rc = std::shared_ptr<rendering_context>(new rendering_context(window->get_surface(),
                                                                       window->get_context(),
                                                                       capture_width,
                                                                       capture_height,
                                                                       scale_ref_width,
                                                                       scale_ref_height,
                                                                       anti_aliasing::fast));

    // Create capture scene
    auto wma_capture_scene = std::shared_ptr<scene>(new scene(capture_width,
                                                              capture_height,
                                                              rc,
                                                              level_of_detail_capture));

    // Connect everything together in an event loop
    auto eloop = event_loop(target_frame_rate,
                            input_dev_poll_rate,
                            window,
                            rc,
                            wma_capture_scene);

    // Event loop (blocking)
    auto capture_vector = eloop.run();

    // Close window after user has requested to exit
    window->close();

    return capture_vector;
}

