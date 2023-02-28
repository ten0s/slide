/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef __GI_SLIDE_H__
#define __GI_SLIDE_H__

#include <glib.h>

G_BEGIN_DECLS

int gi_slide_draw(gpointer cr,
                  unsigned x,
                  unsigned y,
                  unsigned width,
                  unsigned height,
                  const gchar *slide_uri);

G_END_DECLS

#endif // __GI_SLIDE_H__
