/*
 * scrivi-plugin.h
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

#include "scrivi-plugin.h"
#include "scrivi-dirs.h"

/* properties */
enum {
	PROP_0,
	PROP_INSTALL_DIR,
	PROP_DATA_DIR_NAME,
	PROP_DATA_DIR
};

typedef struct _ScriviPluginPrivate ScriviPluginPrivate;

struct _ScriviPluginPrivate
{
	gchar *install_dir;
	gchar *data_dir_name;
};

#define SCRIVI_PLUGIN_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), SCRIVI_TYPE_PLUGIN, ScriviPluginPrivate))

G_DEFINE_TYPE(ScriviPlugin, scrivi_plugin, G_TYPE_OBJECT)

static void
dummy (ScriviPlugin *plugin, ScriviWindow *window)
{
	/* Empty */
}

static GtkWidget *
create_configure_dialog	(ScriviPlugin *plugin)
{
	return NULL;
}

static gboolean
is_configurable (ScriviPlugin *plugin)
{
	return (SCRIVI_PLUGIN_GET_CLASS (plugin)->create_configure_dialog !=
		create_configure_dialog);
}

static void
scrivi_plugin_get_property (GObject    *object,
			   guint       prop_id,
			   GValue     *value,
			   GParamSpec *pspec)
{
	switch (prop_id)
	{
		case PROP_INSTALL_DIR:
			g_value_take_string (value, scrivi_plugin_get_install_dir (SCRIVI_PLUGIN (object)));
			break;
		case PROP_DATA_DIR:
			g_value_take_string (value, scrivi_plugin_get_data_dir (SCRIVI_PLUGIN (object)));
			break;
		default:
			g_return_if_reached ();
	}
}

static void
scrivi_plugin_set_property (GObject      *object,
			   guint         prop_id,
			   const GValue *value,
			   GParamSpec   *pspec)
{
	ScriviPluginPrivate *priv = SCRIVI_PLUGIN_GET_PRIVATE (object);

	switch (prop_id)
	{
		case PROP_INSTALL_DIR:
			priv->install_dir = g_value_dup_string (value);
			break;
		case PROP_DATA_DIR_NAME:
			priv->data_dir_name = g_value_dup_string (value);
			break;
		default:
			g_return_if_reached ();
	}
}

static void
scrivi_plugin_finalize (GObject *object)
{
	ScriviPluginPrivate *priv = SCRIVI_PLUGIN_GET_PRIVATE (object);

	g_free (priv->install_dir);
	g_free (priv->data_dir_name);

	G_OBJECT_CLASS (scrivi_plugin_parent_class)->finalize (object);
}

