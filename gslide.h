#ifndef __GSLIDE_H__
#define __GSLIDE_H__

#include <glib.h>

G_BEGIN_DECLS

int gslide_draw(void *cr, int x, int y, int width, int height, const char *sldname);

G_END_DECLS

#endif // __GSLIDE_H__
