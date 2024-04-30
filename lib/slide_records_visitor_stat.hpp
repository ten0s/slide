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

#ifndef __SLIDE_RECORDS_VISITOR_STAT_HPP__
#define __SLIDE_RECORDS_VISITOR_STAT_HPP__

#include <cstdint>
#include "slide_records_visitor.hpp"

namespace libslide {

class SlideRecordsVisitorStat : public SlideRecordsVisitor {
public:
    SlideRecordsVisitorStat() {}

    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordSolidFillPolygon&) override;
    void accept(SlideRecordColor& r) override;
    void accept(SlideRecordEndOfFile&) override;

public:
    int16_t min_x() const;
    int16_t min_y() const;
    int16_t max_x() const;
    int16_t max_y() const;

private:
    int16_t _last_x = 0;
    int16_t _last_y = 0;
    int16_t _min_x = INT16_MAX;
    int16_t _min_y = INT16_MAX;
    int16_t _max_x = INT16_MIN;
    int16_t _max_y = INT16_MIN;
};

} // namespace libslide

#endif // __SLIDE_RECORDS_VISITOR_STAT_HPP__
