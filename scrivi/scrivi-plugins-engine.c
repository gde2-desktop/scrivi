/*
 * scrivi-plugins-engine.c
 * This file is part of scrivi
 *
 * Copyright (C) 2002-2005 Paolo Maggi
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <glib/gi18n.h>

#include "scrivi-plugins-engine.h"
#include "scrivi-plugin-info-priv.h"
#include "scrivi-plugin.h"
#include "scrivi-debug.h"
#include "scrivi-app.h"
#include "scrivi-prefs-manager.h"
#include "scrivi-plugin-loader.h"
#include "scrivi-object-module.h"
#include "scrivi-dirs.h"

#define SCRIVI_PLUGINS_ENGINE_BASE_KEY "/apps/scrivi/plugins"
#define SCRIVI_PLUGINS_ENGINE_KEY SCRIVI_PLUGINS_ENGINE_BASE_KEY "/active-plugins"

#define PLUGIN_EXT	".scrivi-plugin"
#define LOADER_EXT	G_MODULE_SUFFIX

typedef struct
{
	ScriviPluginLoader *loader;
	ScriviObjectModule *module;
} LoaderInfo;

/* Signals */
enum
{
	ACTIVATE_PLUGIN,
	DEACTIVATE_PLUGIN,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE(ScriviPluginsEngine, scrivi_plugins_engine, G_TYPE_OBJECT)

struct _ScriviPluginsEnginePrivate
{
	GList *plugin_list;
	GHashTable *loaders;

