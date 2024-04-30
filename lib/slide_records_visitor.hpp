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

#ifndef __SLIDE_RECORDS_VISITOR_HPP__
#define __SLIDE_RECORDS_VISITOR_HPP__

namespace libslide {

class SlideRecordVector;
class SlideRecordOffsetVector;
class SlideRecordCommonEndpoint;
class SlideRecordSolidFillPolygon;
class SlideRecordColor;
class SlideRecordEndOfFile;

class SlideRecordsVisitor {
public:
    virtual ~SlideRecordsVisitor() {}
    virtual void accept(SlideRecordVector&) = 0;
    virtual void accept(SlideRecordOffsetVector&) = 0;
    virtual void accept(SlideRecordCommonEndpoint&) = 0;
    virtual void accept(SlideRecordSolidFillPolygon&) = 0;
    virtual void accept(SlideRecordColor&) = 0;
    virtual void accept(SlideRecordEndOfFile&) = 0;
};

} // namespace libslide

#endif // __SLIDE_RECORDS_VISITOR_HPP__
