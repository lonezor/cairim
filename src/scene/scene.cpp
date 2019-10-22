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
    double sz = 1;
    if (level_of_detail_ < 1000) {
        sz = 10;
    }

    for(int i=0; i<10 * (int)level_of_detail_;i++) {
        objects_.emplace_back(object(drand48() * width_, drand48() * height_, drand48()*0.5*sz, drand48()*0.5*sz, rc_));
    }
    std::cout << "Number of particles: " << objects_.size() << std::endl;
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


