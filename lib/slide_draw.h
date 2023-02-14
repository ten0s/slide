#ifndef __SLIDE_DRAW_H__
#define __SLIDE_DRAW_H__

#include <cairo.h>

#ifdef __cplusplus
extern "C" {
#endif

int slide_draw(cairo_t *cr,
               unsigned x,
               unsigned y,
               unsigned width,
               unsigned height,
               const char *slide_uri);

#ifdef __cplusplus
}
#endif

#endif // __SLIDE_DRAW_H__
