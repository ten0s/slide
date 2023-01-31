#ifndef __SLIDE_H__
#define __SLIDE_H__

#include <cairo.h>

#ifdef __cplusplus
extern "C" {
#endif

void slide_draw(cairo_t *cr, int x, int y, int width, int height, const char *sldname);

#ifdef __cplusplus
}
#endif

#endif // __SLIDE_H__
