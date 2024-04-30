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

#include "slide_library_header.hpp"
#include "slide_library_header_binary_writer.hpp"

namespace libslide {

std::ostream&
write_slide_library_header_binary(std::ostream& os, const SlideLibraryHeader& header)
{
    os << header.id_string()
       << '\x0d' << '\x0a' << '\x1a'
       << '\x00' << '\x00' << '\x00'
       << '\x00';
    return os;
}

} // namespace libslide
