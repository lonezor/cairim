#pragma once

#include <stdint.h>

// Note: capture size must have the same aspect ratio as replay size
constexpr int capture_width = 720;
constexpr int capture_height = 408;
constexpr int window_xpos = 100;
constexpr int window_ypos = 100;
constexpr auto window_title = "Water Mill Animation";

// The scaling size is conceptually detached from capture size to allow
// a scenario with rendering without captured user input
constexpr int scale_ref_width = 720;
constexpr int scale_ref_height = 408;

constexpr int replay_width_1k = 1280;
constexpr int replay_width_2k = 1920;
constexpr int replay_width_4k = 3840;
constexpr int replay_width_8k = 7680;

constexpr int replay_height_1k = 720;
constexpr int replay_height_2k = 1080;
constexpr int replay_height_4k = 2160;
constexpr int replay_height_8k = 4320;

constexpr int target_frame_rate = 60;
constexpr int input_dev_poll_rate = target_frame_rate * 2;
constexpr int64_t frame_period = 1000000 / target_frame_rate;

constexpr uint64_t level_of_detail_min = 1;
constexpr uint64_t level_of_detail_max = 1000000;
constexpr uint64_t level_of_detail_capture = 1000;
constexpr uint64_t level_of_detail_replay = 500000;