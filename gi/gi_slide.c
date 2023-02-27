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

#include "gi_slide.h"
#include "../lib/slide_draw.h"

/**
 * gi_slide_draw:
 * @cr: (transfer none) (not nullable): Cairo context
 * @x: X-offset from the upper-left corner of the tile, in pixels. Must be a positive value.
 * @y: Y-offset from the upper-left corner of the tile, in pixels. Must be a positive value.
 * @width: Width of the image, in pixels.
 * @height: Height of the image, in pixels.
 * @slide_uri: (transfer none) (not nullable): Slide identifier.
 *
 * Draws on the slide on Cairo context.
 */
int
gi_slide_draw(gpointer cr,
              unsigned x,
              unsigned y,
              unsigned width,
              unsigned height,
              const gchar *slide_uri)
{
    return slide_draw((cairo_t *)cr, x, y, width, height, slide_uri);
}
