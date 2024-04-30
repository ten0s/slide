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

#include "slide.hpp"
#include "slide_binary_parser.hpp"
#include "slide_library_binary_parser.hpp"
#include "slide_library_header.hpp"
#include "slide_library_header_binary_parser.hpp"
#include "slide_library_directory.hpp"
#include "slide_library_directory_binary_parser.hpp"

namespace libslide {

std::tuple<SlideLibraryHeader,
           std::vector<std::shared_ptr<SlideLibraryDirectory>>,
           std::vector<std::shared_ptr<Slide>>,
           size_t>
parse_slide_library_binary(const uint8_t* buf, size_t size)
{
    auto [header, offset] = parse_slide_library_header_binary(buf, size);
    auto totaloffset = offset;

    std::vector<std::shared_ptr<SlideLibraryDirectory>> dirs;
    std::vector<std::shared_ptr<Slide>> slides;
    for (;;) {
        auto [dir, dirsize] = parse_slide_library_directory_binary(buf+offset, size-offset);
        offset += dirsize;
        totaloffset += dirsize;
        if (dir) {
            dirs.push_back(dir);

            auto name = dir->name();
            auto addr = dir->addr();
            auto [sldheader, records, sldsize] = parse_slide_binary(buf+addr, size-addr);
            totaloffset += sldsize;
            auto slide = std::make_shared<Slide>(name, sldheader, records, sldsize);
            slides.push_back(slide);
        } else {
            break;
        }
    }

    return {header, dirs, slides, totaloffset};
}

} // namespace libslide
