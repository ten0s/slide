/**
 *   AutoCAD slide library
 *
 *   Copyright (C) 2023-2025 Dmitry Klionsky aka ten0s <dm.klionsky@gmail.com>
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

#ifndef __SLIDE_LIBRARY_HEADER_HPP__
#define __SLIDE_LIBRARY_HEADER_HPP__

#include <string>

namespace libslide {

class SlideLibraryHeader {
public:
    static constexpr const char* ID_STRING = "AutoCAD Slide Library 1.0";

public:
    SlideLibraryHeader() {}

    std::string id_string() const { return ID_STRING; }
};

} // namespace libslide

#endif // __SLIDE_LIBRARY_HEADER_HPP__
