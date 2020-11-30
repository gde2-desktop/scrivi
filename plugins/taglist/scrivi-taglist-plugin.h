/*
 * scrivi-taglist-plugin.h
 * 
 * Copyright (C) 2002-2005 - Paolo Maggi
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
 */

/*
 * Modified by the scrivi Team, 2002-2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id$
 */
 
#ifndef __SCRIVI_TAGLIST_PLUGIN_H__
#define __SCRIVI_TAGLIST_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <scrivi/scrivi-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_TAGLIST_PLUGIN		(scrivi_taglist_plugin_get_type ())
#define SCRIVI_TAGLIST_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCRIVI_TYPE_TAGLIST_PLUGIN, ScriviTaglistPlugin))
#define SCRIVI_TAGLIST_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), SCRIVI_TYPE_TAGLIST_PLUGIN, ScriviTaglistPluginClass))
#define SCRIVI_IS_TAGLIST_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCRIVI_TYPE_TAGLIST_PLUGIN))
#define SCRIVI_IS_TAGLIST_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCRIVI_TYPE_TAGLIST_PLUGIN))
#define SCRIVI_TAGLIST_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SCRIVI_TYPE_TAGLIST_PLUGIN, ScriviTaglistPluginClass))

/* Private structure type */
typedef struct _ScriviTaglistPluginPrivate	ScriviTaglistPluginPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviTaglistPlugin		ScriviTaglistPlugin;

struct _ScriviTaglistPlugin
{
	ScriviPlugin parent_instance;

	/*< private >*/
	ScriviTaglistPluginPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviTaglistPluginClass	ScriviTaglistPluginClass;

struct _ScriviTaglistPluginClass
{
	ScriviPluginClass parent_class;
};

/*
 * Public methods
 */
GType	scrivi_taglist_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_scrivi_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __SCRIVI_TAGLIST_PLUGIN_H__ */
