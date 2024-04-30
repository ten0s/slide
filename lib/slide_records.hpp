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

#ifndef __SLIDE_RECORDS_HPP__
#define __SLIDE_RECORDS_HPP__

#include <cstdint>
#include <vector>
#include "slide_records_visitor.hpp"

namespace libslide {

class SlideRecord {
public:
    virtual ~SlideRecord() {}
    virtual void visit(SlideRecordsVisitor& v) = 0;
};

class SlideRecordVector : public SlideRecord {
public:
    SlideRecordVector(int16_t x0, int16_t y0,
                      int16_t x1, int16_t y1)
        : _x0{x0}, _y0{y0},
          _x1{x1}, _y1{y1}
        {}

    int16_t x0() const { return _x0; }
    int16_t y0() const { return _y0; }
    int16_t x1() const { return _x1; }
    int16_t y1() const { return _y1; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int16_t _x0;
    int16_t _y0;
    int16_t _x1;
    int16_t _y1;
};

class SlideRecordOffsetVector : public SlideRecord {
public:
    SlideRecordOffsetVector(int8_t dx0, int8_t dy0,
                            int8_t dx1, int8_t dy1)
        : _dx0{dx0}, _dy0{dy0},
          _dx1{dx1}, _dy1{dy1}
        {}

    int8_t dx0() const { return _dx0; }
    int8_t dy0() const { return _dy0; }
    int8_t dx1() const { return _dx1; }
    int8_t dy1() const { return _dy1; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int8_t _dx0;
    int8_t _dy0;
    int8_t _dx1;
    int8_t _dy1;
};

class SlideRecordCommonEndpoint : public SlideRecord {
public:
    SlideRecordCommonEndpoint(int8_t dx0, int8_t dy0)
        : _dx0{dx0}, _dy0{dy0}
        {}

    int8_t dx0() const { return _dx0; }
    int8_t dy0() const { return _dy0; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int8_t _dx0;
    int8_t _dy0;
};

class SlideRecordSolidFillPolygon : public SlideRecord {
public:
    using vertex_t   = std::pair<int16_t, int16_t>;
    using vertices_t = std::vector<vertex_t>;

    explicit SlideRecordSolidFillPolygon(const vertices_t& vertices)
        : _vertices{vertices}
        {}

    const vertices_t vertices() const { return _vertices; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    vertices_t _vertices;
};

class SlideRecordColor : public SlideRecord {
public:
    explicit SlideRecordColor(uint8_t color)
        : _color{color}
        {}

    uint8_t color() const { return _color; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    uint8_t _color;
};

class SlideRecordEndOfFile : public SlideRecord {
public:
    SlideRecordEndOfFile() {}

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }
};

} // namespace libslide

#endif // __SLIDE_RECORDS_HPP__
