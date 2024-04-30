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
#include <stdexcept>
#include "slide_library_header.hpp"
#include "slide_library_header_binary_parser.hpp"

namespace {

struct Header {
    // "AutoCAD Slide Library 1.0" CR LF ^Z NUL NUL NUL NUL  Header (32 bytes)
    uint8_t id_str[32];
};

} // namespace

namespace libslide {

std::tuple<SlideLibraryHeader, size_t>
parse_slide_library_header_binary(const uint8_t* buf, size_t size)
{
    std::string id_str{SlideLibraryHeader::ID_STRING};
    size_t id_str_sz = id_str.size();
    if (strncmp((char*)buf, id_str.c_str(), id_str_sz) != 0 ||
        buf[id_str_sz+0] != 0x0d || buf[id_str_sz+1] != 0x0a ||
        buf[id_str_sz+2] != 0x1a || buf[id_str_sz+3] != 0x00 ||
        buf[id_str_sz+4] != 0x00 || buf[id_str_sz+5] != 0x00 ||
        buf[id_str_sz+6] != 0x00)
    {
        throw std::runtime_error{"Invalid slide library header"};
    }

    SlideLibraryHeader header{};

    size_t offset = sizeof(Header);

    return {header, offset};
}

} // namespace libslide