static void
scrivi_plugin_class_init (ScriviPluginClass *klass)
{
    	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	klass->activate = dummy;
	klass->deactivate = dummy;
	klass->update_ui = dummy;

	klass->create_configure_dialog = create_configure_dialog;
	klass->is_configurable = is_configurable;

	object_class->get_property = scrivi_plugin_get_property;
	object_class->set_property = scrivi_plugin_set_property;
	object_class->finalize = scrivi_plugin_finalize;

	g_object_class_install_property (object_class,
					 PROP_INSTALL_DIR,
					 g_param_spec_string ("install-dir",
							      "Install Directory",
							      "The directory where the plugin is installed",
							      NULL,
							      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	/* the basename of the data dir is set at construction time by the plugin loader
	 * while the full path is constructed on the fly to take into account relocability
	 * that's why we have a writeonly prop and a readonly prop */
	g_object_class_install_property (object_class,
					 PROP_DATA_DIR_NAME,
					 g_param_spec_string ("data-dir-name",
							      "Basename of the data directory",
							      "The basename of the directory where the plugin should look for its data files",
							      NULL,
							      G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (object_class,
					 PROP_DATA_DIR,
					 g_param_spec_string ("data-dir",
							      "Data Directory",
							      "The full path of the directory where the plugin should look for its data files",
							      NULL,
							      G_PARAM_READABLE));

	g_type_class_add_private (klass, sizeof (ScriviPluginPrivate));
}

static void
scrivi_plugin_init (ScriviPlugin *plugin)
{
	/* Empty */
}

/**
 * scrivi_plugin_get_install_dir:
 * @plugin: a #ScriviPlugin
 *
 * Get the path of the directory where the plugin is installed.
 *
 * Return value: a newly allocated string with the path of the
 * directory where the plugin is installed
 */
gchar *
scrivi_plugin_get_install_dir (ScriviPlugin *plugin)
{
	g_return_val_if_fail (SCRIVI_IS_PLUGIN (plugin), NULL);

	return g_strdup (SCRIVI_PLUGIN_GET_PRIVATE (plugin)->install_dir);
}

/**
 * scrivi_plugin_get_data_dir:
 * @plugin: a #ScriviPlugin
 *
 * Get the path of the directory where the plugin should look for
 * its data files.
 *
 * Return value: a newly allocated string with the path of the
 * directory where the plugin should look for its data files
 */
gchar *
scrivi_plugin_get_data_dir (ScriviPlugin *plugin)
{
	ScriviPluginPrivate *priv;
	gchar *scrivi_lib_dir;
	gchar *data_dir;

	g_return_val_if_fail (SCRIVI_IS_PLUGIN (plugin), NULL);

	priv = SCRIVI_PLUGIN_GET_PRIVATE (plugin);

	/* If it's a "user" plugin the data dir is
	 * install_dir/data_dir_name if instead it's a
	 * "system" plugin the data dir is under scrivi_data_dir,
	 * so it's under $prefix/share/scrivi/plugins/data_dir_name
	 * where data_dir_name usually it's the name of the plugin
	 */
	scrivi_lib_dir = scrivi_dirs_get_scrivi_lib_dir ();

	/* CHECK: is checking the prefix enough or should we be more
	 * careful about normalizing paths etc? */
	if (g_str_has_prefix (priv->install_dir, scrivi_lib_dir))
	{
		gchar *scrivi_data_dir;

		scrivi_data_dir = scrivi_dirs_get_scrivi_data_dir ();

		data_dir = g_build_filename (scrivi_data_dir,
					     "plugins",
					     priv->data_dir_name,
					     NULL);

		g_free (scrivi_data_dir);
	}
	else
	{
		data_dir = g_build_filename (priv->install_dir,
					     priv->data_dir_name,
					     NULL);
	}

	g_free (scrivi_lib_dir);

	return data_dir;
}

/**
 * scrivi_plugin_activate:
 * @plugin: a #ScriviPlugin
 * @window: a #ScriviWindow
 *
 * Activates the plugin.
 */
void
scrivi_plugin_activate (ScriviPlugin *plugin,
		       ScriviWindow *window)
{
	g_return_if_fail (SCRIVI_IS_PLUGIN (plugin));
	g_return_if_fail (SCRIVI_IS_WINDOW (window));

	SCRIVI_PLUGIN_GET_CLASS (plugin)->activate (plugin, window);
}

/**
 * scrivi_plugin_deactivate:
 * @plugin: a #ScriviPlugin
 * @window: a #ScriviWindow
 *
 * Deactivates the plugin.
 */
void
scrivi_plugin_deactivate	(ScriviPlugin *plugin,
			 ScriviWindow *window)
{
	g_return_if_fail (SCRIVI_IS_PLUGIN (plugin));
	g_return_if_fail (SCRIVI_IS_WINDOW (window));

	SCRIVI_PLUGIN_GET_CLASS (plugin)->deactivate (plugin, window);
}

/**
 * scrivi_plugin_update_ui:
 * @plugin: a #ScriviPlugin
 * @window: a #ScriviWindow
 *
 * Triggers an update of the user interface to take into account state changes
 * caused by the plugin.
 */
void
scrivi_plugin_update_ui	(ScriviPlugin *plugin,
			 ScriviWindow *window)
{
	g_return_if_fail (SCRIVI_IS_PLUGIN (plugin));
	g_return_if_fail (SCRIVI_IS_WINDOW (window));

	SCRIVI_PLUGIN_GET_CLASS (plugin)->update_ui (plugin, window);
}

/**
 * scrivi_plugin_is_configurable:
 * @plugin: a #ScriviPlugin
 *
 * Whether the plugin is configurable.
 *
 * Returns: TRUE if the plugin is configurable:
 */
gboolean
scrivi_plugin_is_configurable (ScriviPlugin *plugin)
{
	g_return_val_if_fail (SCRIVI_IS_PLUGIN (plugin), FALSE);

	return SCRIVI_PLUGIN_GET_CLASS (plugin)->is_configurable (plugin);
}

/**
 * scrivi_plugin_create_configure_dialog:
 * @plugin: a #ScriviPlugin
 *
 * Creates the configure dialog widget for the plugin.
 *
 * Returns: the configure dialog widget for the plugin.
 */
GtkWidget *
scrivi_plugin_create_configure_dialog (ScriviPlugin *plugin)
{
	g_return_val_if_fail (SCRIVI_IS_PLUGIN (plugin), NULL);

	return SCRIVI_PLUGIN_GET_CLASS (plugin)->create_configure_dialog (plugin);
}
