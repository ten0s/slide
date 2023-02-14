#ifndef __GI_SLIDE_H__
#define __GI_SLIDE_H__

#include <glib.h>

G_BEGIN_DECLS

int gi_slide_draw(void *cr,
                  unsigned x,
                  unsigned y,
                  unsigned width,
                  unsigned height,
                  const char *slide_uri);

G_END_DECLS

#endif // __GI_SLIDE_H__
