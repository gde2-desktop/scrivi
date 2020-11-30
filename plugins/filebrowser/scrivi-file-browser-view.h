/*
 * scrivi-file-browser-view.h - Scrivi plugin providing easy file access 
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

#ifndef __SCRIVI_FILE_BROWSER_VIEW_H__
#define __SCRIVI_FILE_BROWSER_VIEW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define SCRIVI_TYPE_FILE_BROWSER_VIEW			(scrivi_file_browser_view_get_type ())
#define SCRIVI_FILE_BROWSER_VIEW(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_BROWSER_VIEW, ScriviFileBrowserView))
#define SCRIVI_FILE_BROWSER_VIEW_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_BROWSER_VIEW, ScriviFileBrowserView const))
#define SCRIVI_FILE_BROWSER_VIEW_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_FILE_BROWSER_VIEW, ScriviFileBrowserViewClass))
#define SCRIVI_IS_FILE_BROWSER_VIEW(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_FILE_BROWSER_VIEW))
#define SCRIVI_IS_FILE_BROWSER_VIEW_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_FILE_BROWSER_VIEW))
#define SCRIVI_FILE_BROWSER_VIEW_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_FILE_BROWSER_VIEW, ScriviFileBrowserViewClass))

typedef struct _ScriviFileBrowserView        ScriviFileBrowserView;
typedef struct _ScriviFileBrowserViewClass   ScriviFileBrowserViewClass;
typedef struct _ScriviFileBrowserViewPrivate ScriviFileBrowserViewPrivate;

typedef enum {
	SCRIVI_FILE_BROWSER_VIEW_CLICK_POLICY_DOUBLE,
	SCRIVI_FILE_BROWSER_VIEW_CLICK_POLICY_SINGLE	
} ScriviFileBrowserViewClickPolicy;

struct _ScriviFileBrowserView 
{
	GtkTreeView parent;

	ScriviFileBrowserViewPrivate *priv;
};

struct _ScriviFileBrowserViewClass 
{
	GtkTreeViewClass parent_class;

	/* Signals */
	void (*error) (ScriviFileBrowserView * filetree, 
	               guint code,
		       gchar const *message);
	void (*file_activated) (ScriviFileBrowserView * filetree,
				    GtkTreeIter *iter);
	void (*directory_activated) (ScriviFileBrowserView * filetree,
				    GtkTreeIter *iter);
	void (*bookmark_activated) (ScriviFileBrowserView * filetree,
				    GtkTreeIter *iter);
};

GType scrivi_file_browser_view_get_type			(void) G_GNUC_CONST;
GType scrivi_file_browser_view_register_type		(GTypeModule 			* module);

GtkWidget *scrivi_file_browser_view_new			(void);
void scrivi_file_browser_view_set_model			(ScriviFileBrowserView 		* tree_view,
							 GtkTreeModel 			* model);
void scrivi_file_browser_view_start_rename		(ScriviFileBrowserView 		* tree_view, 
							 GtkTreeIter 			* iter);
void scrivi_file_browser_view_set_click_policy		(ScriviFileBrowserView 		* tree_view,
							 ScriviFileBrowserViewClickPolicy  policy);
void scrivi_file_browser_view_set_restore_expand_state	(ScriviFileBrowserView 		* tree_view,
							 gboolean 			  restore_expand_state);

G_END_DECLS
#endif				/* __SCRIVI_FILE_BROWSER_VIEW_H__ */

// ex:ts=8:noet:
