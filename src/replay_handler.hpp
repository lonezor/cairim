#pragma once

#include <vector>
#include "frame_context.hpp"
#include "common.hpp"
#include "scene.hpp"

class replay_handler
{
public:
    replay_handler();

    void run(std::vector<frame_context>& frame_ctx_vector, int replay_width, int replay_height, std::string output_dir);

private:
    void render_replay_frame(std::shared_ptr<scene> scene, frame_context& frame, size_t frame_number, std::string output_dir);
    frame_context interpolate_frame(frame_context& prev_frame_ctx, frame_context& frame_ctx);
};