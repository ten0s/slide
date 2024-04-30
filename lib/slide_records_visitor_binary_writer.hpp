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

#ifndef __SLIDE_RECORDS_VISITOR_BINARY_WRITER_HPP__
#define __SLIDE_RECORDS_VISITOR_BINARY_WRITER_HPP__

#include <ostream>
#include <string>
#include "slide_endian.hpp"
#include "slide_records_visitor.hpp"

namespace libslide {

class SlideRecordsVisitorBinaryWriter : public SlideRecordsVisitor {
public:
    SlideRecordsVisitorBinaryWriter(std::ostream& os, Endian endian);
    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordSolidFillPolygon&) override;
    void accept(SlideRecordColor& r) override;
    void accept(SlideRecordEndOfFile& r) override;

private:
    std::ostream& _os;
    Endian _endian;
};

} // namespace libslide

#endif // __SLIDE_RECORDS_VISITOR_BINARY_WRITER_HPP__
