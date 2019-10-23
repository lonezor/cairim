#include <cairo.h>

#include "test.hpp"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define SIZE 100
#define WIDTH 720
#define HEIGHT 480
#define PAD 2

#define M_PI (3.1415926535)

static cairo_pattern_t *
create_image_source (int size)
{
    cairo_surface_t *surface;
    cairo_pattern_t *pattern;
    cairo_t *cr;

    /* Create an image surface with my favorite four colors in each
     * quadrant. */
    surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, size, size);
    cr = cairo_create (surface);
    cairo_surface_destroy (surface);

    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_rectangle (cr, 0, 0, size / 2, size / 2);
    cairo_fill (cr);

    cairo_set_source_rgb (cr, 1, 0, 0);
    cairo_rectangle (cr, size / 2, 0, size - size / 2, size / 2);
    cairo_fill (cr);

    cairo_set_source_rgb (cr, 0, 1, 0);
    cairo_rectangle (cr, 0, size / 2, size / 2, size - size / 2);
    cairo_fill (cr);

    cairo_set_source_rgb (cr, 0, 0, 1);
    cairo_rectangle (cr, size / 2, size / 2, size - size / 2, size - size / 2);
    cairo_fill (cr);

    pattern = cairo_pattern_create_for_surface (cairo_get_target (cr));
    cairo_destroy (cr);

    return pattern;
}

void
tdraw(cairo_t *cr, int width, int height)
{
    cairo_pattern_t *source;
    int surface_size = sqrt ((SIZE - 2*PAD)*(SIZE - 2*PAD)/2);

    /* Use a gray (neutral) background, so we can spot if the backend pads
     * with any other colour.
     */
    cairo_set_source_rgb (cr, .5, .5, .5);
    cairo_paint (cr);

    cairo_translate(cr, SIZE/2, SIZE/2);
    cairo_rotate (cr, M_PI / 4.0);
    cairo_translate (cr, -surface_size/2, -surface_size/2);

    printf("before %f\n", (float)SIZE/2);
    printf("after %f\n", (float)-surface_size/2);
    
    sleep(1);

    source = create_image_source (surface_size);
    cairo_pattern_set_filter (source, CAIRO_FILTER_NEAREST);
    cairo_set_source(cr, source);
    cairo_pattern_destroy (source);

    cairo_paint (cr);

}