	gboolean activate_from_prefs;
};

ScriviPluginsEngine *default_engine = NULL;

static void	scrivi_plugins_engine_activate_plugin_real (ScriviPluginsEngine *engine,
							   ScriviPluginInfo    *info);
static void	scrivi_plugins_engine_deactivate_plugin_real (ScriviPluginsEngine *engine,
							     ScriviPluginInfo    *info);

typedef gboolean (*LoadDirCallback)(ScriviPluginsEngine *engine, const gchar *filename, gpointer userdata);

static gboolean
load_dir_real (ScriviPluginsEngine *engine,
	       const gchar        *dir,
	       const gchar        *suffix,
	       LoadDirCallback     callback,
	       gpointer            userdata)
{
	GError *error = NULL;
	GDir *d;
	const gchar *dirent;
	gboolean ret = TRUE;

	g_return_val_if_fail (dir != NULL, TRUE);

	scrivi_debug_message (DEBUG_PLUGINS, "DIR: %s", dir);

	d = g_dir_open (dir, 0, &error);
	if (!d)
	{
		g_warning ("%s", error->message);
		g_error_free (error);
		return TRUE;
	}

	while ((dirent = g_dir_read_name (d)))
	{
		gchar *filename;

		if (!g_str_has_suffix (dirent, suffix))
			continue;

		filename = g_build_filename (dir, dirent, NULL);

		ret = callback (engine, filename, userdata);

		g_free (filename);

		if (!ret)
			break;
	}

	g_dir_close (d);
	return ret;
}

static gboolean
load_plugin_info (ScriviPluginsEngine *engine,
		  const gchar        *filename,
		  gpointer            userdata)
{
	ScriviPluginInfo *info;

	info = _scrivi_plugin_info_new (filename);

	if (info == NULL)
		return TRUE;

	/* If a plugin with this name has already been loaded
	 * drop this one (user plugins override system plugins) */
	if (scrivi_plugins_engine_get_plugin_info (engine, scrivi_plugin_info_get_module_name (info)) != NULL)
	{
		scrivi_debug_message (DEBUG_PLUGINS, "Two or more plugins named '%s'. "
				     "Only the first will be considered.\n",
				     scrivi_plugin_info_get_module_name (info));

		_scrivi_plugin_info_unref (info);

		return TRUE;
	}

	engine->priv->plugin_list = g_list_prepend (engine->priv->plugin_list, info);

	scrivi_debug_message (DEBUG_PLUGINS, "Plugin %s loaded", info->name);
	return TRUE;
}

static void
load_all_plugins (ScriviPluginsEngine *engine)
{
	gchar *plugin_dir;
	const gchar *pdirs_env = NULL;

	/* load user plugins */
	plugin_dir = scrivi_dirs_get_user_plugins_dir ();
	if (g_file_test (plugin_dir, G_FILE_TEST_IS_DIR))
	{
		load_dir_real (engine,
			       plugin_dir,
			       PLUGIN_EXT,
			       load_plugin_info,
			       NULL);

	}
	g_free (plugin_dir);

	/* load system plugins */
	pdirs_env = g_getenv ("SCRIVI_PLUGINS_PATH");

	scrivi_debug_message (DEBUG_PLUGINS, "SCRIVI_PLUGINS_PATH=%s", pdirs_env);

	if (pdirs_env != NULL)
	{
		gchar **pdirs;
		gint i;

		pdirs = g_strsplit (pdirs_env, G_SEARCHPATH_SEPARATOR_S, 0);

		for (i = 0; pdirs[i] != NULL; i++)
		{
			if (!load_dir_real (engine,
					    pdirs[i],
					    PLUGIN_EXT,
					    load_plugin_info,
					    NULL))
			{
				break;
			}
		}

		g_strfreev (pdirs);
	}
	else
	{
		plugin_dir = scrivi_dirs_get_scrivi_plugins_dir ();

		load_dir_real (engine,
			       plugin_dir,
			       PLUGIN_EXT,
			       load_plugin_info,
			       NULL);

		g_free (plugin_dir);
	}
}

static guint
hash_lowercase (gconstpointer data)
{
	gchar *lowercase;
	guint ret;

	lowercase = g_ascii_strdown ((const gchar *)data, -1);
	ret = g_str_hash (lowercase);
	g_free (lowercase);

	return ret;
}

static gboolean
equal_lowercase (gconstpointer a, gconstpointer b)
{
	return g_ascii_strcasecmp ((const gchar *)a, (const gchar *)b) == 0;
}

static void
loader_destroy (LoaderInfo *info)
{
	if (!info)
		return;

	if (info->loader)
		g_object_unref (info->loader);

	g_free (info);
}

static void
add_loader (ScriviPluginsEngine *engine,
	    const gchar        *loader_id,
	    ScriviObjectModule  *module)
{
	LoaderInfo *info;

	info = g_new (LoaderInfo, 1);
	info->loader = NULL;
	info->module = module;

	g_hash_table_insert (engine->priv->loaders, g_strdup (loader_id), info);
}

static void
scrivi_plugins_engine_init (ScriviPluginsEngine *engine)
{
	scrivi_debug (DEBUG_PLUGINS);

	if (!g_module_supported ())
	{
		g_warning ("scrivi is not able to initialize the plugins engine.");
		return;
	}

	engine->priv = G_TYPE_INSTANCE_GET_PRIVATE (engine,
						    SCRIVI_TYPE_PLUGINS_ENGINE,
						    ScriviPluginsEnginePrivate);

	load_all_plugins (engine);

	/* make sure that the first reactivation will read active plugins
	   from the prefs */
	engine->priv->activate_from_prefs = TRUE;

	/* mapping from loadername -> loader object */
	engine->priv->loaders = g_hash_table_new_full (hash_lowercase,
						       equal_lowercase,
						       (GDestroyNotify)g_free,
						       (GDestroyNotify)loader_destroy);
}

static void
loader_garbage_collect (const char *id, LoaderInfo *info)
{
	if (info->loader)
		scrivi_plugin_loader_garbage_collect (info->loader);
}

void
scrivi_plugins_engine_garbage_collect (ScriviPluginsEngine *engine)
{
	g_hash_table_foreach (engine->priv->loaders,
			      (GHFunc) loader_garbage_collect,
			      NULL);
}

static void
scrivi_plugins_engine_finalize (GObject *object)
{
	ScriviPluginsEngine *engine = SCRIVI_PLUGINS_ENGINE (object);
	GList *item;

	scrivi_debug (DEBUG_PLUGINS);

	/* Firs deactivate all plugins */
	for (item = engine->priv->plugin_list; item; item = item->next)
	{
		ScriviPluginInfo *info = SCRIVI_PLUGIN_INFO (item->data);

		if (scrivi_plugin_info_is_active (info))
			scrivi_plugins_engine_deactivate_plugin_real (engine, info);
	}

	/* unref the loaders */
	g_hash_table_destroy (engine->priv->loaders);

	/* and finally free the infos */
	for (item = engine->priv->plugin_list; item; item = item->next)
	{
		ScriviPluginInfo *info = SCRIVI_PLUGIN_INFO (item->data);

		_scrivi_plugin_info_unref (info);
	}

	g_list_free (engine->priv->plugin_list);

	G_OBJECT_CLASS (scrivi_plugins_engine_parent_class)->finalize (object);
}

static void
scrivi_plugins_engine_class_init (ScriviPluginsEngineClass *klass)
{
	GType the_type = G_TYPE_FROM_CLASS (klass);
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = scrivi_plugins_engine_finalize;
	klass->activate_plugin = scrivi_plugins_engine_activate_plugin_real;
	klass->deactivate_plugin = scrivi_plugins_engine_deactivate_plugin_real;

	signals[ACTIVATE_PLUGIN] =
		g_signal_new ("activate-plugin",
			      the_type,
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (ScriviPluginsEngineClass, activate_plugin),
			      NULL, NULL,
			      g_cclosure_marshal_VOID__BOXED,
			      G_TYPE_NONE,
			      1,
			      SCRIVI_TYPE_PLUGIN_INFO | G_SIGNAL_TYPE_STATIC_SCOPE);

	signals[DEACTIVATE_PLUGIN] =
		g_signal_new ("deactivate-plugin",
			      the_type,
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (ScriviPluginsEngineClass, deactivate_plugin),
			      NULL, NULL,
			      g_cclosure_marshal_VOID__BOXED,
			      G_TYPE_NONE,
			      1,
			      SCRIVI_TYPE_PLUGIN_INFO | G_SIGNAL_TYPE_STATIC_SCOPE);

	g_type_class_add_private (klass, sizeof (ScriviPluginsEnginePrivate));
}

static gboolean
load_loader (ScriviPluginsEngine *engine,
	     const gchar        *filename,
	     gpointer		 data)
{
	ScriviObjectModule *module;
	gchar *base;
	gchar *path;
	const gchar *id;
	GType type;

	/* try to load in the module */
	path = g_path_get_dirname (filename);
	base = g_path_get_basename (filename);

	/* for now they are all resident */
	module = scrivi_object_module_new (base,
					  path,
					  "register_scrivi_plugin_loader",
					  TRUE);

	g_free (base);
	g_free (path);

	/* make sure to load the type definition */
	if (!g_type_module_use (G_TYPE_MODULE (module)))
	{
		g_object_unref (module);
		g_warning ("Plugin loader module `%s' could not be loaded", filename);

		return TRUE;
	}

	/* get the exported type and check the name as exported by the
	 * loader interface */
	type = scrivi_object_module_get_object_type (module);
	id = scrivi_plugin_loader_type_get_id (type);

	add_loader (engine, id, module);
	g_type_module_unuse (G_TYPE_MODULE (module));

	return TRUE;
}

static void
ensure_loader (LoaderInfo *info)
{
	if (info->loader == NULL && info->module != NULL)
	{
		/* create a new loader object */
		ScriviPluginLoader *loader;
		loader = (ScriviPluginLoader *)scrivi_object_module_new_object (info->module, NULL);

		if (loader == NULL || !SCRIVI_IS_PLUGIN_LOADER (loader))
		{
			g_warning ("Loader object is not a valid ScriviPluginLoader instance");

			if (loader != NULL && G_IS_OBJECT (loader))
				g_object_unref (loader);
		}
		else
		{
			info->loader = loader;
		}
	}
}

static ScriviPluginLoader *
get_plugin_loader (ScriviPluginsEngine *engine, ScriviPluginInfo *info)
{
	const gchar *loader_id;
	LoaderInfo *loader_info;

	loader_id = info->loader;

	loader_info = (LoaderInfo *)g_hash_table_lookup (
			engine->priv->loaders,
			loader_id);

	if (loader_info == NULL)
	{
		gchar *loader_dir;

		loader_dir = scrivi_dirs_get_scrivi_plugin_loaders_dir ();

		/* loader could not be found in the hash, try to find it by
		   scanning */
		load_dir_real (engine,
			       loader_dir,
			       LOADER_EXT,
			       (LoadDirCallback)load_loader,
			       NULL);
		g_free (loader_dir);

		loader_info = (LoaderInfo *)g_hash_table_lookup (
				engine->priv->loaders,
				loader_id);
	}

	if (loader_info == NULL)
	{
		/* cache non-existent so we don't scan again */
		add_loader (engine, loader_id, NULL);
		return NULL;
	}

	ensure_loader (loader_info);
	return loader_info->loader;
}

ScriviPluginsEngine *
scrivi_plugins_engine_get_default (void)
{
	if (default_engine != NULL)
		return default_engine;

	default_engine = SCRIVI_PLUGINS_ENGINE (g_object_new (SCRIVI_TYPE_PLUGINS_ENGINE, NULL));
	g_object_add_weak_pointer (G_OBJECT (default_engine),
				   (gpointer) &default_engine);
	return default_engine;
}

const GList *
scrivi_plugins_engine_get_plugin_list (ScriviPluginsEngine *engine)
{
	scrivi_debug (DEBUG_PLUGINS);

	return engine->priv->plugin_list;
}

static gint
compare_plugin_info_and_name (ScriviPluginInfo *info,
			      const gchar *module_name)
{
	return strcmp (scrivi_plugin_info_get_module_name (info), module_name);
}

ScriviPluginInfo *
scrivi_plugins_engine_get_plugin_info (ScriviPluginsEngine *engine,
				      const gchar        *name)
{
	GList *l = g_list_find_custom (engine->priv->plugin_list,
				       name,
				       (GCompareFunc) compare_plugin_info_and_name);

	return l == NULL ? NULL : (ScriviPluginInfo *) l->data;
}

static void
save_active_plugin_list (ScriviPluginsEngine *engine)
{
	GSList *active_plugins = NULL;
	GList *l;

	for (l = engine->priv->plugin_list; l != NULL; l = l->next)
	{
		ScriviPluginInfo *info = (ScriviPluginInfo *) l->data;

		if (scrivi_plugin_info_is_active (info))
		{
			active_plugins = g_slist_prepend (active_plugins,
							  (gpointer)scrivi_plugin_info_get_module_name (info));
		}
	}

	scrivi_prefs_manager_set_active_plugins (active_plugins);

	g_slist_free (active_plugins);
}

static gboolean
load_plugin (ScriviPluginsEngine *engine,
	     ScriviPluginInfo    *info)
{
	ScriviPluginLoader *loader;
	gchar *path;

	if (scrivi_plugin_info_is_active (info))
		return TRUE;

	if (!scrivi_plugin_info_is_available (info))
		return FALSE;

	loader = get_plugin_loader (engine, info);

	if (loader == NULL)
	{
		g_warning ("Could not find loader `%s' for plugin `%s'", info->loader, info->name);
		info->available = FALSE;
		return FALSE;
	}

	path = g_path_get_dirname (info->file);
	g_return_val_if_fail (path != NULL, FALSE);

	info->plugin = scrivi_plugin_loader_load (loader, info, path);

	g_free (path);

	if (info->plugin == NULL)
	{
		g_warning ("Error loading plugin '%s'", info->name);
		info->available = FALSE;
		return FALSE;
	}

	return TRUE;
}

static void
scrivi_plugins_engine_activate_plugin_real (ScriviPluginsEngine *engine,
					   ScriviPluginInfo *info)
{
	const GList *wins;

	if (!load_plugin (engine, info))
		return;

	for (wins = scrivi_app_get_windows (scrivi_app_get_default ());
	     wins != NULL;
	     wins = wins->next)
	{
		scrivi_plugin_activate (info->plugin, SCRIVI_WINDOW (wins->data));
	}
}

gboolean
scrivi_plugins_engine_activate_plugin (ScriviPluginsEngine *engine,
				      ScriviPluginInfo    *info)
{
	scrivi_debug (DEBUG_PLUGINS);

	g_return_val_if_fail (info != NULL, FALSE);

	if (!scrivi_plugin_info_is_available (info))
		return FALSE;

	if (scrivi_plugin_info_is_active (info))
		return TRUE;

	g_signal_emit (engine, signals[ACTIVATE_PLUGIN], 0, info);

	if (scrivi_plugin_info_is_active (info))
		save_active_plugin_list (engine);

	return scrivi_plugin_info_is_active (info);
}

static void
call_plugin_deactivate (ScriviPlugin *plugin,
			ScriviWindow *window)
{
	scrivi_plugin_deactivate (plugin, window);

	/* ensure update of ui manager, because we suspect it does something
	   with expected static strings in the type module (when unloaded the
	   strings don't exist anymore, and ui manager updates in an idle
	   func) */
	gtk_ui_manager_ensure_update (scrivi_window_get_ui_manager (window));
}

static void
scrivi_plugins_engine_deactivate_plugin_real (ScriviPluginsEngine *engine,
					     ScriviPluginInfo *info)
{
	const GList *wins;
	ScriviPluginLoader *loader;

	if (!scrivi_plugin_info_is_active (info) ||
	    !scrivi_plugin_info_is_available (info))
		return;

	for (wins = scrivi_app_get_windows (scrivi_app_get_default ());
	     wins != NULL;
	     wins = wins->next)
	{
		call_plugin_deactivate (info->plugin, SCRIVI_WINDOW (wins->data));
	}

	/* first unref the plugin (the loader still has one) */
	g_object_unref (info->plugin);

	/* find the loader and tell it to gc and unload the plugin */
	loader = get_plugin_loader (engine, info);

	scrivi_plugin_loader_garbage_collect (loader);
	scrivi_plugin_loader_unload (loader, info);

	info->plugin = NULL;
}

gboolean
scrivi_plugins_engine_deactivate_plugin (ScriviPluginsEngine *engine,
					ScriviPluginInfo    *info)
{
	scrivi_debug (DEBUG_PLUGINS);

	g_return_val_if_fail (info != NULL, FALSE);

	if (!scrivi_plugin_info_is_active (info))
		return TRUE;

	g_signal_emit (engine, signals[DEACTIVATE_PLUGIN], 0, info);
	if (!scrivi_plugin_info_is_active (info))
		save_active_plugin_list (engine);

	return !scrivi_plugin_info_is_active (info);
}

void
scrivi_plugins_engine_activate_plugins (ScriviPluginsEngine *engine,
					ScriviWindow        *window)
{
	GSList *active_plugins = NULL;
	GList *pl;

	scrivi_debug (DEBUG_PLUGINS);

	g_return_if_fail (SCRIVI_IS_PLUGINS_ENGINE (engine));
	g_return_if_fail (SCRIVI_IS_WINDOW (window));

	/* the first time, we get the 'active' plugins from GSettings */
	if (engine->priv->activate_from_prefs)
	{
		active_plugins = scrivi_prefs_manager_get_active_plugins ();
	}

	for (pl = engine->priv->plugin_list; pl; pl = pl->next)
	{
		ScriviPluginInfo *info = (ScriviPluginInfo*)pl->data;

		if (engine->priv->activate_from_prefs &&
		    g_slist_find_custom (active_plugins,
					 scrivi_plugin_info_get_module_name (info),
					 (GCompareFunc)strcmp) == NULL)
			continue;

		/* If plugin is not active, don't try to activate/load it */
		if (!engine->priv->activate_from_prefs &&
		    !scrivi_plugin_info_is_active (info))
			continue;

		if (load_plugin (engine, info))
			scrivi_plugin_activate (info->plugin,
					       window);
	}

	if (engine->priv->activate_from_prefs)
	{
		g_slist_foreach (active_plugins, (GFunc) g_free, NULL);
		g_slist_free (active_plugins);
		engine->priv->activate_from_prefs = FALSE;
	}

	scrivi_debug_message (DEBUG_PLUGINS, "End");

	/* also call update_ui after activation */
	scrivi_plugins_engine_update_plugins_ui (engine, window);
}

void
scrivi_plugins_engine_deactivate_plugins (ScriviPluginsEngine *engine,
					  ScriviWindow        *window)
{
	GList *pl;

	scrivi_debug (DEBUG_PLUGINS);

	g_return_if_fail (SCRIVI_IS_PLUGINS_ENGINE (engine));
	g_return_if_fail (SCRIVI_IS_WINDOW (window));

	for (pl = engine->priv->plugin_list; pl; pl = pl->next)
	{
		ScriviPluginInfo *info = (ScriviPluginInfo*)pl->data;

		/* check if the plugin is actually active */
		if (!scrivi_plugin_info_is_active (info))
			continue;

		/* call deactivate for the plugin for this window */
		scrivi_plugin_deactivate (info->plugin, window);
	}

	scrivi_debug_message (DEBUG_PLUGINS, "End");
}

void
scrivi_plugins_engine_update_plugins_ui (ScriviPluginsEngine *engine,
					 ScriviWindow        *window)
{
	GList *pl;

	scrivi_debug (DEBUG_PLUGINS);

	g_return_if_fail (SCRIVI_IS_PLUGINS_ENGINE (engine));
	g_return_if_fail (SCRIVI_IS_WINDOW (window));

	/* call update_ui for all active plugins */
	for (pl = engine->priv->plugin_list; pl; pl = pl->next)
	{
		ScriviPluginInfo *info = (ScriviPluginInfo*)pl->data;

		if (!scrivi_plugin_info_is_active (info))
			continue;

	       	scrivi_debug_message (DEBUG_PLUGINS, "Updating UI of %s", info->name);
		scrivi_plugin_update_ui (info->plugin, window);
	}
}

void
scrivi_plugins_engine_configure_plugin (ScriviPluginsEngine *engine,
				       ScriviPluginInfo    *info,
				       GtkWindow          *parent)
{
	GtkWidget *conf_dlg;

	GtkWindowGroup *wg;

	scrivi_debug (DEBUG_PLUGINS);

	g_return_if_fail (info != NULL);

	conf_dlg = scrivi_plugin_create_configure_dialog (info->plugin);
	g_return_if_fail (conf_dlg != NULL);
	gtk_window_set_transient_for (GTK_WINDOW (conf_dlg),
				      parent);

	wg = gtk_window_get_group (parent);
	if (wg == NULL)
	{
		wg = gtk_window_group_new ();
		gtk_window_group_add_window (wg, parent);
	}

	gtk_window_group_add_window (wg,
				     GTK_WINDOW (conf_dlg));

	gtk_window_set_modal (GTK_WINDOW (conf_dlg), TRUE);
	gtk_widget_show (conf_dlg);
}

void
scrivi_plugins_engine_active_plugins_changed (ScriviPluginsEngine *engine)
{
	gboolean to_activate;
	GSList *active_plugins;
	GList *pl;

	scrivi_debug (DEBUG_PLUGINS);

	active_plugins = scrivi_prefs_manager_get_active_plugins ();

	for (pl = engine->priv->plugin_list; pl; pl = pl->next)
	{
		ScriviPluginInfo *info = (ScriviPluginInfo*)pl->data;

		if (!scrivi_plugin_info_is_available (info))
			continue;

		to_activate = (g_slist_find_custom (active_plugins,
						    scrivi_plugin_info_get_module_name (info),
						    (GCompareFunc)strcmp) != NULL);

		if (!scrivi_plugin_info_is_active (info) && to_activate)
			g_signal_emit (engine, signals[ACTIVATE_PLUGIN], 0, info);
		else if (scrivi_plugin_info_is_active (info) && !to_activate)
			g_signal_emit (engine, signals[DEACTIVATE_PLUGIN], 0, info);
	}

	g_slist_foreach (active_plugins, (GFunc) g_free, NULL);
	g_slist_free (active_plugins);
}

void
scrivi_plugins_engine_rescan_plugins (ScriviPluginsEngine *engine)
{
	scrivi_debug (DEBUG_PLUGINS);

	load_all_plugins (engine);
}
