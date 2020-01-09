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

#include <stdint.h>

// The scaling size is conceptually detached from capture size to allow
// a scenario with rendering without captured user input
constexpr int scale_ref_width = 1600;
constexpr int scale_ref_height = 900;

// Note: capture size must have the same aspect ratio as replay size
constexpr int capture_width = scale_ref_width;
constexpr int capture_height = scale_ref_height;
constexpr int window_xpos = 100;
constexpr int window_ypos = 100;
constexpr auto window_title = "Cairim";

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

constexpr auto default_output_dir = "/tmp/cairim/";
constexpr auto default_png_dir = "/home/png";
constexpr auto default_capture_file = "default.cap";

constexpr uint64_t capture_file_magic = 0xf5b24398771e0201;
