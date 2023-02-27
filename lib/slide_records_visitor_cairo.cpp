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

#include "slide_colors.hpp"
#include "slide_records.hpp"
#include "slide_records_visitor_cairo.hpp"

namespace libslide {

static const unsigned PADDING_X = 1;
static const unsigned PADDING_Y = 1;

SlideRecordsVisitorCairo::SlideRecordsVisitorCairo(cairo_t* cr,
                                                   unsigned src_width,
                                                   unsigned src_height,
                                                   double   src_ratio,
                                                   unsigned dst_x,
                                                   unsigned dst_y,
                                                   unsigned dst_width,
                                                   unsigned dst_height)
    : _cr{cr},
      _dst_x{dst_x},
      _dst_y{dst_y},
      _dst_height{dst_height},
      _last_x{0},
      _last_y{0}
{
    //
    // Calculate new_width and new_height taking
    // into account source aspect ratio.
    //
    // https://math.stackexchange.com/questions/1620366/
    // how-to-keep-aspect-ratio-and-position-of-rectangle-inside-another-rectangle
    //
    double dst_ratio = 1.0 * dst_width / dst_height;

    unsigned new_width = dst_width;
    unsigned new_height = dst_height;

    if (src_ratio > dst_ratio) {
        new_height = dst_width / src_ratio;
    } else if (src_ratio < dst_ratio) {
        new_width = dst_height * src_ratio;
    }

    _scale_x = 1.0 * new_width  / src_width;
    _scale_y = 1.0 * new_height / src_height;

    // Set line width
    cairo_set_line_width(_cr, 1);
}

inline
double SlideRecordsVisitorCairo::adjust_x(unsigned x) const {
    // Scale & Move
    return x * _scale_x + _dst_x + PADDING_X;
}

inline
double SlideRecordsVisitorCairo::adjust_y(unsigned y) const {
    // Point (0,0) is located at the lower-left corner.
    // Scale & Move
    return _dst_height - y * _scale_y + _dst_y - PADDING_Y;
}

void SlideRecordsVisitorCairo::accept(SlideRecordVector& r)
{
    int x0 = r.x0(), y0 = r.y0();
    int x1 = r.x1(), y1 = r.y1();

    cairo_move_to(_cr, adjust_x(x0), adjust_y(y0));
    cairo_line_to(_cr, adjust_x(x1), adjust_y(y1));
    cairo_stroke(_cr);

    // The from point is saved as the last point.
    _last_x = x0;
    _last_y = y0;
}

void SlideRecordsVisitorCairo::accept(SlideRecordOffsetVector& r)
{
    int x0 = _last_x + r.dx0(), y0 = _last_y + r.dy0();
    int x1 = _last_x + r.dx1(), y1 = _last_y + r.dy1();

    cairo_move_to(_cr, adjust_x(x0), adjust_y(y0));
    cairo_line_to(_cr, adjust_x(x1), adjust_y(y1));
    cairo_stroke(_cr);

    // The adjusted from point is saved as the last point.
    _last_x = x0;
    _last_y = y0;
}

void SlideRecordsVisitorCairo::accept(SlideRecordCommonEndpoint& r)
{
    // This is a vector starting at the last point.
    int x0 = _last_x,           y0 = _last_y;
    int x1 = _last_x + r.dx0(), y1 = _last_y + r.dy0();

    cairo_move_to(_cr, adjust_x(x0), adjust_y(y0));
    cairo_line_to(_cr, adjust_x(x1), adjust_y(y1));
    cairo_stroke(_cr);

    // The adjusted to point is saved as the last point.
    _last_x = x1;
    _last_y = y1;
}

void SlideRecordsVisitorCairo::accept(SlideRecordSolidFillPolygon& r)
{
    auto& vs = r.vertices();
    if (vs.size() > 0) {
        auto it = vs.cbegin();
        auto end = vs.cend();

        auto [x0, y0] = *it;
        cairo_move_to(_cr, adjust_x(x0), adjust_y(y0));
        ++it;

        for (; it != end; ++it) {
            auto [x, y] = *it;
            cairo_line_to(_cr, adjust_x(x), adjust_y(y));
        }

        cairo_close_path(_cr);
        cairo_fill(_cr);
    }
}

void SlideRecordsVisitorCairo::accept(SlideRecordColor& r)
{
    RGB rgb = AutoCAD::colors[r.color()];
    cairo_set_source_rgb(_cr,
                         rgb.red   / 255.0,
                         rgb.green / 255.0,
                         rgb.blue  / 255.0);
}

void SlideRecordsVisitorCairo::accept(SlideRecordEndOfFile&)
{

}

} // namespace libslide
