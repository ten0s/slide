/**
 *   AutoCAD slide library
 *
 *   Copyright (C) 2023-2025 Dmitry Klionsky aka ten0s <dm.klionsky@gmail.com>
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

#ifndef __SLIDE_DRAW_CAIRO_H__
#define __SLIDE_DRAW_CAIRO_H__

#include <cairo.h>

#ifdef __cplusplus
extern "C" {
#endif

int slide_draw_cairo(cairo_t *cr,
                     unsigned x,
                     unsigned y,
                     unsigned width,
                     unsigned height,
                     const char *slide_uri);

#ifdef __cplusplus
}
#endif

#endif // __SLIDE_DRAW_CAIRO_H__
