/*
 * scrivi-plugins-engine.h
 * This file is part of scrivi
 *
 * Copyright (C) 2002-2005 - Paolo Maggi 
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
 * Modified by the scrivi Team, 2002-2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id$
 */

#ifndef __SCRIVI_PLUGINS_ENGINE_H__
#define __SCRIVI_PLUGINS_ENGINE_H__

#include <glib.h>
#include "scrivi-window.h"
#include "scrivi-plugin-info.h"
#include "scrivi-plugin.h"

G_BEGIN_DECLS

#define SCRIVI_TYPE_PLUGINS_ENGINE              (scrivi_plugins_engine_get_type ())
#define SCRIVI_PLUGINS_ENGINE(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_PLUGINS_ENGINE, ScriviPluginsEngine))
#define SCRIVI_PLUGINS_ENGINE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_PLUGINS_ENGINE, ScriviPluginsEngineClass))
#define SCRIVI_IS_PLUGINS_ENGINE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_PLUGINS_ENGINE))
#define SCRIVI_IS_PLUGINS_ENGINE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PLUGINS_ENGINE))
#define SCRIVI_PLUGINS_ENGINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_PLUGINS_ENGINE, ScriviPluginsEngineClass))

typedef struct _ScriviPluginsEngine		ScriviPluginsEngine;
typedef struct _ScriviPluginsEnginePrivate	ScriviPluginsEnginePrivate;

struct _ScriviPluginsEngine
{
	GObject parent;
	ScriviPluginsEnginePrivate *priv;
};

typedef struct _ScriviPluginsEngineClass		ScriviPluginsEngineClass;

struct _ScriviPluginsEngineClass
{
	GObjectClass parent_class;

	void	 (* activate_plugin)		(ScriviPluginsEngine *engine,
						 ScriviPluginInfo    *info);

	void	 (* deactivate_plugin)		(ScriviPluginsEngine *engine,
						 ScriviPluginInfo    *info);
};

GType			 scrivi_plugins_engine_get_type		(void) G_GNUC_CONST;

ScriviPluginsEngine	*scrivi_plugins_engine_get_default	(void);

void		 scrivi_plugins_engine_garbage_collect	(ScriviPluginsEngine *engine);

const GList	*scrivi_plugins_engine_get_plugin_list 	(ScriviPluginsEngine *engine);

ScriviPluginInfo	*scrivi_plugins_engine_get_plugin_info	(ScriviPluginsEngine *engine,
							 const gchar        *name);

/* plugin load and unloading (overall, for all windows) */
gboolean 	 scrivi_plugins_engine_activate_plugin 	(ScriviPluginsEngine *engine,
							 ScriviPluginInfo    *info);
gboolean 	 scrivi_plugins_engine_deactivate_plugin	(ScriviPluginsEngine *engine,
							 ScriviPluginInfo    *info);

void	 	 scrivi_plugins_engine_configure_plugin	(ScriviPluginsEngine *engine,
							 ScriviPluginInfo    *info,
							 GtkWindow          *parent);

/* plugin activation/deactivation per window, private to ScriviWindow */
void 		 scrivi_plugins_engine_activate_plugins   (ScriviPluginsEngine *engine,
							  ScriviWindow        *window);
void 		 scrivi_plugins_engine_deactivate_plugins (ScriviPluginsEngine *engine,
							  ScriviWindow        *window);
void		 scrivi_plugins_engine_update_plugins_ui  (ScriviPluginsEngine *engine,
							  ScriviWindow        *window);

/* private for GSettings notification */
void		 scrivi_plugins_engine_active_plugins_changed
							(ScriviPluginsEngine *engine);

void		 scrivi_plugins_engine_rescan_plugins	(ScriviPluginsEngine *engine);

G_END_DECLS

#endif  /* __SCRIVI_PLUGINS_ENGINE_H__ */
