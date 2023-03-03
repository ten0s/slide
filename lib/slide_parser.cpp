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

#include <tuple>
#include "slide_parser.hpp"
#include "slide_header.hpp"
#include "slide_header_parser.hpp"
#include "slide_records.hpp"
#include "slide_record_parser.hpp"

namespace libslide {

std::tuple<SlideHeader, std::vector<SlideRecord*>, size_t>
parse_slide(const uint8_t* buf, size_t size)
{
    auto [header, offset] = parse_slide_header(buf, size);
    Endian endian = header.endian();

    std::vector<SlideRecord*> records;
    for (;;) {
        auto [record, delta, stop] = parse_slide_record(buf+offset, size-offset, endian);
        records.push_back(record);
        offset += delta;
        if (stop) {
            break;
        }
    }

    return {header, records, offset};
}

} // namespace libslide
