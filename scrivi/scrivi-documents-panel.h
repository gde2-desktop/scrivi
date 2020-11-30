/*
 * scrivi-documents-panel.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Maggi 
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
 * Modified by the scrivi Team, 2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id$
 */

#ifndef __SCRIVI_DOCUMENTS_PANEL_H__
#define __SCRIVI_DOCUMENTS_PANEL_H__

#include <gtk/gtk.h>

#include <scrivi/scrivi-window.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_DOCUMENTS_PANEL              (scrivi_documents_panel_get_type())
#define SCRIVI_DOCUMENTS_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_DOCUMENTS_PANEL, ScriviDocumentsPanel))
#define SCRIVI_DOCUMENTS_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_DOCUMENTS_PANEL, ScriviDocumentsPanelClass))
#define SCRIVI_IS_DOCUMENTS_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_DOCUMENTS_PANEL))
#define SCRIVI_IS_DOCUMENTS_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_DOCUMENTS_PANEL))
#define SCRIVI_DOCUMENTS_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_DOCUMENTS_PANEL, ScriviDocumentsPanelClass))

/* Private structure type */
typedef struct _ScriviDocumentsPanelPrivate ScriviDocumentsPanelPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviDocumentsPanel ScriviDocumentsPanel;

struct _ScriviDocumentsPanel 
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBox vbox;
#else
	GtkVBox vbox;
#endif

	/*< private > */
	ScriviDocumentsPanelPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviDocumentsPanelClass ScriviDocumentsPanelClass;

struct _ScriviDocumentsPanelClass 
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBoxClass parent_class;
#else
	GtkVBoxClass parent_class;
#endif
};

/*
 * Public methods
 */
GType 		 scrivi_documents_panel_get_type	(void) G_GNUC_CONST;

GtkWidget	*scrivi_documents_panel_new 	(ScriviWindow *window);

G_END_DECLS

#endif  /* __SCRIVI_DOCUMENTS_PANEL_H__  */
