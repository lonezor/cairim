#include "replay_handler.hpp"    

#include "cairo_image_surface.hpp"
#include "rendering_context.hpp"
#include "scene.hpp"
#include "common.hpp"

replay_handler::replay_handler(std::shared_ptr<scene> scene)
 : scene_(scene)
{

}

void replay_handler::run(std::vector<frame_context>& frame_ctx_vector, std::string output_dir)
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
            render_replay_frame(interpol_frame_ctx, frame_number++, output_dir);
            replay_time += frame_period;
        }

        render_replay_frame(frame_ctx, frame_number++, output_dir);
        replay_time += frame_period;

        prev_frame_ctx = frame_ctx;
    }
}


void replay_handler::render_replay_frame(frame_context& frame, size_t frame_number, std::string output_dir)
{
    // Disable foreground for replay
    frame.osd_visible = false;
    frame.screen_border = false;
    frame.cursor_visible = false;

    scene_->draw(frame);

    std::ostringstream path;
                path << output_dir
                     << "/out_" 
                     << std::fixed << std::setw(6) << std::setfill('0') << frame_number << ".png";

    std::cout << std::fixed << std::setprecision(6)
                     << "Generating " << path.str()
                      << std::endl;

    scene_->write_png(path.str());
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