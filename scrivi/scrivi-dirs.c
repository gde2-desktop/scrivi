/*
 * scrivi-dirs.c
 * This file is part of scrivi
 *
 * Copyright (C) 2008 Ignacio Casal Quinteiro
 * Copyright (C) 2011 Perberos
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "scrivi-dirs.h"

gchar* scrivi_dirs_get_user_config_dir(void)
{
	gchar* config_dir = NULL;

	config_dir = g_build_filename(g_get_user_config_dir(), "scrivi", NULL);

	return config_dir;
}

gchar* scrivi_dirs_get_user_cache_dir(void)
{
	const gchar* cache_dir;

	cache_dir = g_get_user_cache_dir();

	return g_build_filename(cache_dir, "scrivi", NULL);
}

gchar* scrivi_dirs_get_user_plugins_dir(void)
{
	gchar* plugin_dir;

	plugin_dir = g_build_filename(g_get_user_data_dir(), "scrivi", "plugins", NULL);

	return plugin_dir;
}

gchar* scrivi_dirs_get_user_accels_file(void)
{
	gchar* accels = NULL;
	gchar *config_dir = NULL;

	config_dir = scrivi_dirs_get_user_config_dir();
	accels = g_build_filename(config_dir, "accels", NULL);

	g_free(config_dir);

	return accels;
}

gchar* scrivi_dirs_get_scrivi_data_dir(void)
{
	return g_build_filename(DATADIR, "scrivi", NULL);
}

gchar* scrivi_dirs_get_scrivi_locale_dir(void)
{
	return g_build_filename(DATADIR, "locale", NULL);
}

gchar* scrivi_dirs_get_scrivi_lib_dir(void)
{
	return g_build_filename(LIBDIR, "scrivi", NULL);
}

gchar* scrivi_dirs_get_scrivi_plugins_dir(void)
{
	gchar* lib_dir;
	gchar* plugin_dir;

	lib_dir = scrivi_dirs_get_scrivi_lib_dir();

	plugin_dir = g_build_filename(lib_dir, "plugins", NULL);
	g_free(lib_dir);

	return plugin_dir;
}

gchar* scrivi_dirs_get_scrivi_plugin_loaders_dir(void)
{
	gchar* lib_dir;
	gchar* loader_dir;

	lib_dir = scrivi_dirs_get_scrivi_lib_dir();

	loader_dir = g_build_filename(lib_dir, "plugin-loaders", NULL);
	g_free(lib_dir);

	return loader_dir;
}

gchar* scrivi_dirs_get_ui_file(const gchar* file)
{
	gchar* datadir;
	gchar* ui_file;

	g_return_val_if_fail(file != NULL, NULL);

	datadir = scrivi_dirs_get_scrivi_data_dir();
	ui_file = g_build_filename(datadir, "ui", file, NULL);
	g_free(datadir);

	return ui_file;
}
