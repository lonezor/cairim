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
    scene(double width, double height, std::shared_ptr<rendering_context> rc, double level_of_detail);
    ~scene();

    void draw(frame_context& fc);

private:
    std::shared_ptr<rendering_context> rc_;
    double width_;
    double height_;
    double level_of_detail_;

    std::vector<object> objects_;
    

};