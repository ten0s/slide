#include <iostream>
#include "slide.hpp"
#include "slide_draw.h"
#include "slide_record_visitor_cairo.hpp"

int slide_draw(cairo_t *cr,
               unsigned x,
               unsigned y,
               unsigned width,
               unsigned height,
               const char *sldname)
{
    std::cout << "+slide_draw "
              << x << " " << y << " "
              << width << " " << height << " "
              << sldname << "\n";

    // Set line width
    cairo_set_line_width(cr, 1);

    try {
        // TODO: slide file or slide lib
        Slide slide = Slide::from_file(sldname);

        unsigned sld_width = slide.header().high_x_dot();
        unsigned sld_height = slide.header().high_y_dot();

        // Draw slide
        SlideRecordVisitorCairo visitor{
            cr,
            sld_width, sld_height,
            x, y,
            width, height
        };
        slide.visit_records(visitor);
    } catch (...) {
        // TODO:
        throw;

        return -1;
    }

    std::cout << "-slide_draw\n";
    return 0;
}
