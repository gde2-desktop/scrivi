/*
 * scrivi-plugin-loader-c.h
 * This file is part of scrivi
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
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

#ifndef __SCRIVI_PLUGIN_LOADER_C_H__
#define __SCRIVI_PLUGIN_LOADER_C_H__

#include <scrivi/scrivi-plugin-loader.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_PLUGIN_LOADER_C		(scrivi_plugin_loader_c_get_type ())
#define SCRIVI_PLUGIN_LOADER_C(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_PLUGIN_LOADER_C, ScriviPluginLoaderC))
#define SCRIVI_PLUGIN_LOADER_C_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_PLUGIN_LOADER_C, ScriviPluginLoaderC const))
#define SCRIVI_PLUGIN_LOADER_C_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_PLUGIN_LOADER_C, ScriviPluginLoaderCClass))
#define SCRIVI_IS_PLUGIN_LOADER_C(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_PLUGIN_LOADER_C))
#define SCRIVI_IS_PLUGIN_LOADER_C_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PLUGIN_LOADER_C))
#define SCRIVI_PLUGIN_LOADER_C_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_PLUGIN_LOADER_C, ScriviPluginLoaderCClass))

typedef struct _ScriviPluginLoaderC		ScriviPluginLoaderC;
typedef struct _ScriviPluginLoaderCClass		ScriviPluginLoaderCClass;
typedef struct _ScriviPluginLoaderCPrivate	ScriviPluginLoaderCPrivate;

struct _ScriviPluginLoaderC {
	GObject parent;
	
	ScriviPluginLoaderCPrivate *priv;
};

struct _ScriviPluginLoaderCClass {
	GObjectClass parent_class;
};

GType scrivi_plugin_loader_c_get_type (void) G_GNUC_CONST;
ScriviPluginLoaderC *scrivi_plugin_loader_c_new(void);

/* All the loaders must implement this function */
G_MODULE_EXPORT GType register_scrivi_plugin_loader (GTypeModule * module);

G_END_DECLS

#endif /* __SCRIVI_PLUGIN_LOADER_C_H__ */
