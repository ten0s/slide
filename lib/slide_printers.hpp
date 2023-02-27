/**
 *   AutoCAD slide library and command line utility
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

#ifndef __SLIDE_PRINTERS_HPP__
#define __SLIDE_PRINTERS_HPP__

#include <ostream>
#include <string>

namespace libslide {

enum class slide_info_t {
    NONE = 0,
    INFO,
    RECS,
    ALL
};

class Slide;

template <slide_info_t>
class SlidePrinter {
public:
    explicit SlidePrinter(const Slide& slide, const std::string& pad = "")
        : _slide{slide}, _pad{pad} {}

    const Slide& slide() const { return _slide; }
    const std::string&  pad() const { return _pad; }

private:
    const Slide& _slide;
    std::string _pad;
};

template <slide_info_t What>
std::ostream& operator<<(std::ostream& os, const SlidePrinter<What>& p);

} // namespace libslide

#endif // __SLIDE_PRINTERS_HPP__
