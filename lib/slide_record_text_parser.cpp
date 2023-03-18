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

#include <regex>
#include "slide.hpp"
#include "slide_colors.hpp"
#include "slide_records.hpp"
#include "slide_record_text_parser.hpp"

using std::regex_constants::icase;

namespace libslide {

static uint8_t
check_color(int color, const std::string& str)
{
    if (color < 0 || color > MAX_COLOR) {
        throw std::runtime_error{
            "Invalid color: " + std::to_string(color) +
                " in slide record: " + str};
    }
    return color;
}

static int16_t
check_size(int size, const std::string& str)
{
    if (size > MAX_SLIDE_SIZE) {
        throw std::runtime_error{
            "Invalid size: " + std::to_string(size) +
                " in slide record: " + str};
    }
    return size;
}

static int8_t
check_delta(int delta, const std::string& str)
{
    if (delta < -128 || delta > 127) {
        throw std::runtime_error{
            "Invalid delta: " + std::to_string(delta) +
                " in slide record: " + str};
    }
    return delta;
}

static std::shared_ptr<SlideRecord>
ignore(const std::smatch&, const std::string&) {
    return {};
}

static std::shared_ptr<SlideRecord>
parse_vector(const std::smatch& matches, const std::string& str) {
    int16_t x0 = check_size(std::stoi(matches[1]), str);
    int16_t y0 = check_size(std::stoi(matches[2]), str);
    int16_t x1 = check_size(std::stoi(matches[3]), str);
    int16_t y1 = check_size(std::stoi(matches[4]), str);
    return std::make_shared<SlideRecordVector>(x0, y0, x1, y1);
}

static std::shared_ptr<SlideRecord>
parse_offset_vector(const std::smatch& matches, const std::string& str) {
    int8_t dx0 = check_delta(std::stoi(matches[1]), str);
    int8_t dy0 = check_delta(std::stoi(matches[2]), str);
    int8_t dx1 = check_delta(std::stoi(matches[3]), str);
    int8_t dy1 = check_delta(std::stoi(matches[4]), str);
    return std::make_shared<SlideRecordOffsetVector>(dx0, dy0, dx1, dy1);
}

static std::shared_ptr<SlideRecord>
parse_common_endpoint(const std::smatch& matches, const std::string& str) {
    int8_t dx0 = check_delta(std::stoi(matches[1]), str);
    int8_t dy0 = check_delta(std::stoi(matches[2]), str);
    return std::make_shared<SlideRecordCommonEndpoint>(dx0, dy0);
}

static std::shared_ptr<SlideRecord>
parse_solid_fill_polygon(const std::smatch& matches, const std::string& str) {
    SlideRecordSolidFillPolygon::vertices_t vertices;
    for (size_t i = 1; i < matches.size(); i += 2) {
        int16_t x = check_size(std::stoi(matches[i]), str);
        int16_t y = check_size(std::stoi(matches[i+1]), str);
        vertices.push_back({x, y});
    }
    return std::make_shared<SlideRecordSolidFillPolygon>(vertices);
}

static std::shared_ptr<SlideRecord>
parse_color(const std::smatch& matches, const std::string& str) {
    uint8_t color = check_color(std::stoi(matches[1]), str);
    return std::make_shared<SlideRecordColor>(color);
}

static std::shared_ptr<SlideRecord>
parse_end_of_file(const std::smatch&, const std::string&) {
    return std::make_shared<SlideRecordEndOfFile>();
}

std::shared_ptr<SlideRecord>
parse_slide_record_text(const std::string& str)
{
    static std::regex comment {"^[;#].*"};
    static std::regex empty   {"^\\s*"};

    static std::regex vector {
        "\\s*\\(\\s*VECTOR"
            "\\s+(\\d+)\\s+(\\d+)"
            "\\s+(\\d+)\\s+(\\d+)"
        "\\s*\\).*", icase};

    static std::regex offset_vector {
        "\\s*\\(\\s*OFFSET_VECTOR"
            "\\s+(-?\\d+)\\s+(-?\\d+)"
            "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex common_endpoint {
        "\\s*\\(\\s*COMMON_ENDPOINT"
            "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex polygon_1 {
        "\\s*\\(\\s*SOLID_FILL_POLYGON"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex polygon_2 {
        "\\s*\\(\\s*SOLID_FILL_POLYGON"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex polygon_3 {
        "\\s*\\(\\s*SOLID_FILL_POLYGON"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex polygon_4 {
        "\\s*\\(\\s*SOLID_FILL_POLYGON"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex polygon_5 {
        "\\s*\\(\\s*SOLID_FILL_POLYGON"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex polygon_6 {
        "\\s*\\(\\s*SOLID_FILL_POLYGON"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex polygon_7 {
        "\\s*\\(\\s*SOLID_FILL_POLYGON"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex polygon_8 {
        "\\s*\\(\\s*SOLID_FILL_POLYGON"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
             "\\s+(-?\\d+)\\s+(-?\\d+)"
        "\\s*\\).*", icase};

    static std::regex color {
        "\\s*\\(\\s*COLOR"
            "\\s+(\\d+)"
        "\\s*\\).*", icase};

    static std::regex end_of_file {"\\s*\\(\\s*END_OF_FILE\\s*\\).*", icase};

    using parsers_t =
        std::vector<
            std::pair<
                std::regex,
                std::function<std::shared_ptr<SlideRecord>(const std::smatch&,
                                                           const std::string&)>>>;

    static parsers_t parsers {
        {comment, ignore},
        {empty, ignore},
        {color, parse_color},
        {vector, parse_vector},
        {offset_vector, parse_offset_vector},
        {common_endpoint, parse_common_endpoint},
        {end_of_file, parse_end_of_file},
        {polygon_1, parse_solid_fill_polygon},
        {polygon_2, parse_solid_fill_polygon},
        {polygon_3, parse_solid_fill_polygon},
        {polygon_4, parse_solid_fill_polygon},
        {polygon_5, parse_solid_fill_polygon},
        {polygon_6, parse_solid_fill_polygon},
        {polygon_7, parse_solid_fill_polygon},
        {polygon_8, parse_solid_fill_polygon},
    };

    for (auto& [pattern, parser] : parsers) {
        if (std::smatch matches; std::regex_match(str, matches, pattern)) {
            return parser(matches, str);
        }
    }

    throw std::runtime_error{"Invalid slide record: " + str};
}

} // namespace libslide
