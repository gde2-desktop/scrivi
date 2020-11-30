/*
 * scrivi-file-browser-plugin.h - Scrivi plugin providing easy file access 
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

#ifndef __SCRIVI_FILE_BROWSER_PLUGIN_H__
#define __SCRIVI_FILE_BROWSER_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <scrivi/scrivi-plugin.h>

G_BEGIN_DECLS
/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_FILE_BROWSER_PLUGIN		(filetree_plugin_get_type ())
#define SCRIVI_FILE_BROWSER_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCRIVI_TYPE_FILE_BROWSER_PLUGIN, ScriviFileBrowserPlugin))
#define SCRIVI_FILE_BROWSER_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), SCRIVI_TYPE_FILE_BROWSER_PLUGIN, ScriviFileBrowserPluginClass))
#define SCRIVI_IS_FILE_BROWSER_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCRIVI_TYPE_FILE_BROWSER_PLUGIN))
#define SCRIVI_IS_FILE_BROWSER_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCRIVI_TYPE_FILE_BROWSER_PLUGIN))
#define SCRIVI_FILE_BROWSER_GET_CLASS(o)		(G_TYPE_INSTANCE_GET_CLASS ((o), SCRIVI_TYPE_FILE_BROWSER_PLUGIN, ScriviFileBrowserPluginClass))

/* Private structure type */
typedef struct _ScriviFileBrowserPluginPrivate ScriviFileBrowserPluginPrivate;
typedef struct _ScriviFileBrowserPlugin        ScriviFileBrowserPlugin;
typedef struct _ScriviFileBrowserPluginClass   ScriviFileBrowserPluginClass;

struct _ScriviFileBrowserPlugin 
{
	ScriviPlugin parent_instance;

	/*< private > */
	ScriviFileBrowserPluginPrivate *priv;
};



struct _ScriviFileBrowserPluginClass 
{
	ScriviPluginClass parent_class;
};

/*
 * Public methods
 */
GType filetree_plugin_get_type              (void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_scrivi_plugin (GTypeModule * module);

G_END_DECLS
#endif /* __SCRIVI_FILE_BROWSER_PLUGIN_H__ */

// ex:ts=8:noet:
