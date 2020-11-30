/*
 * scrivi-file-bookmarks-store.h - Scrivi plugin providing easy file access 
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

#ifndef __SCRIVI_FILE_BOOKMARKS_STORE_H__
#define __SCRIVI_FILE_BOOKMARKS_STORE_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define SCRIVI_TYPE_FILE_BOOKMARKS_STORE			(scrivi_file_bookmarks_store_get_type ())
#define SCRIVI_FILE_BOOKMARKS_STORE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_BOOKMARKS_STORE, ScriviFileBookmarksStore))
#define SCRIVI_FILE_BOOKMARKS_STORE_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_BOOKMARKS_STORE, ScriviFileBookmarksStore const))
#define SCRIVI_FILE_BOOKMARKS_STORE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_FILE_BOOKMARKS_STORE, ScriviFileBookmarksStoreClass))
#define SCRIVI_IS_FILE_BOOKMARKS_STORE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_FILE_BOOKMARKS_STORE))
#define SCRIVI_IS_FILE_BOOKMARKS_STORE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_FILE_BOOKMARKS_STORE))
#define SCRIVI_FILE_BOOKMARKS_STORE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_FILE_BOOKMARKS_STORE, ScriviFileBookmarksStoreClass))

typedef struct _ScriviFileBookmarksStore        ScriviFileBookmarksStore;
typedef struct _ScriviFileBookmarksStoreClass   ScriviFileBookmarksStoreClass;
typedef struct _ScriviFileBookmarksStorePrivate ScriviFileBookmarksStorePrivate;

enum 
{
	SCRIVI_FILE_BOOKMARKS_STORE_COLUMN_ICON = 0,
	SCRIVI_FILE_BOOKMARKS_STORE_COLUMN_NAME,
	SCRIVI_FILE_BOOKMARKS_STORE_COLUMN_OBJECT,
	SCRIVI_FILE_BOOKMARKS_STORE_COLUMN_FLAGS,
	SCRIVI_FILE_BOOKMARKS_STORE_N_COLUMNS
};

enum 
{
	SCRIVI_FILE_BOOKMARKS_STORE_NONE            	= 0,
	SCRIVI_FILE_BOOKMARKS_STORE_IS_SEPARATOR   	= 1 << 0,  /* Separator item */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_SPECIAL_DIR 	= 1 << 1,  /* Special user dir */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_HOME         	= 1 << 2,  /* The special Home user directory */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_DESKTOP      	= 1 << 3,  /* The special Desktop user directory */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_DOCUMENTS    	= 1 << 4,  /* The special Documents user directory */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_FS        	= 1 << 5,  /* A mount object */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_MOUNT        	= 1 << 6,  /* A mount object */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_VOLUME        	= 1 << 7,  /* A volume object */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_DRIVE        	= 1 << 8,  /* A drive object */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_ROOT         	= 1 << 9,  /* The root file system (file:///) */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_BOOKMARK     	= 1 << 10,  /* A gtk bookmark */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_REMOTE_BOOKMARK	= 1 << 11, /* A remote gtk bookmark */
	SCRIVI_FILE_BOOKMARKS_STORE_IS_LOCAL_BOOKMARK	= 1 << 12  /* A local gtk bookmark */
};

struct _ScriviFileBookmarksStore 
{
	GtkTreeStore parent;

	ScriviFileBookmarksStorePrivate *priv;
};

struct _ScriviFileBookmarksStoreClass 
{
	GtkTreeStoreClass parent_class;
};

GType scrivi_file_bookmarks_store_get_type               (void) G_GNUC_CONST;
GType scrivi_file_bookmarks_store_register_type          (GTypeModule * module);

ScriviFileBookmarksStore *scrivi_file_bookmarks_store_new (void);
gchar *scrivi_file_bookmarks_store_get_uri               (ScriviFileBookmarksStore * model,
					                 GtkTreeIter * iter);
void scrivi_file_bookmarks_store_refresh                 (ScriviFileBookmarksStore * model);

G_END_DECLS
#endif				/* __SCRIVI_FILE_BOOKMARKS_STORE_H__ */

// ex:ts=8:noet:
