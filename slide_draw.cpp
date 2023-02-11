#include <iostream>
#include <sstream>
#include "slide.hpp"
#include "slide_draw.h"
#include "slide_loader.hpp"
#include "slide_record_visitor_cairo.hpp"

int slide_draw(cairo_t *cr,
               unsigned x,
               unsigned y,
               unsigned width,
               unsigned height,
               const char *slide_uri)
{
    //std::cout << "+slide_draw "
    //          << x << " " << y << " "
    //          << width << " " << height << " "
    //          << slide_uri << "\n";

    try {

        // Load slide.
        const Slide* slide = slide_from_uri(slide_uri);

        if (!slide) {
            std::ostringstream ss;
            ss << "Slide " << slide_uri << " not found";
            throw std::runtime_error(ss.str());
        }

        unsigned sld_width = slide->header().high_x_dot();
        unsigned sld_height = slide->header().high_y_dot();
        double   sld_aspect_ratio = slide->header().aspect_ratio();

        // Draw slide.
        SlideRecordVisitorCairo visitor{
            cr,
            sld_width, sld_height,
            sld_aspect_ratio,
            x, y,
            width, height
        };
        slide->visit_records(visitor);

    } catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "Exception: " << e.what() << "\n";
        std::cerr << ss.str();
        return -1;
    }

    //std::cout << "-slide_draw\n";
    return 0;
}
