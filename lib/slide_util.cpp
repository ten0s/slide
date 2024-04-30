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

#include <algorithm> // std::transform
#include <filesystem>
#include "slide_util.hpp"

namespace fs = std::filesystem;

namespace libslide {

std::string
basename(const std::string& filename)
{
    return fs::path(filename).filename().u8string();
}

std::string
get_ext(const std::string& filename)
{
    return fs::path(filename).extension().u8string();
}

std::string
strip_ext(const std::string& filename)
{
    return fs::path(filename).stem().u8string();
}

std::string
to_lower(const std::string& in)
{
    std::string out{in};
    std::transform(
        out.begin(), out.end(),
        out.begin(),
        [](auto c) { return std::tolower(c); }
    );
    return out;
}

std::string
to_upper(const std::string& in)
{
    std::string out{in};
    std::transform(
        out.begin(), out.end(),
        out.begin(),
        [](auto c) { return std::toupper(c); }
    );
    return out;
}

void
make_backup(const std::string& filename)
{
    if (fs::exists(filename)) {
        auto bakname = filename + ".bak";
        auto i = 1;
        while (fs::exists(bakname)) {
            bakname = filename + ".bak." + std::to_string(i);
            ++i;
        }
        fs::copy_file(filename, bakname);
    }
}

} // namespace libslide
