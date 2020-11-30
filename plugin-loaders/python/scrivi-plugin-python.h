/*
 * scrivi-plugin-python.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Raphael Slinckx
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

#ifndef __SCRIVI_PLUGIN_PYTHON_H__
#define __SCRIVI_PLUGIN_PYTHON_H__

#define NO_IMPORT_PYGOBJECT

#include <glib-object.h>
#include <pygobject.h>

#include <scrivi/scrivi-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_PLUGIN_PYTHON		(scrivi_plugin_python_get_type())
#define SCRIVI_PLUGIN_PYTHON(obj)		(G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_PLUGIN_PYTHON, ScriviPluginPython))
#define SCRIVI_PLUGIN_PYTHON_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_PLUGIN_PYTHON, ScriviPluginPythonClass))
#define SCRIVI_IS_PLUGIN_PYTHON(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_PLUGIN_PYTHON))
#define SCRIVI_IS_PLUGIN_PYTHON_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PLUGIN_PYTHON))
#define SCRIVI_PLUGIN_PYTHON_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_PLUGIN_PYTHON, ScriviPluginPythonClass))

/* Private structure type */
typedef struct _ScriviPluginPythonPrivate ScriviPluginPythonPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviPluginPython ScriviPluginPython;

struct _ScriviPluginPython 
{
	ScriviPlugin parent;
	
	/*< private > */
	ScriviPluginPythonPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviPluginPythonClass ScriviPluginPythonClass;

struct _ScriviPluginPythonClass 
{
	ScriviPluginClass parent_class;
};

/*
 * Public methods
 */
GType	 scrivi_plugin_python_get_type 		(void) G_GNUC_CONST;


/* 
 * Private methods
 */
void	  _scrivi_plugin_python_set_instance	(ScriviPluginPython *plugin, 
						 PyObject 	   *instance);
PyObject *_scrivi_plugin_python_get_instance	(ScriviPluginPython *plugin);

G_END_DECLS

#endif  /* __SCRIVI_PLUGIN_PYTHON_H__ */

