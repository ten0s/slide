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

#include "slide_records.hpp"
#include "slide_records_visitor_binary_writer.hpp"
#include "slide_binary_util.hpp"

namespace libslide {

SlideRecordsVisitorBinaryWriter::SlideRecordsVisitorBinaryWriter(std::ostream& os,
                                                                 Endian endian)
    : _os{os}, _endian{endian} {}

void SlideRecordsVisitorBinaryWriter::accept(SlideRecordVector& r)
{
    write(_os, r.x0(), _endian);
    write(_os, r.y0(), _endian);
    write(_os, r.x1(), _endian);
    write(_os, r.y1(), _endian);
}

void SlideRecordsVisitorBinaryWriter::accept(SlideRecordOffsetVector& r)
{
    write(_os, make<int16_t>({0xfb, static_cast<uint8_t>(r.dx0())}), _endian);
    _os << r.dy0();
    _os << r.dx1();
    _os << r.dy1();
}

void SlideRecordsVisitorBinaryWriter::accept(SlideRecordCommonEndpoint& r)
{
    write(_os, make<int16_t>({0xfe, static_cast<uint8_t>(r.dx0())}), _endian);
    _os << r.dy0();
}

void SlideRecordsVisitorBinaryWriter::accept(SlideRecordSolidFillPolygon& r)
{
    // Start record
    write(_os, make<int16_t>({0xfd, 0x00}), _endian);
    // x is number of records
    write(_os, static_cast<int16_t>(r.vertices().size()), _endian);
    // y is negative for start record
    write(_os, static_cast<int16_t>(-1), _endian);

    for (auto [x, y] : r.vertices()) {
        write(_os, make<int16_t>({0xfd, 0x00}), _endian);
        write(_os, static_cast<int16_t>(x), _endian);
        write(_os, static_cast<int16_t>(y), _endian);
    }

    // End Record
    write(_os, make<int16_t>({0xfd, 0x00}), _endian);
    // x is ignored
    write(_os, static_cast<int16_t>(0), _endian);
    // y is negative for end record
    write(_os, static_cast<int16_t>(-1), _endian);
}

void SlideRecordsVisitorBinaryWriter::accept(SlideRecordColor& r)
{
    write(_os, make<int16_t>({0xff, r.color()}), _endian);
}

void SlideRecordsVisitorBinaryWriter::accept(SlideRecordEndOfFile&)
{
    write(_os, make<int16_t>({0xfc, 0x00}), _endian);
}

} // namespace libslide
