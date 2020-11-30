/*
 * scrivi-file-browser-widget.h - Scrivi plugin providing easy file access 
 * from the sidepanel
 *
 * Copyright (C) 2006 - Jesse van den Kieboom <jesse@icecrew.nl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __SCRIVI_FILE_BROWSER_WIDGET_H__
#define __SCRIVI_FILE_BROWSER_WIDGET_H__

#include <gtk/gtk.h>
#include "scrivi-file-browser-store.h"
#include "scrivi-file-bookmarks-store.h"
#include "scrivi-file-browser-view.h"

G_BEGIN_DECLS
#define SCRIVI_TYPE_FILE_BROWSER_WIDGET			(scrivi_file_browser_widget_get_type ())
#define SCRIVI_FILE_BROWSER_WIDGET(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_BROWSER_WIDGET, ScriviFileBrowserWidget))
#define SCRIVI_FILE_BROWSER_WIDGET_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_BROWSER_WIDGET, ScriviFileBrowserWidget const))
#define SCRIVI_FILE_BROWSER_WIDGET_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_FILE_BROWSER_WIDGET, ScriviFileBrowserWidgetClass))
#define SCRIVI_IS_FILE_BROWSER_WIDGET(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_FILE_BROWSER_WIDGET))
#define SCRIVI_IS_FILE_BROWSER_WIDGET_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_FILE_BROWSER_WIDGET))
#define SCRIVI_FILE_BROWSER_WIDGET_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_FILE_BROWSER_WIDGET, ScriviFileBrowserWidgetClass))

typedef struct _ScriviFileBrowserWidget        ScriviFileBrowserWidget;
typedef struct _ScriviFileBrowserWidgetClass   ScriviFileBrowserWidgetClass;
typedef struct _ScriviFileBrowserWidgetPrivate ScriviFileBrowserWidgetPrivate;

typedef
gboolean (*ScriviFileBrowserWidgetFilterFunc) (ScriviFileBrowserWidget * obj,
					      ScriviFileBrowserStore *
					      model, GtkTreeIter * iter,
					      gpointer user_data);

struct _ScriviFileBrowserWidget 
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBox parent;
#else
	GtkVBox parent;
#endif

	ScriviFileBrowserWidgetPrivate *priv;
};

struct _ScriviFileBrowserWidgetClass 
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBoxClass parent_class;
#else
	GtkVBoxClass parent_class;
#endif

	/* Signals */
	void (*uri_activated)        (ScriviFileBrowserWidget * widget,
			              gchar const *uri);
	void (*error)                (ScriviFileBrowserWidget * widget, 
	                              guint code,
		                      gchar const *message);
	gboolean (*confirm_delete)   (ScriviFileBrowserWidget * widget,
	                              ScriviFileBrowserStore * model,
	                              GList *list);
	gboolean (*confirm_no_trash) (ScriviFileBrowserWidget * widget,
	                              GList *list);
};

GType scrivi_file_browser_widget_get_type            (void) G_GNUC_CONST;
GType scrivi_file_browser_widget_register_type       (GTypeModule * module);

GtkWidget *scrivi_file_browser_widget_new            (const gchar *data_dir);

void scrivi_file_browser_widget_show_bookmarks       (ScriviFileBrowserWidget * obj);
void scrivi_file_browser_widget_show_files           (ScriviFileBrowserWidget * obj);

void scrivi_file_browser_widget_set_root             (ScriviFileBrowserWidget * obj,
                                                     gchar const *root,
                                                     gboolean virtual_root);
void
scrivi_file_browser_widget_set_root_and_virtual_root (ScriviFileBrowserWidget * obj,
						     gchar const *root,
						     gchar const *virtual_root);

gboolean
scrivi_file_browser_widget_get_selected_directory    (ScriviFileBrowserWidget * obj, 
                                                     GtkTreeIter * iter);

ScriviFileBrowserStore * 
scrivi_file_browser_widget_get_browser_store         (ScriviFileBrowserWidget * obj);
ScriviFileBookmarksStore * 
scrivi_file_browser_widget_get_bookmarks_store       (ScriviFileBrowserWidget * obj);
ScriviFileBrowserView *
scrivi_file_browser_widget_get_browser_view          (ScriviFileBrowserWidget * obj);
GtkWidget *
scrivi_file_browser_widget_get_filter_entry          (ScriviFileBrowserWidget * obj);

GtkUIManager * 
scrivi_file_browser_widget_get_ui_manager            (ScriviFileBrowserWidget * obj);

gulong scrivi_file_browser_widget_add_filter         (ScriviFileBrowserWidget * obj,
                                                     ScriviFileBrowserWidgetFilterFunc func, 
                                                     gpointer user_data,
                                                     GDestroyNotify notify);
void scrivi_file_browser_widget_remove_filter        (ScriviFileBrowserWidget * obj,
                                                     gulong id);
void scrivi_file_browser_widget_set_filter_pattern   (ScriviFileBrowserWidget * obj,
                                                     gchar const *pattern);

void scrivi_file_browser_widget_refresh		    (ScriviFileBrowserWidget * obj);
void scrivi_file_browser_widget_history_back	    (ScriviFileBrowserWidget * obj);
void scrivi_file_browser_widget_history_forward	    (ScriviFileBrowserWidget * obj);

G_END_DECLS
#endif /* __SCRIVI_FILE_BROWSER_WIDGET_H__ */

// ex:ts=8:noet:
