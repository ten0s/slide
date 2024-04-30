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

#include <cstring> // strncpy
#include "slide_library_directory.h"
#include "slide_library_directory.hpp"
#include "slide_library_directory_binary_parser.hpp"
#include "slide_binary_util.hpp"

namespace libslide {

std::tuple<std::shared_ptr<SlideLibraryDirectory>, size_t>
parse_slide_library_directory_binary(const uint8_t* buf, size_t /*size*/)
{
    constexpr auto offset = sizeof(Directory);
    constexpr auto length = sizeof(Directory::name);

    char name[length];
    strncpy(name, (char*)buf, length);

    // If name is null, then return dir is null and
    // increased offset should point to the first Slide.
    std::shared_ptr<SlideLibraryDirectory> dir;
    if (strlen(name) > 0) {
        // The slide address is always written with the low-order byte first.
        auto addr = read<uint32_t>(buf + length, Endian::little);
        dir = std::make_shared<SlideLibraryDirectory>(name, addr);
    }

    return {dir, offset};
}

} // namespace libslide
