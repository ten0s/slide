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

#include <cstring> // strncmp
#include <cstddef> // offsetof
#include <sstream>
#include "slide_header.hpp"
#include "slide_header_binary_parser.hpp"
#include "slide_binary_util.hpp"

namespace {

// The floating-point aspect ratio value and all 2-byte integers are
// written in the native format of the CPU that was used to create the file
// (for 8086-family CPUs, IEEE double-precision, and low-order byte first).
struct HeaderV1 {
    // Generic Part
    uint8_t id_string[17];    // AutoCAD Slide CR LF ^Z NUL
    uint8_t type_indicator;   // 56
    uint8_t level_indicator;  // 01
    uint8_t high_x_dot[2];    // Little | Big endian
    uint8_t high_y_dot[2];    // Little | Big endian
    // Specific
    uint8_t aspect_ratio[8];  // Double Little | Big endian
    uint8_t hardware_fill[2]; // 0x00 | 0x02 Unused
    uint8_t filler_byte;      // Unused
};

struct HeaderV2 {
    // Generic Part
    uint8_t id_string[17];    // AutoCAD Slide CR LF ^Z NUL
    uint8_t type_indicator;   // 56
    uint8_t level_indicator;  // 02
    uint8_t high_x_dot[2];    // Little | Big endian
    uint8_t high_y_dot[2];    // Little | Big endian
    // Specific Part
    uint8_t aspect_ratio[4];  // Little endian always
    uint8_t hardware_fill[2]; // 0x00 | 0x02 Unused
    uint8_t test_number[2];   // 0x1234 - Little | Big endian
};

} // namespace

namespace libslide {

std::tuple<SlideHeader, size_t>
parse_slide_header_binary(const uint8_t* buf, size_t size)
{
    Endian endian;
    uint16_t high_x_dot;
    uint16_t high_y_dot;
    double aspect_ratio;
    uint16_t hardware_fill;

    std::string id_str{SlideHeader::ID_STRING};
    size_t id_str_sz = id_str.size();
    if (strncmp((char*)buf, id_str.c_str(), id_str_sz) != 0 ||
        buf[id_str_sz+0] != 0x0d || buf[id_str_sz+1] != 0x0a ||
        buf[id_str_sz+2] != 0x1a || buf[id_str_sz+3] != 0x00) {
        throw std::runtime_error{"Invalid slide file header"};
    }

    uint8_t type_indicator = buf[offsetof(HeaderV1, type_indicator)];
    uint8_t level_indicator = buf[offsetof(HeaderV1, level_indicator)];

    switch (level_indicator) {
    case 1: { // Old version
        // Determine endianess by looking at the end of
        // the buffer inspecting the End of File marker.
        switch (read<uint16_t>(buf+size-2, Endian::little)) {
        case 0xfc00:
            endian = Endian::little;
            break;
        case 0x00fc:
            endian = Endian::big;
            break;
        default:
            throw std::runtime_error{"End of File is not found"};
        }
        high_x_dot = read<uint16_t>(buf+offsetof(HeaderV1, high_x_dot), endian);
        high_y_dot = read<uint16_t>(buf+offsetof(HeaderV1, high_y_dot), endian);

        aspect_ratio = read<double>(buf+offsetof(HeaderV1, aspect_ratio), endian);
        hardware_fill = read<uint16_t>(buf+offsetof(HeaderV1, hardware_fill), endian);
        break;
    }
    case 2: { // New version
        {
            // Determine endianess
            uint16_t tmp = read<uint16_t>(buf+offsetof(HeaderV2, test_number), Endian::little);
            endian = (tmp == 0x1234 ? Endian::little : Endian::big);
        }

        high_x_dot = read<uint16_t>(buf+offsetof(HeaderV2, high_x_dot), endian);
        high_y_dot = read<uint16_t>(buf+offsetof(HeaderV2, high_y_dot), endian);

        {
            // Always written with the least significant byte first.
            uint32_t tmp = read<uint32_t>(buf+offsetof(HeaderV2, aspect_ratio), Endian::little);
            aspect_ratio = tmp / 10'000'000.0;
        }

        hardware_fill = read<uint16_t>(buf+offsetof(HeaderV2, hardware_fill), endian);
        break;
    }
    default:
        std::ostringstream ss;
        ss << "Unknown slide file version: " << level_indicator;
        throw std::runtime_error{ss.str()};
    }

    SlideHeader header{
        type_indicator,
        level_indicator,
        high_x_dot,
        high_y_dot,
        aspect_ratio,
        hardware_fill,
        endian
    };

    size_t offset = level_indicator == 1 ? sizeof(HeaderV1) : sizeof(HeaderV2);

    return {header, offset};
}

} // namespace libslide
