#include <stdio.h>
#include "slide.h"

void slide_draw(cairo_t *cr, int x, int y, int width, int height, const char *sldname)
{
    printf("+draw\n");
    printf(" draw: %p\n", cr);

    cairo_set_source_rgb(cr, 255, 0, 0);
    cairo_set_line_width(cr, 1);

    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x + width, y);
    cairo_line_to(cr, x + width, y + height);
    cairo_line_to(cr, x, y + height);
    cairo_line_to(cr, x, y);
    cairo_line_to(cr, x + width, y + height);
    cairo_move_to(cr, x + width, y);
    cairo_line_to(cr, x, y + height);
    cairo_stroke(cr);

    cairo_move_to(cr, 10, 20);
    cairo_set_font_size(cr, 15);
    cairo_show_text(cr, "Hello from Slide Lib!");

    printf(" draw: %d %d %d %d %s\n", x, y, width, height, sldname);
    printf("-draw\n");
}
