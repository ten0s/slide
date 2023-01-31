#include "gi-slide.h"
#include "slide.h"

/**
 * gi_slide_draw:
 * @cr: (transfer none) (not nullable): Cairo context
 * @x: X-offset from the upper-left corner of the tile, in pixels. Must be a positive value.
 * @y: Y-offset from the upper-left corner of the tile, in pixels. Must be a positive value.
 * @width: Width of the image, in pixels.
 * @height: Height of the image, in pixels.
 * @sldname: (transfer none) (not nullable): Slide identifier.
 *
 * Draws on the slide on Cairo context.
 */
void
gi_slide_draw(void *cr, int x, int y, int width, int height, const char *sldname)
{
    slide_draw((cairo_t *)cr, x, y, width, height, sldname);
}
