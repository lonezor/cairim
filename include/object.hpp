#pragma once

#include "rendering_context.hpp"
#include "frame_context.hpp"
#include "cairo_svg_surface.hpp"
#include <memory>

class object
{
public:
    object(double x, double y, double width, double height, std::shared_ptr<rendering_context> rc, std::string svg_path);
    virtual ~object() {}
    virtual void draw(frame_context& fc) = 0;

    

    virtual bool intersects(double x, double y);
   // virtual bool intersects(const object& other);

protected:
    void draw_svg_surface(double x_center, double y_center, double angle);

    double x_;
    double y_;
    double width_;
    double height_;
    std::shared_ptr<rendering_context> rc_;
    cairo_svg_surface* svg_surface_;
    std::string svg_path_;
    
};