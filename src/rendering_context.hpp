
#pragma once

#include <cairo.h>
#include <string>

enum class anti_aliasing
{
    none,
    fast,
    best

};

enum class font_slant
{
    normal,
    italic,
    oblique,
};

enum class font_weight
{
    normal,
    bold,
};

constexpr double m_pi = 3.14159265358979323846;

class rendering_context
{
public:
    rendering_context(cairo_t* cr, int screen_width, int screen_height, int ref_width, int ref_height, anti_aliasing anti_aliasing);

    // Attributes
    void set_source_rgb(double r, double g, double b);
    void set_source_rgba(double r, double g, double b, double a);
    void line_width(double width);
    void font_size(double size);
    void font_face(std::string name, font_slant slant, font_weight weight);

    // Shapes
    void move_to(double x, double y);
    void line_to(double x, double y);
    void close_path();
    void arc(double xc, double yc, double radius, double angle1, double angle2);
    void rectangle(double x, double y, double width, double height);

    // Text
    void show_text(std::string text);

    // Drawing
    void fill();
    void stroke();
    void paint();
    void push_group();
    void pop_group_to_source();

    // Image file generation
    void write_png(std::string path);

private:
    cairo_t* cr_;
    int screen_width_;
    int screen_height_;
    int ref_width_;
    int ref_height_;

    double scale(double value);

};