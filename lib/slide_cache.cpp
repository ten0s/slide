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

#include "slide_cache.hpp"
#include "slide.hpp"

namespace libslide {

SlideCache::SlideCache() {}

SlideCache::~SlideCache()
{
    _cache = {};
}

std::optional<std::shared_ptr<const Slide>>
SlideCache::get(const std::string& slide_uri) const
{
    auto it = _cache.find(slide_uri);
    if (it != _cache.end()) {
        return it->second;
    } else {
        return {};
    }
}

void
SlideCache::set(const std::string& slide_uri, std::shared_ptr<Slide> slide)
{
    _cache.insert({slide_uri, slide});
}

} // namespace libslide
