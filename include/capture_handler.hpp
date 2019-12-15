#pragma once

#include <vector>
#include <string>

#include "frame_context.hpp"
#include "common.hpp"
#include "scene.hpp"
#include "cairo_xlib_window.hpp"
#include "cairo_image_surface.hpp"
#include "rendering_context.hpp"

class capture_handler
{
public:
    capture_handler(std::shared_ptr<scene> scene,
                    std::shared_ptr<cairo_xlib_window> window,
                    std::shared_ptr<rendering_context> rc);

    std::vector<frame_context> run();

    void write_capture_file(std::vector<frame_context>& frame_ctx_vector, std::string path);
    std::vector<frame_context> read_capture_file(std::string path);
private:
    std::shared_ptr<scene> scene_;
    std::shared_ptr<cairo_xlib_window> window_;
    std::shared_ptr<rendering_context> rc_;
};