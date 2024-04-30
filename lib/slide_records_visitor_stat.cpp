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

#include <algorithm>
#include "slide_records.hpp"
#include "slide_records_visitor_stat.hpp"

namespace libslide {

void SlideRecordsVisitorStat::accept(SlideRecordVector& r)
{
    int16_t x0 = r.x0(), y0 = r.y0();
    int16_t x1 = r.x1(), y1 = r.y1();

    // The adjusted from point is saved as the last point.
    _last_x = x0;
    _last_y = y0;

    _min_x = std::min({_min_x, x0, x1});
    _min_y = std::min({_min_y, y0, y1});
    _max_x = std::max({_max_x, x0, x1});
    _max_y = std::max({_max_y, y0, y1});
}

void SlideRecordsVisitorStat::accept(SlideRecordOffsetVector& r)
{
    int16_t x0 = _last_x + r.dx0(), y0 = _last_y + r.dy0();
    int16_t x1 = _last_x + r.dx1(), y1 = _last_y + r.dy1();

    // The adjusted from point is saved as the last point.
    _last_x = x0;
    _last_y = y0;

    _min_x = std::min({_min_x, x0, x1});
    _min_y = std::min({_min_y, y0, y1});
    _max_x = std::max({_max_x, x0, x1});
    _max_y = std::max({_max_y, y0, y1});
}

void SlideRecordsVisitorStat::accept(SlideRecordCommonEndpoint& r)
{
    // This is a vector starting at the last point.
    int16_t x0 = _last_x,           y0 = _last_y;
    int16_t x1 = _last_x + r.dx0(), y1 = _last_y + r.dy0();

    // The adjusted to point is saved as the last point.
    _last_x = x1;
    _last_y = y1;

    _min_x = std::min({_min_x, x0, x1});
    _min_y = std::min({_min_y, y0, y1});
    _max_x = std::max({_max_x, x0, x1});
    _max_y = std::max({_max_y, y0, y1});
}

void SlideRecordsVisitorStat::accept(SlideRecordSolidFillPolygon& r)
{
    auto& vs = r.vertices();
    if (vs.size() > 0) {
        auto it = vs.cbegin();
        auto end = vs.cend();

        for (; it != end; ++it) {
            auto [x, y] = *it;
            _min_x = std::min({_min_x, x});
            _min_y = std::min({_min_y, y});
            _max_x = std::max({_max_x, x});
            _max_y = std::max({_max_y, y});
        }
    }
}

void SlideRecordsVisitorStat::accept(SlideRecordColor& r)
{

}

void SlideRecordsVisitorStat::accept(SlideRecordEndOfFile&)
{

}

int16_t SlideRecordsVisitorStat::min_x() const
{
    return std::min(static_cast<decltype(_min_x)>(0), _min_x);
}

int16_t SlideRecordsVisitorStat::min_y() const
{
    return std::min(static_cast<decltype(_min_y)>(0), _min_y);
}

int16_t SlideRecordsVisitorStat::max_x() const
{
    return std::max(static_cast<decltype(_max_x)>(0), _max_x);
}

int16_t SlideRecordsVisitorStat::max_y() const
{
    return std::max(static_cast<decltype(_max_y)>(0), _max_y);
}

} // namespace libslide
