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

#ifndef __SLIDE_HPP__
#define __SLIDE_HPP__

#include <memory>
#include <string>
#include <vector>
#include "slide_header.hpp"

namespace libslide {

// Both Slide format and Cairo agree on it.
// https://cairo.cairographics.narkive.com/H3TgPxCc/
// invalid-value-typically-too-big-for-the-size-of-the-input-surface-pattern-etc
static constexpr int MAX_SLIDE_SIZE = 32767;

class SlideRecord;
class SlideRecordsVisitor;

class Slide {
public:
    static std::shared_ptr<Slide> from_file(const std::string& filename);
    static std::shared_ptr<Slide> from_buf(const std::string& name, const uint8_t* buf, size_t size);

    Slide(const std::string& name,
          const SlideHeader& header,
          const std::vector<std::shared_ptr<SlideRecord>>& records,
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
    std::vector<std::shared_ptr<SlideRecord>> _records;
    size_t _size;
};

} // namespace libslide

#endif // __SLIDE_HPP__
