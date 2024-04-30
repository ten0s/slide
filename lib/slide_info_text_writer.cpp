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
#include "slide_header.hpp"
#include "slide_info_text_writer.hpp"
#include "slide_records_visitor_text_writer.hpp"

namespace libslide {

std::ostream&
write_slide_info_text(std::ostream& os,
                      const Slide& slide,
                      slide_info_t info,
                      const std::string& pad)
{
    if (info == slide_info_t::INFO) {
        const auto& header = slide.header();
        const auto version = (int)header.level_indicator();

        os << pad << "Type   : " << header.id_string() << " " << version << ".0" << "\n";
        os << pad << "Name   : " << slide.name() << "\n";
        os << pad << "Size   : " << slide.size() << "\n";
        os << pad << "Width  : " << header.high_x_dot() << "\n";
        os << pad << "Height : " << header.high_y_dot() << "\n";
        os << pad << "Ratio  : " << header.aspect_ratio() << "\n";
        os << pad << "Endian : " << (header.endian() == Endian::little ?
                                     "Little" : "Big") << "\n";
    }

    if (info == slide_info_t::RECS) {
        SlideRecordsVisitorTextWriter visitor{os, pad};
        slide.visit_records(visitor);
    }

    return os;
}

} // namespace libslide
