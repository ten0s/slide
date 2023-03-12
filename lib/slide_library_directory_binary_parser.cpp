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

#include <cstddef> // offsetof
#include <cstring> // strncpy
#include "slide_library_directory.hpp"
#include "slide_library_directory_binary_parser.hpp"
#include "slide_binary_util.hpp"

namespace {

struct Directory {
    // Slide name (NUL terminated) (32 bytes)
    uint8_t name[32];
    // Address of slide within library file (4 bytes)
    uint32_t addr;
};

} // namespace

namespace libslide {

std::tuple<SlideLibraryDirectory*, size_t>
parse_slide_library_directory_binary(const uint8_t* buf, size_t /*size*/)
{
    constexpr size_t offset = sizeof(Directory);
    constexpr size_t length = offsetof(Directory, addr);

    char name[length];
    strncpy(name, (char*)buf, length);

    // If name is null, then return dir is null and
    // increased offset should point to the first Slide.
    SlideLibraryDirectory* dir = nullptr;
    if (strlen(name) > 0) {
        // The slide address is always written with the low-order byte first.
        uint32_t addr = read<uint32_t>(buf + length, Endian::LE);
        dir = new SlideLibraryDirectory{
            name,
            addr
        };
    }

    return {dir, offset};
}

} // namespace libslide
