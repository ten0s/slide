/**
 *   AutoCAD slide library
 *
 *   Copyright (C) 2023-2024 Dmitry Klionsky aka ten0s <dm.klionsky@gmail.com>
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

//#include "slide_colors.hpp"
#include "slide_records.hpp"
#include "slide_records_visitor_text_writer.hpp"

namespace libslide {

SlideRecordsVisitorTextWriter::SlideRecordsVisitorTextWriter(
    std::ostream& os, const std::string& pad)
    : _os{os}, _pad{pad} {}

void SlideRecordsVisitorTextWriter::accept(SlideRecordVector& r)
{
    _os << _pad;
    _os << "(VECTOR"
        << " " << (int)r.x0()
        << " " << (int)r.y0()
        << " " << (int)r.x1()
        << " " << (int)r.y1()
        << ")\n";
}

void SlideRecordsVisitorTextWriter::accept(SlideRecordOffsetVector& r)
{
    _os << _pad;
    _os << "(OFFSET_VECTOR"
        << " " << (int)r.dx0()
        << " " << (int)r.dy0()
        << " " << (int)r.dx1()
        << " " << (int)r.dy1()
        << ")\n";
}

void SlideRecordsVisitorTextWriter::accept(SlideRecordCommonEndpoint& r)
{
    _os << _pad;
    _os << "(COMMON_ENDPOINT"
        << " " << (int)r.dx0()
        << " " << (int)r.dy0()
        << ")\n";
}

void SlideRecordsVisitorTextWriter::accept(SlideRecordSolidFillPolygon& r)
{
    _os << _pad;
    _os << "(SOLID_FILL_POLYGON";
    for (auto [x, y] : r.vertices()) {
        _os << " " << x << " " << y;
    }
    _os << ")\n";
}

void SlideRecordsVisitorTextWriter::accept(SlideRecordColor& r)
{
    //RGB rgb = AutoCAD::colors[r.color()];
    _os << _pad;
    _os << "(COLOR " << (int)r.color() << ")"
        //<< " # ("
        //<< (int)rgb.red   << " "
        //<< (int)rgb.green << " "
        //<< (int)rgb.blue
        //<< ")"
        << "\n";
}

void SlideRecordsVisitorTextWriter::accept(SlideRecordEndOfFile&)
{
    _os << _pad;
    _os << "(END_OF_FILE)\n";
}

} // namespace libslide
