/**
 *   AutoCAD slide library
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

#include <algorithm> // std::find_if
#include <memory>    // std::unique_ptr
#include <fstream>
#include <sstream>
#include "slide.hpp"
#include "slide_binary_parser.hpp"
#include "slide_library.hpp"
#include "slide_library_binary_parser.hpp"
#include "slide_library_directory.hpp"
#include "slide_util.hpp"

namespace libslide {

SlideLibrary
SlideLibrary::from_file(const std::string& filename)
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
            ss << "Slide library read failed: " << filename;
            throw std::runtime_error{ss.str()};
        }
    } else {
        std::stringstream ss;
        ss << "Slide library open failed: " << filename;
        throw std::runtime_error{ss.str()};
    }
}

SlideLibrary
SlideLibrary::from_buf(const std::string& name,
                       const uint8_t* buf, size_t size)
{
    auto [header, dirs, slides, offset] = parse_slide_library_binary(buf, size);

    return SlideLibrary{
        name,
        header,
        dirs,
        slides,
        offset,
    };
}

SlideLibrary::SlideLibrary(const std::string& name,
                           const SlideLibraryHeader& header,
                           const std::vector<std::shared_ptr<SlideLibraryDirectory>>& dirs,
                           const std::vector<std::shared_ptr<Slide>>& slides,
                           size_t size)
        : _name{name},
          _header{header},
          _dirs{dirs},
          _slides{slides},
          _size{size}
       {}

SlideLibrary::SlideLibrary(SlideLibrary&& old)
    : _name{old._name},
      _header{old._header},
      _dirs{old._dirs},
      _slides{old._slides},
      _size{old._size}
{
    old._dirs = {};
    old._slides = {};
}

SlideLibrary::~SlideLibrary()
{
    _dirs = {};
    _slides = {};
}

std::optional<std::shared_ptr<const Slide>>
SlideLibrary::find(const std::string& name) const
{
    auto upper = to_upper(name);
    auto pos = std::find_if(
        _dirs.cbegin(), _dirs.cend(),
        [&](const auto& dir) { return dir->name() == upper; }
    );

    if (pos != _dirs.cend()) {
        auto idx = std::distance(_dirs.cbegin(), pos);
        return find(idx);
    } else {
        return {};
    }
}

std::optional<std::shared_ptr<const Slide>>
SlideLibrary::find(size_t idx) const
{
    if (idx >= 0 && idx < _slides.size()) {
        return _slides.at(idx);
    }
    return {};
}

void
SlideLibrary::append(Slide&& slide)
{
    //_dirs.push_back(
    //_slides.push_back(slide);
    //_size += slide.size() + ;
}

} // namespace libslide
