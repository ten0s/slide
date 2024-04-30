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

#include <filesystem>
#include <iostream>
#include <tuple>
#include <variant>
#include "slide.hpp"
#include "slide_library.hpp"
#include "slide_library_directory.hpp"
#include "slide_cache.hpp"
#include "slide_loader.hpp"
#include "slide_util.hpp"

namespace libslide {

static SlideCache cache;

using var_t = std::variant<std::tuple<std::string, std::string>, std::string>;

static std::string
normalize_slide_file(const std::string& file)
{
    namespace fs = std::filesystem;
    return fs::absolute(file).u8string();
}

static std::string
normalize_slide_name(const std::string& name)
{
    return to_upper(name);
}

static std::string
make_slide_lib_name_uri(const std::string& file, const std::string& name)
{
    return file + "(" + name + ")";
}

static std::string
normalize_slide_lib_name(const std::string& file, const std::string& name)
{
    return make_slide_lib_name_uri(
        normalize_slide_file(file),
        normalize_slide_name(name)
    );
}

static var_t
parse_slide_uri(const std::string& slide_uri)
{
    // Check if it's a slide library or slide
    auto i = slide_uri.find("(");
    auto j = slide_uri.rfind(")");

    if (i != std::string::npos && j == slide_uri.length()-1) {
        // It's a slide library
        return std::make_tuple(
            slide_uri.substr(0, i),
            slide_uri.substr(i+1, j-i-1)
        );
    } else if (i == std::string::npos && j == std::string::npos) {
        // It's a slide
        return slide_uri;
    } else {
        throw std::runtime_error("slide_uri: bad format");
    }
}

static std::string
join_slide_uri(const var_t& var)
{
   if (std::holds_alternative<std::string>(var)) {
       auto file = std::get<std::string>(var);
       return normalize_slide_file(file);
   } else {
       auto [file, name] = std::get<std::tuple<std::string, std::string>>(var);
       return normalize_slide_lib_name(file, name);
   }
}

static void
cache_slide_file(const std::string& file)
{
    // Read slide file.
    auto slide = Slide::from_file(file);

    // Store slide to cache.
    auto normal_uri = normalize_slide_file(file);
    cache.set(normal_uri, slide);
}

static void
cache_slide_lib(const std::string& file)
{
    // Read slide library.
    auto lib = SlideLibrary::from_file(file);

    // Store slides from library to cache.
    auto& dirs = lib->dirs();
    auto& slides = lib->slides();

    auto begin = dirs.begin();
    auto end = dirs.end();

    auto normal_file = normalize_slide_file(file);
    for (auto it = begin; it != end; ++it) {
        // Get slide.
        auto idx = std::distance(begin, it);
        auto slide = slides.at(idx);

        // Store named slide to cache.
        auto normal_name = normalize_slide_name((*it)->name());
        auto normal_uri = make_slide_lib_name_uri(normal_file, normal_name);
        cache.set(normal_uri, slide);

        // Store indexed slide to cache.
        auto index_name = std::to_string(idx);
        auto index_uri = make_slide_lib_name_uri(normal_file, index_name);
        cache.set(index_uri, slide);
    }
}

std::optional<std::shared_ptr<const Slide>>
slide_from_uri(const std::string& slide_uri)
{
    auto parsed_uri = parse_slide_uri(slide_uri);
    auto normal_uri = join_slide_uri(parsed_uri);

    // Look slides cache.
    auto slide = cache.get(normal_uri);
    if (slide) {
        return slide;
    }

    if (std::holds_alternative<std::string>(parsed_uri)) {
        auto file = std::get<std::string>(parsed_uri);
        cache_slide_file(file);
    } else {
        auto [file, _name] = std::get<std::tuple<std::string, std::string>>(parsed_uri);
        cache_slide_lib(file);
    }

    // Look slides cache again.
    return cache.get(normal_uri);
}

} // namespace libslide
