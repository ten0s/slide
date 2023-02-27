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

#ifndef __SLIDE_HPP__
#define __SLIDE_HPP__

#include <string>
#include <vector>
#include "slide_header.hpp"

namespace libslide {

class SlideRecord;
class SlideRecordsVisitor;

class Slide {
public:
    static Slide from_file(const std::string& filename);
    static Slide from_buf(const std::string& name, const uint8_t* buf, size_t size);

    Slide(const std::string& name,
          const SlideHeader& header,
          const std::vector<SlideRecord*>& records,
          size_t size);
    Slide(Slide&&);
    ~Slide();

    Slide(const Slide&) = delete;
    Slide& operator=(const Slide&) = delete;
    Slide& operator=(Slide&&) = delete;

    const std::string& name() const { return _name; }
    const SlideHeader& header() const { return _header; }
    void visit_records(SlideRecordsVisitor& visitor) const;
    size_t size() const { return _size; }

private:
    std::string _name;
    SlideHeader _header;
    std::vector<SlideRecord*> _records;
    size_t _size;
};

} // namespace libslide

#endif // __SLIDE_HPP__
