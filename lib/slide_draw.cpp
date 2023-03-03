/**
 *   AutoCAD slide library and command line utility
 *
 *   Copyright (C) 2023 Dmitry Klionsky aka ten0s <dm.klionsky@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <iostream>
#include <sstream>
#include "slide.hpp"
#include "slide_draw.h"
#include "slide_loader.hpp"
#include "slide_records_visitor_cairo.hpp"

using namespace libslide;

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
        auto maybeSlide = slide_from_uri(slide_uri);

        if (!maybeSlide) {
            std::ostringstream ss;
            ss << "Slide " << slide_uri << " not found";
            throw std::runtime_error(ss.str());
        }

        auto slide = maybeSlide.value();
        unsigned sld_width  = slide->header().high_x_dot();
        unsigned sld_height = slide->header().high_y_dot();
        double   sld_ratio  = slide->header().aspect_ratio();

        // Draw slide.
        SlideRecordsVisitorCairo visitor{
            cr,
            sld_width, sld_height,
            sld_ratio,
            x, y,
            width, height
        };
        slide->visit_records(visitor);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return -1;
    }

    //std::cout << "-slide_draw\n";
    return 0;
}
