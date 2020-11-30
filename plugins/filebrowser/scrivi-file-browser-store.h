/*
 * scrivi-file-browser-store.h - Scrivi plugin providing easy file access 
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

#ifndef __SCRIVI_FILE_BROWSER_STORE_H__
#define __SCRIVI_FILE_BROWSER_STORE_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define SCRIVI_TYPE_FILE_BROWSER_STORE			(scrivi_file_browser_store_get_type ())
#define SCRIVI_FILE_BROWSER_STORE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_BROWSER_STORE, ScriviFileBrowserStore))
#define SCRIVI_FILE_BROWSER_STORE_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_BROWSER_STORE, ScriviFileBrowserStore const))
#define SCRIVI_FILE_BROWSER_STORE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_FILE_BROWSER_STORE, ScriviFileBrowserStoreClass))
#define SCRIVI_IS_FILE_BROWSER_STORE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_FILE_BROWSER_STORE))
#define SCRIVI_IS_FILE_BROWSER_STORE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_FILE_BROWSER_STORE))
#define SCRIVI_FILE_BROWSER_STORE_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_FILE_BROWSER_STORE, ScriviFileBrowserStoreClass))

typedef enum 
{
	SCRIVI_FILE_BROWSER_STORE_COLUMN_ICON = 0,
	SCRIVI_FILE_BROWSER_STORE_COLUMN_NAME,
	SCRIVI_FILE_BROWSER_STORE_COLUMN_URI,
	SCRIVI_FILE_BROWSER_STORE_COLUMN_FLAGS,
	SCRIVI_FILE_BROWSER_STORE_COLUMN_EMBLEM,
	SCRIVI_FILE_BROWSER_STORE_COLUMN_NUM
} ScriviFileBrowserStoreColumn;

typedef enum 
{
	SCRIVI_FILE_BROWSER_STORE_FLAG_IS_DIRECTORY = 1 << 0,
	SCRIVI_FILE_BROWSER_STORE_FLAG_IS_HIDDEN    = 1 << 1,
	SCRIVI_FILE_BROWSER_STORE_FLAG_IS_TEXT      = 1 << 2,
	SCRIVI_FILE_BROWSER_STORE_FLAG_LOADED       = 1 << 3,
	SCRIVI_FILE_BROWSER_STORE_FLAG_IS_FILTERED  = 1 << 4,
	SCRIVI_FILE_BROWSER_STORE_FLAG_IS_DUMMY     = 1 << 5
} ScriviFileBrowserStoreFlag;

typedef enum 
{
	SCRIVI_FILE_BROWSER_STORE_RESULT_OK,
	SCRIVI_FILE_BROWSER_STORE_RESULT_NO_CHANGE,
	SCRIVI_FILE_BROWSER_STORE_RESULT_ERROR,
	SCRIVI_FILE_BROWSER_STORE_RESULT_NO_TRASH,
	SCRIVI_FILE_BROWSER_STORE_RESULT_MOUNTING,
	SCRIVI_FILE_BROWSER_STORE_RESULT_NUM
} ScriviFileBrowserStoreResult;

typedef enum 
{
	SCRIVI_FILE_BROWSER_STORE_FILTER_MODE_NONE        = 0,
	SCRIVI_FILE_BROWSER_STORE_FILTER_MODE_HIDE_HIDDEN = 1 << 0,
	SCRIVI_FILE_BROWSER_STORE_FILTER_MODE_HIDE_BINARY = 1 << 1
} ScriviFileBrowserStoreFilterMode;

#define FILE_IS_DIR(flags)	(flags & SCRIVI_FILE_BROWSER_STORE_FLAG_IS_DIRECTORY)
#define FILE_IS_HIDDEN(flags)	(flags & SCRIVI_FILE_BROWSER_STORE_FLAG_IS_HIDDEN)
#define FILE_IS_TEXT(flags)	(flags & SCRIVI_FILE_BROWSER_STORE_FLAG_IS_TEXT)
#define FILE_LOADED(flags)	(flags & SCRIVI_FILE_BROWSER_STORE_FLAG_LOADED)
#define FILE_IS_FILTERED(flags)	(flags & SCRIVI_FILE_BROWSER_STORE_FLAG_IS_FILTERED)
#define FILE_IS_DUMMY(flags)	(flags & SCRIVI_FILE_BROWSER_STORE_FLAG_IS_DUMMY)

typedef struct _ScriviFileBrowserStore        ScriviFileBrowserStore;
typedef struct _ScriviFileBrowserStoreClass   ScriviFileBrowserStoreClass;
typedef struct _ScriviFileBrowserStorePrivate ScriviFileBrowserStorePrivate;

typedef gboolean (*ScriviFileBrowserStoreFilterFunc) (ScriviFileBrowserStore
						     * model,
						     GtkTreeIter * iter,
						     gpointer user_data);

struct _ScriviFileBrowserStore 
{
	GObject parent;

	ScriviFileBrowserStorePrivate *priv;
};

struct _ScriviFileBrowserStoreClass {
	GObjectClass parent_class;

	/* Signals */
	void (*begin_loading)        (ScriviFileBrowserStore * model,
			              GtkTreeIter * iter);
	void (*end_loading)          (ScriviFileBrowserStore * model,
			              GtkTreeIter * iter);
	void (*error)                (ScriviFileBrowserStore * model, 
	                              guint code,
		                      gchar * message);
	gboolean (*no_trash)	     (ScriviFileBrowserStore * model,
				      GList * files);
	void (*rename)		     (ScriviFileBrowserStore * model,
				      const gchar * olduri,
				      const gchar * newuri);
	void (*begin_refresh)	     (ScriviFileBrowserStore * model);
	void (*end_refresh)	     (ScriviFileBrowserStore * model);
	void (*unload)		     (ScriviFileBrowserStore * model,
				      const gchar * uri);
};

