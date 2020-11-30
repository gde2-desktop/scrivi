/*
 * scrivi-sort-plugin.h
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
 *
 * $Id$
 */

#ifndef __SCRIVI_SORT_PLUGIN_H__
#define __SCRIVI_SORT_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <scrivi/scrivi-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_SORT_PLUGIN		(scrivi_sort_plugin_get_type ())
#define SCRIVI_SORT_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCRIVI_TYPE_SORT_PLUGIN, ScriviSortPlugin))
#define SCRIVI_SORT_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), SCRIVI_TYPE_SORT_PLUGIN, ScriviSortPluginClass))
#define SCRIVI_IS_SORT_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCRIVI_TYPE_SORT_PLUGIN))
#define SCRIVI_IS_SORT_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCRIVI_TYPE_SORT_PLUGIN))
#define SCRIVI_SORT_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SCRIVI_TYPE_SORT_PLUGIN, ScriviSortPluginClass))

/* Private structure type */
typedef struct _ScriviSortPluginPrivate	ScriviSortPluginPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviSortPlugin		ScriviSortPlugin;

struct _ScriviSortPlugin
{
	ScriviPlugin parent_instance;
};

/*
 * Class definition
 */
typedef struct _ScriviSortPluginClass	ScriviSortPluginClass;

struct _ScriviSortPluginClass
{
	ScriviPluginClass parent_class;
};

/*
 * Public methods
 */
GType	scrivi_sort_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_scrivi_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __SCRIVI_SORT_PLUGIN_H__ */
