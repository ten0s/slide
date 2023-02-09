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

    try {
        // TODO: slide file or slide lib
        Slide slide = Slide::from_file(sldname);

        unsigned sld_width = slide.header().high_x_dot();
        unsigned sld_height = slide.header().high_y_dot();
        double   sld_aspect_ratio = slide.header().aspect_ratio();

        // Draw slide
        SlideRecordVisitorCairo visitor{
            cr,
            sld_width, sld_height,
            sld_aspect_ratio,
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
