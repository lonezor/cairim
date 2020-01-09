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

#include "replay_handler.hpp"    

#include "cairo_image_surface.hpp"
#include "rendering_context.hpp"
#include "scene.hpp"
#include "common.hpp"

replay_handler::replay_handler(std::shared_ptr<scene> scene)
 : scene_(scene)
{

}

void replay_handler::run(std::vector<frame_context>& frame_ctx_vector)
{
    if (frame_ctx_vector.empty())
    {
        return;
    }

    std::cout << "Number of captured frames: " << frame_ctx_vector.size() << std::endl;

    // Replay
    int64_t capture_time = 0;
    int64_t replay_time = 0;
    size_t frame_number = 0;
    frame_context prev_frame_ctx;
    memset(&prev_frame_ctx, 0, sizeof(prev_frame_ctx));

    for (auto&& frame_ctx : frame_ctx_vector) {
        capture_time = frame_ctx.timestamp;
        auto accumulated_delta = capture_time - replay_time;

        // Compensate for clock drift
        if (accumulated_delta > frame_period) {
            // The capture timeline will never be identical to the perfect 1/fps frame period
            // The replay timeline must have the same length so introduce an extra frame context
            // by interpolating between current and previous frame context
            auto interpol_frame_ctx = interpolate_frame(prev_frame_ctx, frame_ctx);
            render_replay_frame(interpol_frame_ctx, frame_number++);
            replay_time += frame_period;
        }

        render_replay_frame(frame_ctx, frame_number++);
        replay_time += frame_period;

        prev_frame_ctx = frame_ctx;
    }
}


void replay_handler::render_replay_frame(frame_context& frame, size_t frame_number)
{
    // Disable foreground for replay
    frame.osd_visible = false;
    frame.screen_border = false;
    frame.cursor_visible = false;

    scene_->draw(frame);
    scene_->generate_png();
}

frame_context replay_handler::interpolate_frame(frame_context& prev_frame_ctx, frame_context& frame_ctx)
{
    // Previous frame has been processed. Instead of duplicating current frame context
    // the interpolation between the two contexts are used. This will ensure smooth
    // animation

    // First, inherit everything from current frame context
    frame_context ctx = frame_ctx;

    // Calculate coordinate delta
    auto x_delta = ctx.cursor_x - prev_frame_ctx.cursor_x;
    auto y_delta = ctx.cursor_y - prev_frame_ctx.cursor_y;

    // Use linear interpolation
    ctx.cursor_x = prev_frame_ctx.cursor_x + (x_delta / 2);
    ctx.cursor_y = prev_frame_ctx.cursor_y + (y_delta / 2);

    return ctx;
}