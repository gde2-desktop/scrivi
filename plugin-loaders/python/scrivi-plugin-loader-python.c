/*
 * scrivi-plugin-loader-python.c
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

#include "scrivi-plugin-loader-python.h"
#include "scrivi-plugin-python.h"
#include <scrivi/scrivi-object-module.h>

#define NO_IMPORT_PYGOBJECT
#define NO_IMPORT_PYGTK

#include <Python.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <signal.h>
#include "config.h"

#if PY_VERSION_HEX < 0x02050000
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
#endif

#define SCRIVI_PLUGIN_LOADER_PYTHON_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SCRIVI_TYPE_PLUGIN_LOADER_PYTHON, ScriviPluginLoaderPythonPrivate))

struct _ScriviPluginLoaderPythonPrivate
{
	GHashTable *loaded_plugins;
	guint idle_gc;
	gboolean init_failed;
};

typedef struct
{
	PyObject *type;
	PyObject *instance;
	gchar    *path;
} PythonInfo;

static void scrivi_plugin_loader_iface_init (gpointer g_iface, gpointer iface_data);

/* Exported by pyscrivi module */
void pyscrivi_register_classes (PyObject *d);
void pyscrivi_add_constants (PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pyscrivi_functions[];

/* Exported by pyscriviutils module */
void pyscriviutils_register_classes (PyObject *d);
extern PyMethodDef pyscriviutils_functions[];

/* Exported by pyscrivicommands module */
void pyscrivicommands_register_classes (PyObject *d);
extern PyMethodDef pyscrivicommands_functions[];

/* We retreive this to check for correct class hierarchy */
static PyTypeObject *PyScriviPlugin_Type;

SCRIVI_PLUGIN_LOADER_REGISTER_TYPE (ScriviPluginLoaderPython, scrivi_plugin_loader_python, G_TYPE_OBJECT, scrivi_plugin_loader_iface_init);


static PyObject *
find_python_plugin_type (ScriviPluginInfo *info,
			 PyObject        *pymodule)
{
	PyObject *locals, *key, *value;
	Py_ssize_t pos = 0;
	
	locals = PyModule_GetDict (pymodule);

	while (PyDict_Next (locals, &pos, &key, &value))
	{
		if (!PyType_Check(value))
			continue;

		if (PyObject_IsSubclass (value, (PyObject*) PyScriviPlugin_Type))
			return value;
	}
	
	g_warning ("No ScriviPlugin derivative found in Python plugin '%s'",
		   scrivi_plugin_info_get_name (info));
	return NULL;
}

static ScriviPlugin *
new_plugin_from_info (ScriviPluginLoaderPython *loader,
		      ScriviPluginInfo         *info)
{
	PythonInfo *pyinfo;
	PyTypeObject *pytype;
	PyObject *pyobject;
	PyGObject *pygobject;
	ScriviPlugin *instance;
	PyObject *emptyarg;

	pyinfo = (PythonInfo *)g_hash_table_lookup (loader->priv->loaded_plugins, info);
	
	if (pyinfo == NULL)
		return NULL;
	
	pytype = (PyTypeObject *)pyinfo->type;
	
	if (pytype->tp_new == NULL)
		return NULL;

	emptyarg = PyTuple_New(0);
	pyobject = pytype->tp_new (pytype, emptyarg, NULL);
	Py_DECREF (emptyarg);
	
	if (pyobject == NULL)
	{
		g_error ("Could not create instance for %s.", scrivi_plugin_info_get_name (info));
		return NULL;
	}

	pygobject = (PyGObject *)pyobject;
	
	if (pygobject->obj != NULL)
	{
		Py_DECREF (pyobject);
		g_error ("Could not create instance for %s (GObject already initialized).", scrivi_plugin_info_get_name (info));
		return NULL;
	}
	
	pygobject_construct (pygobject,
			     "install-dir", pyinfo->path,
			     "data-dir-name", scrivi_plugin_info_get_module_name (info),
			     NULL);
	
	if (pygobject->obj == NULL)
	{
		g_error ("Could not create instance for %s (GObject not constructed).", scrivi_plugin_info_get_name (info));
		Py_DECREF (pyobject);

		return NULL;
	}

	/* now call tp_init manually */
	if (PyType_IsSubtype (pyobject->ob_type, pytype) && 
	    pyobject->ob_type->tp_init != NULL)
	{
		emptyarg = PyTuple_New(0);
		pyobject->ob_type->tp_init (pyobject, emptyarg, NULL);
		Py_DECREF (emptyarg);
	}

	instance = SCRIVI_PLUGIN (pygobject->obj);
	pyinfo->instance = (PyObject *)pygobject;

	/* make sure to register the python instance for the ScriviPluginPython
	   object to it can wrap the virtual scrivi plugin funcs back to python */
	_scrivi_plugin_python_set_instance (SCRIVI_PLUGIN_PYTHON (instance), (PyObject *)pygobject);
	
	/* we return a reference here because the other is owned by python */
	return SCRIVI_PLUGIN (g_object_ref (instance));
}

static ScriviPlugin *
add_python_info (ScriviPluginLoaderPython *loader,
		 ScriviPluginInfo         *info,
		 PyObject		 *module,
		 const gchar             *path,
		 PyObject                *type)
{
	PythonInfo *pyinfo;
	
	pyinfo = g_new (PythonInfo, 1);
	pyinfo->path = g_strdup (path);
	pyinfo->type = type;

	Py_INCREF (pyinfo->type);
	
	g_hash_table_insert (loader->priv->loaded_plugins, info, pyinfo);
	
	return new_plugin_from_info (loader, info);
}

static const gchar *
scrivi_plugin_loader_iface_get_id (void)
{
	return "Python";
}

static ScriviPlugin *
scrivi_plugin_loader_iface_load (ScriviPluginLoader *loader,
				ScriviPluginInfo   *info,
				const gchar       *path)
{
	ScriviPluginLoaderPython *pyloader = SCRIVI_PLUGIN_LOADER_PYTHON (loader);
	PyObject *main_module, *main_locals, *pytype;
	PyObject *pymodule, *fromlist;
	gchar *module_name;
	ScriviPlugin *result;
	
	if (pyloader->priv->init_failed)
	{
		g_warning ("Cannot load python plugin Python '%s' since scrivi was"
		           "not able to initialize the Python interpreter.",
		           scrivi_plugin_info_get_name (info));
		return NULL;
	}
	
	/* see if py definition for the plugin is already loaded */
	result = new_plugin_from_info (pyloader, info);
	
	if (result != NULL)
		return result;
	
	main_module = PyImport_AddModule ("scrivi.plugins");
	if (main_module == NULL)
	{
		g_warning ("Could not get scrivi.plugins.");
		return NULL;
	}
	
	/* If we have a special path, we register it */
	if (path != NULL)
	{
		PyObject *sys_path = PySys_GetObject ("path");
		PyObject *pypath = PyString_FromString (path);

		if (PySequence_Contains (sys_path, pypath) == 0)
			PyList_Insert (sys_path, 0, pypath);

		Py_DECREF (pypath);
	}
	
	main_locals = PyModule_GetDict (main_module);
	
	/* we need a fromlist to be able to import modules with a '.' in the
	   name. */
	fromlist = PyTuple_New(0);
	module_name = g_strdup (scrivi_plugin_info_get_module_name (info));
	
	pymodule = PyImport_ImportModuleEx (module_name, 
					    main_locals, 
					    main_locals, 
					    fromlist);
	
	Py_DECREF(fromlist);

	if (!pymodule)
	{
		g_free (module_name);
		PyErr_Print ();
		return NULL;
	}

	PyDict_SetItemString (main_locals, module_name, pymodule);
	g_free (module_name);
	
	pytype = find_python_plugin_type (info, pymodule);
	
	if (pytype)
		return add_python_info (pyloader, info, pymodule, path, pytype);

	return NULL;
}

static void
scrivi_plugin_loader_iface_unload (ScriviPluginLoader *loader,
				  ScriviPluginInfo   *info)
{
	ScriviPluginLoaderPython *pyloader = SCRIVI_PLUGIN_LOADER_PYTHON (loader);
	PythonInfo *pyinfo;
	PyGILState_STATE state;
	
	pyinfo = (PythonInfo *)g_hash_table_lookup (pyloader->priv->loaded_plugins, info);
	
	if (!pyinfo)
		return;
	
	state = pyg_gil_state_ensure ();
	Py_XDECREF (pyinfo->instance);
	pyg_gil_state_release (state);
	
	pyinfo->instance = NULL;
}

static gboolean
run_gc (ScriviPluginLoaderPython *loader)
{
	while (PyGC_Collect ())
		;

	loader->priv->idle_gc = 0;
	return FALSE;
}

static void
scrivi_plugin_loader_iface_garbage_collect (ScriviPluginLoader *loader)
{
	ScriviPluginLoaderPython *pyloader;
	
	if (!Py_IsInitialized())
		return;

	pyloader = SCRIVI_PLUGIN_LOADER_PYTHON (loader);

	/*
	 * We both run the GC right now and we schedule
	 * a further collection in the main loop.
	 */

	while (PyGC_Collect ())
		;

	if (pyloader->priv->idle_gc == 0)
		pyloader->priv->idle_gc = g_idle_add ((GSourceFunc)run_gc, pyloader);
}

static void
scrivi_plugin_loader_iface_init (gpointer g_iface, 
				gpointer iface_data)
{
	ScriviPluginLoaderInterface *iface = (ScriviPluginLoaderInterface *)g_iface;
	
	iface->get_id = scrivi_plugin_loader_iface_get_id;
	iface->load = scrivi_plugin_loader_iface_load;
	iface->unload = scrivi_plugin_loader_iface_unload;
	iface->garbage_collect = scrivi_plugin_loader_iface_garbage_collect;
}

static void
scrivi_python_shutdown (ScriviPluginLoaderPython *loader)
{
	if (!Py_IsInitialized ())
		return;

	if (loader->priv->idle_gc != 0)
	{
		g_source_remove (loader->priv->idle_gc);
		loader->priv->idle_gc = 0;
	}

	while (PyGC_Collect ())
		;	

	Py_Finalize ();
}


/* C equivalent of
 *    import pygtk
 *    pygtk.require ("2.0")
 */
static gboolean
scrivi_check_pygtk2 (void)
{
	PyObject *pygtk, *mdict, *require;

	/* pygtk.require("2.0") */
	pygtk = PyImport_ImportModule ("pygtk");
	if (pygtk == NULL)
	{
		g_warning ("Error initializing Python interpreter: could not import pygtk.");
		return FALSE;
	}

	mdict = PyModule_GetDict (pygtk);
	require = PyDict_GetItemString (mdict, "require");
	PyObject_CallObject (require,
			     Py_BuildValue ("(S)", PyString_FromString ("2.0")));
	if (PyErr_Occurred())
	{
		g_warning ("Error initializing Python interpreter: pygtk 2 is required.");
		return FALSE;
	}

	return TRUE;
}

/* Note: the following two functions are needed because
 * init_pyobject and init_pygtk which are *macros* which in case
 * case of error set the PyErr and then make the calling
 * function return behind our back.
 * It's up to the caller to check the result with PyErr_Occurred()
 */
static void
scrivi_init_pygobject (void)
{
	init_pygobject_check (2, 11, 5); /* FIXME: get from config */
}

static void
scrivi_init_pygtk (void)
{
	PyObject *gtk, *mdict, *version, *required_version;

	init_pygtk ();

	/* there isn't init_pygtk_check(), do the version
	 * check ourselves */
	gtk = PyImport_ImportModule("gtk");
	mdict = PyModule_GetDict(gtk);
	version = PyDict_GetItemString (mdict, "pygtk_version");
	if (!version)
	{
		PyErr_SetString (PyExc_ImportError,
				 "PyGObject version too old");
		return;
	}

	required_version = Py_BuildValue ("(iii)", 2, 4, 0); /* FIXME */

	if (PyObject_Compare (version, required_version) == -1)
	{
		PyErr_SetString (PyExc_ImportError,
				 "PyGObject version too old");
		Py_DECREF (required_version);
		return;
	}

	Py_DECREF (required_version);
}

static void
old_gtksourceview_init (void)
{
	PyErr_SetString(PyExc_ImportError,
			"gtksourceview module not allowed, use gtksourceview2");
}

static void
scrivi_init_pygtksourceview (void)
{
	PyObject *gtksourceview, *mdict, *version, *required_version;

	gtksourceview = PyImport_ImportModule("gtksourceview2");
	if (gtksourceview == NULL)
	{
		PyErr_SetString (PyExc_ImportError,
				 "could not import gtksourceview");
		return;
	}

	mdict = PyModule_GetDict (gtksourceview);
	version = PyDict_GetItemString (mdict, "pygtksourceview2_version");
	if (!version)
	{
		PyErr_SetString (PyExc_ImportError,
				 "PyGtkSourceView version too old");
		return;
	}

	required_version = Py_BuildValue ("(iii)", 0, 8, 0); /* FIXME */

	if (PyObject_Compare (version, required_version) == -1)
	{
		PyErr_SetString (PyExc_ImportError,
				 "PyGtkSourceView version too old");
		Py_DECREF (required_version);
		return;
	}

	Py_DECREF (required_version);

	/* Create a dummy 'gtksourceview' module to prevent
	 * loading of the old 'gtksourceview' modules that
	 * has conflicting symbols with the gtksourceview2 module.
	 * Raise an exception when trying to import it.
	 */
	PyImport_AppendInittab ("gtksourceview", old_gtksourceview_init);
}

static gboolean
scrivi_python_init (ScriviPluginLoaderPython *loader)
{
	PyObject *mdict, *tuple;
	PyObject *scrivi, *scriviutils, *scrivicommands, *scriviplugins;
	PyObject *gettext, *install, *gettext_args;
	//char *argv[] = { "scrivi", NULL };
	char *argv[] = { SCRIVI_PLUGINS_LIBS_DIR, NULL };
#ifdef HAVE_SIGACTION
	gint res;
	struct sigaction old_sigint;
#endif

	if (loader->priv->init_failed)
	{
		/* We already failed to initialized Python, don't need to
		 * retry again */
		return FALSE;
	}
	
	if (Py_IsInitialized ())
	{
		/* Python has already been successfully initialized */
		return TRUE;
	}

	/* We are trying to initialize Python for the first time,
	   set init_failed to FALSE only if the entire initialization process
	   ends with success */
	loader->priv->init_failed = TRUE;

	/* Hack to make python not overwrite SIGINT: this is needed to avoid
	 * the crash reported on bug #326191 */

	/* CHECK: can't we use Py_InitializeEx instead of Py_Initialize in order
          to avoid to manage signal handlers ? - Paolo (Dec. 31, 2006) */

#ifdef HAVE_SIGACTION
	/* Save old handler */
	res = sigaction (SIGINT, NULL, &old_sigint);  
	if (res != 0)
	{
		g_warning ("Error initializing Python interpreter: cannot get "
		           "handler to SIGINT signal (%s)",
		           g_strerror (errno));

		return FALSE;
	}
#endif

	/* Python initialization */
	Py_Initialize ();

#ifdef HAVE_SIGACTION
	/* Restore old handler */
	res = sigaction (SIGINT, &old_sigint, NULL);
	if (res != 0)
	{
		g_warning ("Error initializing Python interpreter: cannot restore "
		           "handler to SIGINT signal (%s).",
		           g_strerror (errno));

		goto python_init_error;
	}
#endif

	PySys_SetArgv (1, argv);

	if (!scrivi_check_pygtk2 ())
	{
		/* Warning message already printed in check_pygtk2 */
		goto python_init_error;
	}

	/* import gobject */	
	scrivi_init_pygobject ();
	if (PyErr_Occurred ())
	{
		g_warning ("Error initializing Python interpreter: could not import pygobject.");

		goto python_init_error;		
	}

	/* import gtk */
	scrivi_init_pygtk ();
	if (PyErr_Occurred ())
	{
		g_warning ("Error initializing Python interpreter: could not import pygtk.");

		goto python_init_error;
	}
	
	/* import gtksourceview */
	scrivi_init_pygtksourceview ();
	if (PyErr_Occurred ())
	{
		PyErr_Print ();

		g_warning ("Error initializing Python interpreter: could not import pygtksourceview.");

		goto python_init_error;
	}	
	
	/* import scrivi */
	scrivi = Py_InitModule ("scrivi", pyscrivi_functions);
	mdict = PyModule_GetDict (scrivi);

	pyscrivi_register_classes (mdict);
	pyscrivi_add_constants (scrivi, "SCRIVI_");

	/* scrivi version */
	tuple = Py_BuildValue("(iii)", 
			      SCRIVI_MAJOR_VERSION,
			      SCRIVI_MINOR_VERSION,
			      SCRIVI_MICRO_VERSION);
	PyDict_SetItemString(mdict, "version", tuple);
	Py_DECREF(tuple);
	
	/* Retrieve the Python type for scrivi.Plugin */
	PyScriviPlugin_Type = (PyTypeObject *) PyDict_GetItemString (mdict, "Plugin"); 
	if (PyScriviPlugin_Type == NULL)
	{
		PyErr_Print ();

		goto python_init_error;
	}

	/* import scrivi.utils */
	scriviutils = Py_InitModule ("scrivi.utils", pyscriviutils_functions);
	PyDict_SetItemString (mdict, "utils", scriviutils);

	/* import scrivi.commands */
	scrivicommands = Py_InitModule ("scrivi.commands", pyscrivicommands_functions);
	PyDict_SetItemString (mdict, "commands", scrivicommands);

	/* initialize empty scrivi.plugins module */
	scriviplugins = Py_InitModule ("scrivi.plugins", NULL);
	PyDict_SetItemString (mdict, "plugins", scriviplugins);

	mdict = PyModule_GetDict (scriviutils);
	pyscriviutils_register_classes (mdict);
	
	mdict = PyModule_GetDict (scrivicommands);
	pyscrivicommands_register_classes (mdict);

	/* i18n support */
	gettext = PyImport_ImportModule ("gettext");
	if (gettext == NULL)
	{
		g_warning ("Error initializing Python interpreter: could not import gettext.");

		goto python_init_error;
	}

	mdict = PyModule_GetDict (gettext);
	install = PyDict_GetItemString (mdict, "install");
	gettext_args = Py_BuildValue ("ss", GETTEXT_PACKAGE, SCRIVI_LOCALEDIR);
	PyObject_CallObject (install, gettext_args);
	Py_DECREF (gettext_args);
	
	/* Python has been successfully initialized */
	loader->priv->init_failed = FALSE;
	
	return TRUE;
	
python_init_error:

	g_warning ("Please check the installation of all the Python related packages required "
	           "by scrivi and try again.");

	PyErr_Clear ();

	scrivi_python_shutdown (loader);

	return FALSE;
}

static void
scrivi_plugin_loader_python_finalize (GObject *object)
{
	ScriviPluginLoaderPython *pyloader = SCRIVI_PLUGIN_LOADER_PYTHON (object);
	
	g_hash_table_destroy (pyloader->priv->loaded_plugins);
	scrivi_python_shutdown (pyloader);

	G_OBJECT_CLASS (scrivi_plugin_loader_python_parent_class)->finalize (object);
}

static void
scrivi_plugin_loader_python_class_init (ScriviPluginLoaderPythonClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	
	object_class->finalize = scrivi_plugin_loader_python_finalize;

	g_type_class_add_private (object_class, sizeof (ScriviPluginLoaderPythonPrivate));
}

static void
scrivi_plugin_loader_python_class_finalize (ScriviPluginLoaderPythonClass *klass)
{
}

static void
destroy_python_info (PythonInfo *info)
{
	PyGILState_STATE state = pyg_gil_state_ensure ();
	Py_XDECREF (info->type);	
	pyg_gil_state_release (state);
	
	g_free (info->path);
	g_free (info);
}

static void
scrivi_plugin_loader_python_init (ScriviPluginLoaderPython *self)
{
	self->priv = SCRIVI_PLUGIN_LOADER_PYTHON_GET_PRIVATE (self);
	
	/* initialize python interpreter */
	scrivi_python_init (self);

	/* loaded_plugins maps ScriviPluginInfo to a PythonInfo */
	self->priv->loaded_plugins = g_hash_table_new_full (g_direct_hash,
						            g_direct_equal,
						            NULL,
						            (GDestroyNotify)destroy_python_info);
}

ScriviPluginLoaderPython *
scrivi_plugin_loader_python_new ()
{
	GObject *loader = g_object_new (SCRIVI_TYPE_PLUGIN_LOADER_PYTHON, NULL);

	return SCRIVI_PLUGIN_LOADER_PYTHON (loader);
}

