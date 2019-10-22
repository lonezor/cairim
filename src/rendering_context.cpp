#include "rendering_context.hpp"

rendering_context::rendering_context(cairo_t* cr, int screen_width, int screen_height, int ref_width, int ref_height, anti_aliasing anti_aliasing)
 : cr_(cr)
 , screen_width_(screen_width)
 , screen_height_(screen_height)
 , ref_width_(ref_width)
 , ref_height_(ref_height)
{
    cairo_antialias_t cr_antialias;
    switch(anti_aliasing) {
        case anti_aliasing::none:
            cr_antialias = CAIRO_ANTIALIAS_NONE;
            break;
        case anti_aliasing::fast:
            cr_antialias = CAIRO_ANTIALIAS_FAST;
            break;
        case anti_aliasing::best:
            cr_antialias = CAIRO_ANTIALIAS_BEST;
            break;
    }
    cairo_set_antialias(cr_, cr_antialias);

    font_face("DejaVu Sans Book", font_slant::normal, font_weight::normal);
}

void rendering_context::set_source_rgb(double r, double g, double b)
{
    cairo_set_source_rgb(cr_, r, g, b);
}

void rendering_context::set_source_rgba(double r, double g, double b, double a)
{
    cairo_set_source_rgba(cr_, r, g, b, a);
}

void rendering_context::line_width(double width)
{
    cairo_set_line_width(cr_, scale(width));
}

void rendering_context::font_size(double size)
{
    cairo_set_font_size(cr_, scale(size));
}

void rendering_context::font_face(std::string name, font_slant slant, font_weight weight)
{
    cairo_font_slant_t cr_slant;
    switch(slant)
    {
        case font_slant::normal:
            cr_slant = CAIRO_FONT_SLANT_NORMAL;
            break;
        case font_slant::italic:
            cr_slant = CAIRO_FONT_SLANT_ITALIC;
            break;
        case font_slant::oblique:
            cr_slant = CAIRO_FONT_SLANT_OBLIQUE;
            break;
    }

    cairo_font_weight_t cr_weight;
    switch(weight)
    {
        case font_weight::normal:
            cr_weight = CAIRO_FONT_WEIGHT_NORMAL;
            break;
        case font_weight::bold:
            cr_weight = CAIRO_FONT_WEIGHT_BOLD;
            break;
    }
     
    cairo_select_font_face(cr_, name.c_str(), cr_slant, cr_weight);
}

void rendering_context::move_to(double x, double y)
{
    cairo_move_to(cr_, scale(x), scale(y));
}

void rendering_context::line_to(double x, double y)
{
     cairo_line_to(cr_, scale(x), scale(y));
}

void rendering_context::close_path()
{
    cairo_close_path(cr_);
}

void rendering_context::arc(double xc, double yc, double radius, double angle1, double angle2)
{
    cairo_arc(cr_, scale(xc), scale(yc), scale(radius), angle1, angle2);
}

void rendering_context::rectangle(double x, double y, double width, double height)
{
    cairo_rectangle(cr_, scale(x), scale(y), scale(width), scale(height));
}

void rendering_context::show_text(std::string text)
{
    cairo_show_text(cr_, text.c_str());
}

void rendering_context::fill()
{
    cairo_fill(cr_);
}

void rendering_context::stroke()
{
    cairo_stroke(cr_);
}

void rendering_context::paint()
{
     cairo_paint(cr_);
}

void rendering_context::push_group()
{
    cairo_push_group(cr_);
}

void rendering_context::pop_group_to_source()
{
    cairo_pop_group_to_source(cr_);
}

void rendering_context::write_png(std::string path)
{

}

double rendering_context::scale(double value)
{
    // Assumption: using the same aspect ratio
    double multiplier = screen_width_ / ref_width_;
    return value * multiplier;
}