GType scrivi_file_browser_store_get_type               (void) G_GNUC_CONST;
GType scrivi_file_browser_store_register_type          (GTypeModule * module);

ScriviFileBrowserStore *scrivi_file_browser_store_new   (gchar const *root);

ScriviFileBrowserStoreResult
scrivi_file_browser_store_set_root_and_virtual_root    (ScriviFileBrowserStore * model,
						       gchar const *root,
			  			       gchar const *virtual_root);
ScriviFileBrowserStoreResult
scrivi_file_browser_store_set_root                     (ScriviFileBrowserStore * model,
				                       gchar const *root);
ScriviFileBrowserStoreResult
scrivi_file_browser_store_set_virtual_root             (ScriviFileBrowserStore * model,
					               GtkTreeIter * iter);
ScriviFileBrowserStoreResult
scrivi_file_browser_store_set_virtual_root_from_string (ScriviFileBrowserStore * model, 
                                                       gchar const *root);
ScriviFileBrowserStoreResult
scrivi_file_browser_store_set_virtual_root_up          (ScriviFileBrowserStore * model);
ScriviFileBrowserStoreResult
scrivi_file_browser_store_set_virtual_root_top         (ScriviFileBrowserStore * model);

gboolean
scrivi_file_browser_store_get_iter_virtual_root        (ScriviFileBrowserStore * model, 
                                                       GtkTreeIter * iter);
gboolean scrivi_file_browser_store_get_iter_root       (ScriviFileBrowserStore * model,
						       GtkTreeIter * iter);
gchar * scrivi_file_browser_store_get_root             (ScriviFileBrowserStore * model);
gchar * scrivi_file_browser_store_get_virtual_root     (ScriviFileBrowserStore * model);

gboolean scrivi_file_browser_store_iter_equal          (ScriviFileBrowserStore * model, 
                                                       GtkTreeIter * iter1,
					               GtkTreeIter * iter2);

void scrivi_file_browser_store_set_value               (ScriviFileBrowserStore * tree_model, 
                                                       GtkTreeIter * iter,
                                                       gint column, 
                                                       GValue * value);

void _scrivi_file_browser_store_iter_expanded          (ScriviFileBrowserStore * model, 
                                                       GtkTreeIter * iter);
void _scrivi_file_browser_store_iter_collapsed         (ScriviFileBrowserStore * model, 
                                                       GtkTreeIter * iter);

ScriviFileBrowserStoreFilterMode
scrivi_file_browser_store_get_filter_mode              (ScriviFileBrowserStore * model);
void scrivi_file_browser_store_set_filter_mode         (ScriviFileBrowserStore * model,
                                                       ScriviFileBrowserStoreFilterMode mode);
void scrivi_file_browser_store_set_filter_func         (ScriviFileBrowserStore * model,
                                                       ScriviFileBrowserStoreFilterFunc func, 
                                                       gpointer user_data);
void scrivi_file_browser_store_refilter                (ScriviFileBrowserStore * model);
ScriviFileBrowserStoreFilterMode
scrivi_file_browser_store_filter_mode_get_default      (void);

void scrivi_file_browser_store_refresh                 (ScriviFileBrowserStore * model);
gboolean scrivi_file_browser_store_rename              (ScriviFileBrowserStore * model,
                                                       GtkTreeIter * iter,
                                                       gchar const *new_name,
                                                       GError ** error);
ScriviFileBrowserStoreResult
scrivi_file_browser_store_delete                       (ScriviFileBrowserStore * model,
                                                       GtkTreeIter * iter, 
                                                       gboolean trash);
ScriviFileBrowserStoreResult
scrivi_file_browser_store_delete_all                   (ScriviFileBrowserStore * model,
                                                       GList *rows, 
                                                       gboolean trash);

gboolean scrivi_file_browser_store_new_file            (ScriviFileBrowserStore * model,
                                                       GtkTreeIter * parent,
                                                       GtkTreeIter * iter);
gboolean scrivi_file_browser_store_new_directory       (ScriviFileBrowserStore * model,
                                                       GtkTreeIter * parent,
                                                       GtkTreeIter * iter);

void scrivi_file_browser_store_cancel_mount_operation  (ScriviFileBrowserStore *store);

G_END_DECLS
#endif				/* __SCRIVI_FILE_BROWSER_STORE_H__ */

// ex:ts=8:noet:
