/*
 * scrivi-print-preview.h
 *
 * Copyright (C) 2008 Paolo Borelli
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

/*
 * Modified by the scrivi Team, 1998-2006. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id: scrivi-commands-search.c 5931 2007-09-25 20:05:40Z pborelli $
 */


#ifndef __SCRIVI_PRINT_PREVIEW_H__
#define __SCRIVI_PRINT_PREVIEW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_PRINT_PREVIEW            (scrivi_print_preview_get_type ())
#define SCRIVI_PRINT_PREVIEW(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), SCRIVI_TYPE_PRINT_PREVIEW, ScriviPrintPreview))
#define SCRIVI_PRINT_PREVIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_PRINT_PREVIEW, ScriviPrintPreviewClass))
#define SCRIVI_IS_PRINT_PREVIEW(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), SCRIVI_TYPE_PRINT_PREVIEW))
#define SCRIVI_IS_PRINT_PREVIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PRINT_PREVIEW))
#define SCRIVI_PRINT_PREVIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_PRINT_PREVIEW, ScriviPrintPreviewClass))

typedef struct _ScriviPrintPreview        ScriviPrintPreview;
typedef struct _ScriviPrintPreviewPrivate ScriviPrintPreviewPrivate;
typedef struct _ScriviPrintPreviewClass   ScriviPrintPreviewClass;

struct _ScriviPrintPreview
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBox parent;
#else
	GtkVBox parent;
#endif
	ScriviPrintPreviewPrivate *priv;
};

struct _ScriviPrintPreviewClass
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBoxClass parent_class;
#else
	GtkVBoxClass parent_class;
#endif

	void (* close)		(ScriviPrintPreview          *preview);
};


GType		 scrivi_print_preview_get_type	(void) G_GNUC_CONST;

GtkWidget	*scrivi_print_preview_new	(GtkPrintOperation		*op,
						 GtkPrintOperationPreview	*gtk_preview,
						 GtkPrintContext		*context);

G_END_DECLS

#endif /* __SCRIVI_PRINT_PREVIEW_H__ */
