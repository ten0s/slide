#include <iostream>
#include "slide.h"
#include "slide_file.hpp"
#include "slide_record_visitor_cairo.hpp"

int slide_draw(cairo_t *cr, int x, int y, int width, int height, const char *sldname)
{
    std::cout << "+draw "
              << x << " " << y << " "
              << width << " " << height << " "
              << sldname << "\n";

    // Draw black background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    // Set line width
    cairo_set_line_width(cr, 1);

    try {
        // TODO: slide file or slide lib
        SlideFile file = SlideFile::from_file(sldname);

        unsigned sld_width = file.header().high_x_dot();
        unsigned sld_height = file.header().high_y_dot();
        double aspect = file.header().aspect_ratio();

        unsigned dst_width = width;
        unsigned dst_height = height;

        // TODO: pass x, y

        // Draw slide
        SlideRecordVisitorCairo visitor{
            cr,
                sld_width, sld_height,
                dst_width, dst_height,
                aspect
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
