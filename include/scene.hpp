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

#include <memory>
#include "scene.hpp"
#include "rendering_context.hpp"
#include "object.hpp"
#include "frame_context.hpp"

#include <vector>

class scene
{
    public:
        scene(double width, double height, std::shared_ptr<rendering_context> rc, uint64_t level_of_detail);
        virtual ~scene();

        void draw(frame_context& fc);
        void generate_png();
    protected:
        std::shared_ptr<rendering_context> rc_;
        double width_;
        double height_;
        uint64_t level_of_detail_;

        void draw_begin();
        void draw_bg();
        
        void draw_fg(frame_context& fc);
        void draw_end();

        std::string elapsed_time_str(int64_t elapsed_time);

        virtual void draw_main(frame_context& fc);

};