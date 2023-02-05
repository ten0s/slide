#include "gslide.h"
#include "slide.h"

/**
 * gslide_draw:
 * @cr: (transfer none) (not nullable): Cairo context
 * @x: X-offset from the upper-left corner of the tile, in pixels. Must be a positive value.
 * @y: Y-offset from the upper-left corner of the tile, in pixels. Must be a positive value.
 * @width: Width of the image, in pixels.
 * @height: Height of the image, in pixels.
 * @sldname: (transfer none) (not nullable): Slide identifier.
 *
 * Draws on the slide on Cairo context.
 */
int
gslide_draw(void *cr,
            unsigned x,
            unsigned y,
            unsigned width,
            unsigned height,
            const char *sldname)
{
    return slide_draw((cairo_t *)cr, x, y, width, height, sldname);
}
