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
#include "slide_records.hpp"
#include "slide_record_text_parser.hpp"

using std::regex_constants::icase;

namespace libslide {

static std::shared_ptr<SlideRecord>
parse_comment(std::smatch&) {
    return {};
}

static std::shared_ptr<SlideRecord>
parse_vector(std::smatch& matches) {
    int16_t x0 = std::stoi(matches[1]);
    int16_t y0 = std::stoi(matches[2]);
    int16_t x1 = std::stoi(matches[3]);
    int16_t y1 = std::stoi(matches[4]);
    return std::make_shared<SlideRecordVector>(x0, y0, x1, y1);
}

static std::shared_ptr<SlideRecord>
parse_offset_vector(std::smatch& matches) {
    int8_t dx0 = std::stoi(matches[1]);
    int8_t dy0 = std::stoi(matches[2]);
    int8_t dx1 = std::stoi(matches[3]);
    int8_t dy1 = std::stoi(matches[4]);
    return std::make_shared<SlideRecordOffsetVector>(dx0, dy0, dx1, dy1);
}

static std::shared_ptr<SlideRecord>
parse_common_endpoint(std::smatch& matches) {
    int8_t dx0 = std::stoi(matches[1]);
    int8_t dy0 = std::stoi(matches[2]);
    return std::make_shared<SlideRecordCommonEndpoint>(dx0, dy0);
}

static std::shared_ptr<SlideRecord>
parse_solid_fill_polygon(std::smatch& matches) {
    SlideRecordSolidFillPolygon::vertices_t vertices;
    for (size_t i = 1; i < matches.size(); i += 2) {
        int16_t x = std::stoi(matches[i]);
        int16_t y = std::stoi(matches[i+1]);
        vertices.push_back({x, y});
    }
    return std::make_shared<SlideRecordSolidFillPolygon>(vertices);
}

static std::shared_ptr<SlideRecord>
parse_color(std::smatch& matches) {
    uint8_t color = std::stoi(matches[1]);
    return std::make_shared<SlideRecordColor>(color);
}

static std::shared_ptr<SlideRecord>
parse_end_of_file(std::smatch&) {
    return std::make_shared<SlideRecordEndOfFile>();
}

std::shared_ptr<SlideRecord>
parse_slide_record_text(const std::string& str)
{
    static std::regex comment {"^#.*"};

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
                std::function<std::shared_ptr<SlideRecord>(std::smatch&)>>>;

    static parsers_t parsers {
        {comment, parse_comment},
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
            return parser(matches);
        }
    }

    throw std::runtime_error{"Invalid slide record: " + str};
}

} // namespace libslide
