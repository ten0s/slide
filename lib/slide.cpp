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

#include <cstring>   // basename
#include <memory>    // std::unique_ptr
#include <fstream>
#include <sstream>
#include "slide.hpp"
#include "slide_binary_parser.hpp"
#include "slide_records.hpp"
#include "slide_util.hpp"

namespace libslide {

std::shared_ptr<Slide>
Slide::from_file(const std::string& filename)
{
    std::ifstream is{filename, std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        // Determine file size.
        size_t size = is.tellg();
        std::unique_ptr<uint8_t[]> buf{new uint8_t[size]};
        is.seekg(0);
        // Read the whole file.
        if (is.read((char*)buf.get(), size)) {
            auto name = strip_ext(basename(filename));
            return from_buf(name, buf.get(), size);
        } else {
            std::stringstream ss;
            ss << "Slide file read failed: " << filename;
            throw std::runtime_error{ss.str()};
        }
    } else {
        std::stringstream ss;
        ss << "Slide file open failed: " << filename;
        throw std::runtime_error{ss.str()};
    }
}

std::shared_ptr<Slide>
Slide::from_buf(const std::string& name,
                const uint8_t* buf, size_t size)
{
    auto [header, records, offset] = parse_slide_binary(buf, size);
    return std::make_shared<Slide>(name, header, records, offset);
}

Slide::Slide(const std::string& name,
             const SlideHeader& header,
             const std::vector<std::shared_ptr<SlideRecord>>& records,
             size_t size)
    : _name{name},
      _header{header},
      _records{records},
      _size{size}
    {}

Slide::Slide(Slide&& old)
    : _name{old._name},
      _header{old._header},
      _records{old._records},
      _size{old._size}
{
    old._records = {};
}

Slide::~Slide()
{
    _records = {};
}

void
Slide::visit_records(SlideRecordsVisitor& visitor) const
{
    for (auto& record : _records) {
        record->visit(visitor);
    }
}

} // namespace libslide
