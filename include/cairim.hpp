#pragma once

#include "capture_handler.hpp"
#include "replay_handler.hpp"
#include "constants.hpp"
#include "png_generator.hpp"

int cairim_main(int argc, char* argv[], std::shared_ptr<cairo_xlib_window> window, 
    std::shared_ptr<scene> capture_scene, std::shared_ptr<scene> replay_scene,
    bool show_cursor);
