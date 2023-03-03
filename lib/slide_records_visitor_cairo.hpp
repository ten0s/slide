/**
 *   AutoCAD slide library
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

#ifndef __SLIDE_RECORDS_VISITOR_CAIRO_HPP__
#define __SLIDE_RECORDS_VISITOR_CAIRO_HPP__

#include <cairo.h>
#include "slide_records_visitor.hpp"

namespace libslide {

class SlideRecordsVisitorCairo : public SlideRecordsVisitor {
public:
    explicit SlideRecordsVisitorCairo(cairo_t* cr,
                                      unsigned src_width,   // slide width
                                      unsigned src_height,  // slide height
                                      double   src_ratio,   // slide aspect ratio
                                      unsigned dst_x,       // draw offset x
                                      unsigned dst_y,       // draw offset y
                                      unsigned dst_width,   // draw width
                                      unsigned dst_height); // draw height

    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordSolidFillPolygon&) override;
    void accept(SlideRecordColor& r) override;
    void accept(SlideRecordEndOfFile&) override;

private:
    double adjust_x(unsigned x) const;
    double adjust_y(unsigned y) const;

private:
    cairo_t* _cr;
    unsigned _dst_x;
    unsigned _dst_y;
    unsigned _dst_height;
    unsigned _last_x;
    unsigned _last_y;
    double _scale_x;
    double _scale_y;
};

} // namespace libslide

#endif // __SLIDE_RECORDS_VISITOR_CAIRO_HPP__
