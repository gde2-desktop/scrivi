/*
 * scrivi-trail-save-plugin.h
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

#ifndef __SCRIVI_TRAIL_SAVE_PLUGIN_H__
#define __SCRIVI_TRAIL_SAVE_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <scrivi/scrivi-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_TRAIL_SAVE_PLUGIN		(scrivi_trail_save_plugin_get_type ())
#define SCRIVI_TRAIL_SAVE_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCRIVI_TYPE_TRAIL_SAVE_PLUGIN, ScriviTrailSavePlugin))
#define SCRIVI_TRAIL_SAVE_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), SCRIVI_TYPE_TRAIL_SAVE_PLUGIN, ScriviTrailSavePluginClass))
#define SCRIVI_IS_TRAIL_SAVE_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCRIVI_TYPE_TRAIL_SAVE_PLUGIN))
#define SCRIVI_IS_TRAIL_SAVE_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCRIVI_TYPE_TRAIL_SAVE_PLUGIN))
#define SCRIVI_TRAIL_SAVE_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SCRIVI_TYPE_TRAIL_SAVE_PLUGIN, ScriviTrailSavePluginClass))

/*
 * Main object structure
 */
typedef struct _ScriviTrailSavePlugin		ScriviTrailSavePlugin;

struct _ScriviTrailSavePlugin
{
	ScriviPlugin parent_instance;
};

/*
 * Class definition
 */
typedef struct _ScriviTrailSavePluginClass	ScriviTrailSavePluginClass;

struct _ScriviTrailSavePluginClass
{
	ScriviPluginClass parent_class;
};

/*
 * Public methods
 */
GType	scrivi_trail_save_plugin_get_type	(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_scrivi_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __SCRIVI_TRAIL_SAVE_PLUGIN_H__ */
