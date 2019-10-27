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
    ~scene();

    void draw(frame_context& fc);

    void write_png(std::string path);



private:
    std::shared_ptr<rendering_context> rc_;
    double width_;
    double height_;
    uint64_t level_of_detail_;

    void draw_begin();
    void draw_bg();
    void draw_main(frame_context& fc);
    void draw_fg(frame_context& fc);
    void draw_end();

    std::string elapsed_time_str(int64_t elapsed_time);
    

    std::vector<object> objects_;


    

};