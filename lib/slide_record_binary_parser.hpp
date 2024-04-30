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

#ifndef __SLIDE_RECORD_BINARY_PARSER_HPP__
#define __SLIDE_RECORD_BINARY_PARSER_HPP__

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <memory>
#include <tuple>
#include "slide_endian.hpp"

namespace libslide {

class SlideRecord;

std::tuple<std::shared_ptr<SlideRecord>, size_t, bool>
parse_slide_record_binary(const uint8_t* buf, size_t size, Endian endian);

} // namespace libslide

#endif // __SLIDE_RECORD_BINARY_PARSER_HPP__
