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

#ifndef __SLIDE_LIBRARY_HPP__
#define __SLIDE_LIBRARY_HPP__

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "slide_library_header.hpp"

namespace libslide {

class Slide;
class SlideLibraryDirectory;

class SlideLibrary {
public:
    static std::shared_ptr<SlideLibrary> from_file(const std::string& filename);
    static std::shared_ptr<SlideLibrary> from_buf(const std::string& name, const uint8_t* buf, size_t size);

    SlideLibrary(const std::string& name,
                 const SlideLibraryHeader& header,
                 const std::vector<std::shared_ptr<SlideLibraryDirectory>>& dirs,
                 const std::vector<std::shared_ptr<Slide>>& slides);
    SlideLibrary(SlideLibrary&&);
    ~SlideLibrary();

    SlideLibrary(const SlideLibrary&) = delete;
    SlideLibrary& operator=(const SlideLibrary&) = delete;
    SlideLibrary& operator=(SlideLibrary&&) = delete;

    const std::string& name() const { return _name; }
    const SlideLibraryHeader& header() const { return _header; }
    const std::vector<std::shared_ptr<SlideLibraryDirectory>>& dirs() const { return _dirs; }
    const std::vector<std::shared_ptr<Slide>>& slides() const { return _slides; }
    size_t size() const { return _size; }

    std::optional<std::shared_ptr<const Slide>> find(const std::string& name) const;
    std::optional<std::shared_ptr<const Slide>> find(size_t idx) const;

    bool remove(const std::string& name);
    bool remove(size_t idx);

    void append(std::shared_ptr<Slide> slide);

private:
    void recalc_addrs_and_size();

private:
    std::string _name;
    SlideLibraryHeader _header;
    std::vector<std::shared_ptr<SlideLibraryDirectory>> _dirs;
    std::vector<std::shared_ptr<Slide>> _slides;
    size_t _size;
};

} // namespace libslide

#endif // __SLIDE_LIBRARY_HPP__
