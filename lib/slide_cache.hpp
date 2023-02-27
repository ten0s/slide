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

#ifndef __SLIDE_CACHE_HPP__
#define __SLIDE_CACHE_HPP__

#include <string>
#include <unordered_map>

namespace libslide {

class Slide;

class SlideCache {
public:
    explicit SlideCache();
    ~SlideCache();

    SlideCache(const SlideCache&) = delete;
    SlideCache(SlideCache&&) = delete;
    SlideCache& operator=(const SlideCache&) = delete;
    SlideCache& operator=(SlideCache&&) = delete;

    const Slide* get(const std::string& slide_uri) const;
    void set(const std::string& slide_uri, Slide* slide);

private:
    std::unordered_map<std::string, Slide*> _cache;
};

} // namespace libslide

#endif // __SLIDE_CACHE_HPP__
