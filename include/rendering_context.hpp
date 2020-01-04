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
    rendering_context(cairo_surface_t* surface, cairo_t* cr, int screen_width, int screen_height, int ref_width, int ref_height, anti_aliasing anti_aliasing);
    rendering_context(cairo_surface_t* surface, cairo_t* cr, rendering_context& reference);
    
    void init(anti_aliasing anti_aliasing);

    cairo_surface_t* get_surface();
    cairo_t* get_ctx();
    int get_screen_width();
    int get_screen_height();
    int get_ref_width();
    int get_ref_height();
    anti_aliasing get_anti_aliasing();

    // State
    void save();
    void restore();
    
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

    // Transformations
    void rotate(double angle);
    void translate (double tx, double ty);
    void matrix_init(cairo_matrix_t* matrix,
                        double xx,
                        double yx,
                        double xy,
                        double yy,
                        double x0,
                        double y0);

    void matrix_translate (cairo_matrix_t* matrix,
                            double tx,
                            double ty);
    void matrix_rotate (cairo_matrix_t* matrix,
                        double radians);
    void transform(const cairo_matrix_t* matrix);

    // Drawing
    void fill();
    void stroke();
    void paint();
    void push_group();
    void pop_group_to_source();

    // Surface operations
    void draw_rc(rendering_context& rc, double x, double y);
    void draw_surface(cairo_surface_t* surface, double x, double y, double alpha);

    void write_png(std::string path);

    double scale(double value);

private:
    cairo_t* cr_{nullptr};
    cairo_surface_t* surface_{nullptr};
    int screen_width_;
    int screen_height_;
    int ref_width_;
    int ref_height_;
    anti_aliasing anti_aliasing_;
};
