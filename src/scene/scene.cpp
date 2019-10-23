#include "scene/scene.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

scene::scene(double width, double height, std::shared_ptr<rendering_context> rc,
                double level_of_detail)
: width_(width)
, height_(height)
, rc_(rc)
, level_of_detail_(level_of_detail)
{
        objects_.emplace_back(object(width/2,height/2,width/2, height/2, rc_));
}

scene::~scene()
{

}

void scene::draw(frame_context& fc)
{
    rc_->set_source_rgb(0, 0, 0);
    rc_->paint();

    for(auto&& obj : objects_) {
        obj.draw(fc, width_, height_);
    }
}


