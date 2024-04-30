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

#include <algorithm> // std::max
#include "slide_library.hpp"
#include "slide_library_directory.hpp"
#include "slide_library_info_text_writer.hpp"

namespace libslide {

std::ostream&
write_slide_library_info_text(std::ostream& os,
                              const SlideLibrary& lib,
                              slide_library_info_t info,
                              const std::string& pad)
{
    if (info == slide_library_info_t::INFO) {
        const auto& header = lib.header();
        os << pad << "Type   : " << header.id_string() << "\n";
        os << pad << "Name   : " << lib.name() << "\n";
        os << pad << "Size   : " << lib.size() << "\n";
        os << pad << "Slides : " << lib.dirs().size() << "\n";
    }

    if (info == slide_library_info_t::NAMES) {
        for (const auto& dir : lib.dirs()) {
            os << pad << dir->name() << "\n";
        };
    }

    if (info == slide_library_info_t::DIRS) {
        size_t max = 0;
        for (const auto& dir : lib.dirs()) {
            max = std::max(max, dir->name().size());
        };

        for (const auto& dir : lib.dirs()) {
            size_t size = dir->name().size();
            os << pad
               << dir->name()
               << std::string(max - size + 1, ' ')
               << ": "
               << dir->addr()
               << "\n";
        };
    }

    return os;
}

} // namespace libslide
