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

#include <png.h>

#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <stdint.h>
#include <cairo.h>

enum png_compression_level
{
    png_compression_level_none = 0,
    png_compression_level_min = 1,
    png_compression_level_good = 6,
    png_compression_level_max = 9,
};

class cairo_surface
{
    public:
        cairo_surface(uint8_t* data, size_t buffer_size, int width,
                      int height, int stride, cairo_format_t format);

        std::vector<uint8_t> buffer_;
        size_t buffer_size_;
        int width_;
        int height_;
        int stride_;
        cairo_format_t format_;
};

class png_generator
{
    public:
        png_generator();
        png_generator(size_t concurrency, std::string output_dir, png_compression_level level);
        ~png_generator();

        void append_work_queue(cairo_surface_t* surface);

        size_t get_queue_size();
        size_t get_concurrency();

    private:
        void worker_main(int tid);
        
        bool worker_exit_{false};

        size_t concurrency_;
        std::queue<std::shared_ptr<cairo_surface>> work_queue_;
        std::mutex mutex_;
        std::vector<std::unique_ptr<std::thread>> threads_;
        png_compression_level level_;
        std::string output_dir_;
        size_t file_idx_{0};

};