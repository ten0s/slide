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

#ifndef __SLIDE_HEADER_HPP__
#define __SLIDE_HEADER_HPP__

#include <cstdint>
#include <string>
#include "slide_endian.hpp"

namespace libslide {

class SlideHeader {
public:
    static constexpr const char*    ID_STRING      = "AutoCAD Slide";
    static constexpr const uint8_t  TYPE_INDICATOR = 56;
    static constexpr const uint16_t HARDWARE_FILL  = 0;

public:
    SlideHeader(uint8_t type_indicator,
                uint8_t level_indicator,
                uint16_t high_x_dot,
                uint16_t high_y_dot,
                double aspect_ratio,
                uint16_t hardware_fill,
                Endian endian)
        : _type_indicator{type_indicator},
          _level_indicator{level_indicator},
          _high_x_dot{high_x_dot},
          _high_y_dot{high_y_dot},
          _aspect_ratio{aspect_ratio},
          _hardware_fill{hardware_fill},
          _endian{endian}
        {}

    SlideHeader(uint8_t level_indicator,
                uint16_t high_x_dot,
                uint16_t high_y_dot,
                double aspect_ratio,
                Endian endian)
        : SlideHeader(TYPE_INDICATOR,
                      level_indicator,
                      high_x_dot,
                      high_y_dot,
                      aspect_ratio,
                      HARDWARE_FILL,
                      endian)
        {}

    std::string id_string() const { return ID_STRING; }
    uint8_t type_indicator() const { return _type_indicator; }
    uint8_t level_indicator() const { return _level_indicator; }
    uint16_t high_x_dot() const { return _high_x_dot; }
    uint16_t high_y_dot() const { return _high_y_dot; }
    double aspect_ratio() const { return _aspect_ratio; }
    uint16_t hardware_fill() const { return _hardware_fill; }
    Endian endian() const { return _endian; }

private:
    uint8_t _type_indicator;
    uint8_t _level_indicator;
    uint16_t _high_x_dot;
    uint16_t _high_y_dot;
    double _aspect_ratio;
    uint16_t _hardware_fill;
    Endian _endian;
};

} // namespace libslide

#endif // __SLIDE_HEADER_HPP__
