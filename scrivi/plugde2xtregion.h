/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- 
 *
 * plugde2xtregion.h - GtkTextMark based region utility functions
 *
 * This file is part of the GtkSourceView widget
 *
 * Copyright (C) 2002 Gustavo Giráldez <gustavo.giraldez@gmx.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.  
 */

#ifndef __SCRIVI_TEXT_REGION_H__
#define __SCRIVI_TEXT_REGION_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _ScriviTextRegion		ScriviTextRegion;
typedef struct _ScriviTextRegionIterator	ScriviTextRegionIterator;

struct _ScriviTextRegionIterator {
	/* ScriviTextRegionIterator is an opaque datatype; ignore all these fields.
	 * Initialize the iter with scrivi_text_region_get_iterator
	 * function
	 */
	/*< private >*/
	gpointer dummy1;
	guint32  dummy2;
	gpointer dummy3;	
};

ScriviTextRegion *scrivi_text_region_new                          (GtkTextBuffer *buffer);
void           scrivi_text_region_destroy                      (ScriviTextRegion *region,
							     gboolean       delete_marks);

GtkTextBuffer *scrivi_text_region_get_buffer                   (ScriviTextRegion *region);

void           scrivi_text_region_add                          (ScriviTextRegion     *region,
							     const GtkTextIter *_start,
							     const GtkTextIter *_end);

void           scrivi_text_region_subtract                     (ScriviTextRegion     *region,
							     const GtkTextIter *_start,
							     const GtkTextIter *_end);

gint           scrivi_text_region_subregions                   (ScriviTextRegion *region);

gboolean       scrivi_text_region_nth_subregion                (ScriviTextRegion *region,
							     guint          subregion,
							     GtkTextIter   *start,
							     GtkTextIter   *end);

ScriviTextRegion *scrivi_text_region_intersect                    (ScriviTextRegion     *region,
							     const GtkTextIter *_start,
							     const GtkTextIter *_end);

void           scrivi_text_region_get_iterator                 (ScriviTextRegion         *region,
                                                             ScriviTextRegionIterator *iter,
                                                             guint                  start);

gboolean       scrivi_text_region_iterator_is_end              (ScriviTextRegionIterator *iter);

/* Returns FALSE if iterator is the end iterator */
gboolean       scrivi_text_region_iterator_next	            (ScriviTextRegionIterator *iter);

void           scrivi_text_region_iterator_get_subregion       (ScriviTextRegionIterator *iter,
							     GtkTextIter           *start,
							     GtkTextIter           *end);

void           scrivi_text_region_debug_print                  (ScriviTextRegion *region);

G_END_DECLS

#endif /* __SCRIVI_TEXT_REGION_H__ */
