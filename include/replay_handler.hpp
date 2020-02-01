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
#include "frame_context.hpp"
#include "common.hpp"
#include "scene.hpp"

class replay_handler
{
public:
    replay_handler(std::shared_ptr<scene> scene, bool cursor_visible);

    void run(std::vector<frame_context>& frame_ctx_vector);

private:
    void render_replay_frame(frame_context& frame, size_t frame_number);
    frame_context interpolate_frame(frame_context& prev_frame_ctx, frame_context& frame_ctx);
    std::shared_ptr<scene> scene_;
    bool cursor_visible_;
};