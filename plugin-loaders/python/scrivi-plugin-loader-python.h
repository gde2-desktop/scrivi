/*
 * scrivi-plugin-loader-python.h
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

#ifndef __SCRIVI_PLUGIN_LOADER_PYTHON_H__
#define __SCRIVI_PLUGIN_LOADER_PYTHON_H__

#include <scrivi/scrivi-plugin-loader.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_PLUGIN_LOADER_PYTHON		(scrivi_plugin_loader_python_get_type ())
#define SCRIVI_PLUGIN_LOADER_PYTHON(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_PLUGIN_LOADER_PYTHON, ScriviPluginLoaderPython))
#define SCRIVI_PLUGIN_LOADER_PYTHON_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_PLUGIN_LOADER_PYTHON, ScriviPluginLoaderPython const))
#define SCRIVI_PLUGIN_LOADER_PYTHON_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_PLUGIN_LOADER_PYTHON, ScriviPluginLoaderPythonClass))
#define SCRIVI_IS_PLUGIN_LOADER_PYTHON(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_PLUGIN_LOADER_PYTHON))
#define SCRIVI_IS_PLUGIN_LOADER_PYTHON_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PLUGIN_LOADER_PYTHON))
#define SCRIVI_PLUGIN_LOADER_PYTHON_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_PLUGIN_LOADER_PYTHON, ScriviPluginLoaderPythonClass))

typedef struct _ScriviPluginLoaderPython		ScriviPluginLoaderPython;
typedef struct _ScriviPluginLoaderPythonClass		ScriviPluginLoaderPythonClass;
typedef struct _ScriviPluginLoaderPythonPrivate	ScriviPluginLoaderPythonPrivate;

struct _ScriviPluginLoaderPython {
	GObject parent;
	
	ScriviPluginLoaderPythonPrivate *priv;
};

struct _ScriviPluginLoaderPythonClass {
	GObjectClass parent_class;
};

GType scrivi_plugin_loader_python_get_type (void) G_GNUC_CONST;
ScriviPluginLoaderPython *scrivi_plugin_loader_python_new(void);

/* All the loaders must implement this function */
G_MODULE_EXPORT GType register_scrivi_plugin_loader (GTypeModule * module);

G_END_DECLS

#endif /* __SCRIVI_PLUGIN_LOADER_PYTHON_H__ */

