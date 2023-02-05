#include <iostream>
#include "slide.h"
#include "slide_file.hpp"
#include "slide_record_visitor_cairo.hpp"

int slide_draw(cairo_t *cr,
               unsigned x,
               unsigned y,
               unsigned width,
               unsigned height,
               const char *sldname)
{
    std::cout << "+draw "
              << x << " " << y << " "
              << width << " " << height << " "
              << sldname << "\n";

    // Draw black background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);

    // Set line width
    cairo_set_line_width(cr, 1);

    try {
        // TODO: slide file or slide lib
        SlideFile file = SlideFile::from_file(sldname);

        unsigned sld_width = file.header().high_x_dot();
        unsigned sld_height = file.header().high_y_dot();

        // Draw slide
        SlideRecordVisitorCairo visitor{
            cr,
            sld_width, sld_height,
            x, y,
            width, height
        };
        file.visit_records(visitor);
    } catch (...) {
        // TODO:
        throw;

        return -1;
    }

    std::cout << "-draw\n";
    return 0;
}
