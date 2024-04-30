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

#ifndef __SLIDE_UTIL_HPP__
#define __SLIDE_UTIL_HPP__

#include <string>

namespace libslide {

std::string basename(const std::string& filename);
std::string get_ext(const std::string& filename);
std::string strip_ext(const std::string& filename);
std::string to_lower(const std::string& str);
std::string to_upper(const std::string& str);
void make_backup(const std::string& filename);

template<typename Cont>
Cont tail(const Cont& c)
{
    return Cont{++c.begin(), c.end()};
}

} // namespace libslide

#endif // __SLIDE_UTIL_HPP__
