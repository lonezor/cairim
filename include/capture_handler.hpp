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