#ifndef __GSLIDE_H__
#define __GSLIDE_H__

#include <glib.h>

G_BEGIN_DECLS

int gslide_draw(void *cr,
                unsigned x,
                unsigned y,
                unsigned width,
                unsigned height,
                const char *slide_uri);

G_END_DECLS

#endif // __GSLIDE_H__
