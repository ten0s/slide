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

#include <cstring>
#include "slide_endian.hpp"
#include "slide_library_directory.h"
#include "slide_library_directory.hpp"
#include "slide_library_directory_binary_writer.hpp"
#include "slide_binary_util.hpp"
#include "slide_util.hpp"

namespace libslide {

std::ostream&
write_slide_library_directory_binary(std::ostream& os, const SlideLibraryDirectory& dir)
{
    uint8_t name[sizeof(Directory::name)] = {0};
    strncpy((char*)name, to_upper(dir.name()).c_str(), sizeof(name));
    os.write((char*)name, sizeof(name));

    write(os, dir.addr(), Endian::little);

    return os;
}

std::ostream&
write_slide_library_directory_nil_binary(std::ostream& os)
{
    uint8_t nil[sizeof(Directory)] = {0};
    os.write((char*)nil, sizeof(nil));

    return os;
}

} // namespace libslide
