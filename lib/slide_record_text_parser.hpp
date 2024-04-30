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

#ifndef __SLIDE_RECORD_TEXT_PARSER_HPP__
#define __SLIDE_RECORD_TEXT_PARSER_HPP__

#include <memory>

namespace libslide {

class SlideRecord;

std::shared_ptr<SlideRecord>
parse_slide_record_text(const std::string& str);

} // namespace libslide

#endif // __SLIDE_RECORD_TEXT_PARSER_HPP__
