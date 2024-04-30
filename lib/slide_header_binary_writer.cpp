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

#include "slide_header.hpp"
#include "slide_header_binary_writer.hpp"
#include "slide_binary_util.hpp"

namespace libslide {

std::ostream&
write_slide_header_binary(std::ostream& os, const SlideHeader& header, Endian endian)
{
    // Generic Part
    os << header.id_string();
    os << '\x0d' << '\x0a';
    os << '\x1a' << '\x00';

    os << header.type_indicator();
    os << header.level_indicator();

    write(os, header.high_x_dot(), endian);
    write(os, header.high_y_dot(), endian);

    if (header.level_indicator() == 1) {
        // V1 Specific
        write(os, header.aspect_ratio(), endian);
        write(os, header.hardware_fill(), endian);
        os << '\x00'; // Filler byte
    } else if (header.level_indicator() == 2) {
        // V2 Specific
        write(os, static_cast<uint32_t>(header.aspect_ratio() * 10'000'000), Endian::little);
        write(os, header.hardware_fill(), endian);
        write(os, make<uint16_t>({0x12, 0x34}), endian); // Test number
    } else {
        throw std::runtime_error{"Unknown slide version"};
    }

    return os;
}

} // namespace libslide
