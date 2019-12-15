#pragma once

#include <vector>
#include "frame_context.hpp"
#include "common.hpp"
#include "scene.hpp"

class replay_handler
{
public:
    replay_handler(std::shared_ptr<scene> scene);

    void run(std::vector<frame_context>& frame_ctx_vector, std::string output_dir);

private:
    void render_replay_frame(frame_context& frame, size_t frame_number, std::string output_dir);
    frame_context interpolate_frame(frame_context& prev_frame_ctx, frame_context& frame_ctx);
    std::shared_ptr<scene> scene_;
};