#include "capture_handler.hpp"

#include "event_loop.hpp"
#include "scene.hpp"
#include "common.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <iostream>

capture_handler::capture_handler(std::shared_ptr<scene> scene,
                                 std::shared_ptr<cairo_xlib_window> window,
                                 std::shared_ptr<rendering_context> rc)
 : scene_(scene)
 , window_(window)
 , rc_(rc)
{

}

std::vector<frame_context> capture_handler::run()
{
 
    // Connect everything together in an event loop
    auto eloop = event_loop(target_frame_rate,
                            input_dev_poll_rate,
                            window_,
                            rc_,
                            scene_);

    // Event loop (blocking)
    auto capture_vector = eloop.run();

    // Close window after user has requested to exit
    window_->close();

    return capture_vector;
}

