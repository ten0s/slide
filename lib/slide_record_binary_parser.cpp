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

#include <cassert>
#include <iomanip> // std::setfill, std::setw, std::hex
#include <sstream> // std::ostringstream
#include "slide_records.hpp"
#include "slide_record_binary_parser.hpp"
#include "slide_binary_util.hpp"

namespace libslide {

static
std::vector<std::pair<int16_t, int16_t>>
parse_polygon_binary(const uint8_t* buf, Endian endian);

std::tuple<std::shared_ptr<SlideRecord>, size_t, bool>
parse_slide_record_binary(const uint8_t* buf, size_t /*size*/, Endian endian)
{
    std::shared_ptr<SlideRecord> record;
    size_t offset = 0;
    bool stop = false;

    auto head = read<int16_t>(buf, endian);
    auto hob = high_order_byte<int16_t>(head);
    auto lob = low_order_byte<int16_t>(head);

    if (hob <= 0x7f) {
        // Vector. Bytes: 8
        auto x0 = read<int16_t>(buf+0*sizeof(int16_t), endian);
        auto y0 = read<int16_t>(buf+1*sizeof(int16_t), endian);
        auto x1 = read<int16_t>(buf+2*sizeof(int16_t), endian);
        auto y1 = read<int16_t>(buf+3*sizeof(int16_t), endian);
        record = std::make_shared<SlideRecordVector>(x0, y0, x1, y1);
        offset = 8;
    } else if (hob == 0xfb) {
        // Offset vector. Bytes: 5
        auto dx0 = static_cast<int8_t>(lob);
        auto dy0 = read<int8_t>(buf+2, endian);
        auto dx1 = read<int8_t>(buf+3, endian);
        auto dy1 = read<int8_t>(buf+4, endian);
        record = std::make_shared<SlideRecordOffsetVector>(dx0, dy0, dx1, dy1);
        offset = 5;
    } else if (hob == 0xfc) {
        // End of file. Bytes: 2
        record = std::make_shared<SlideRecordEndOfFile>();
        offset = 2;
        stop = true;
    } else if (hob == 0xfd) {
        // Solid fill polygon Start record.
        // Bytes: 6 * (Vertices number + 2 /* start & end records*/)
        // The low-order byte is always zero.
        assert(lob == 0);
        // `num' sometimes includes the Stop record and
        // sometimes it doesn't :( We better ignore it
        // and parse until the Stop record.
        //auto num = read<int16_t>(buf+1*sizeof(int16_t), endian);
        // y is negative for start record
        auto y = read<int16_t>(buf+2*sizeof(int16_t), endian);
        assert(y < 0);
        // Parses until the Stop record.
        auto vertices = parse_polygon_binary(buf+3*sizeof(int16_t), endian);
        record = std::make_shared<SlideRecordSolidFillPolygon>(vertices);
        offset = 6 * (vertices.size() + 2);
    } else if (hob == 0xfe) {
        // Common endpoint vector. Bytes: 3
        auto dx0 = static_cast<int8_t>(lob);
        auto dy0 = read<int8_t>(buf+2, endian);
        record = std::make_shared<SlideRecordCommonEndpoint>(dx0, dy0);
        offset = 3;
    } else if (hob == 0xff) {
        // New color. Bytes: 2
        auto color = lob;
        record = std::make_shared<SlideRecordColor>(color);
        offset = 2;
    } else {
        std::ostringstream ss;
        ss << "Unknown record code: 0x"
           << std::setfill('0') << std::setw(2) << std::hex << int(hob);
        throw std::runtime_error{ss.str()};
    }

    return {record, offset, stop};
}

static
std::vector<std::pair<int16_t, int16_t>>
parse_polygon_binary(const uint8_t* buf, Endian endian)
{
    std::vector<std::pair<int16_t, int16_t>> vertices;
    size_t offset = 0;
    bool is_end = false;

    while (!is_end) {
        auto head = read<int16_t>(buf+offset, endian);
        auto hob = high_order_byte<int16_t>(head);
        auto lob = low_order_byte<int16_t>(head);

        if (hob == 0xfd) {
            // Solid fill polygon. Bytes: 6
            // The low-order byte is always zero.
            assert(lob == 0);
            auto x = read<int16_t>(buf+offset+1*sizeof(int16_t), endian);
            auto y = read<int16_t>(buf+offset+2*sizeof(int16_t), endian);
            if (y >= 0) {
                vertices.push_back({x, y});
            } else {
                // y is negative for end record
                is_end = true;
            }
            offset += 6;
        } else {
            std::ostringstream ss;
            ss << "Unknown record code: 0x"
               << std::setfill('0') << std::setw(2) << std::hex << int(hob);
            throw std::runtime_error{ss.str()};
        }
    }

    return vertices;
}

} // namespace libslide
