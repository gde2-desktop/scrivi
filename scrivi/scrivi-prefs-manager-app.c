/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * scrivi-prefs-manager.c
 * This file is part of scrivi
 *
 * Copyright (C) 2002-2005  Paolo Maggi
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
 * Modified by the scrivi Team, 2002-2003. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id$
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <gio/gio.h>
#include "scrivi-prefs-manager.h"
#include "scrivi-prefs-manager-private.h"
#include "scrivi-prefs-manager-app.h"
#include "scrivi-app.h"
#include "scrivi-debug.h"
#include "scrivi-view.h"
#include "scrivi-window.h"
#include "scrivi-window-private.h"
#include "scrivi-plugins-engine.h"
#include "scrivi-style-scheme-manager.h"
#include "scrivi-dirs.h"

static void scrivi_prefs_manager_editor_font_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_system_font_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_tabs_size_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_wrap_mode_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_line_numbers_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_auto_indent_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_undo_changed		(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_right_margin_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_smart_home_end_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_hl_current_line_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_bracket_matching_changed(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_syntax_hl_enable_changed(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_search_hl_enable_changed(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_source_style_scheme_changed(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_max_recents_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_auto_save_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_active_plugins_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

static void scrivi_prefs_manager_lockdown_changed	(GSettings *settings,
							 gchar       *key,
							 gpointer     user_data);

/* GUI state is serialized to a .desktop file, not in GSettings */

#define SCRIVI_STATE_DEFAULT_WINDOW_STATE	0
#define SCRIVI_STATE_DEFAULT_WINDOW_WIDTH	650
#define SCRIVI_STATE_DEFAULT_WINDOW_HEIGHT	500
#define SCRIVI_STATE_DEFAULT_SIDE_PANEL_SIZE	200
#define SCRIVI_STATE_DEFAULT_BOTTOM_PANEL_SIZE	140

#define SCRIVI_STATE_FILE_LOCATION "scrivi.ini"

#define SCRIVI_STATE_WINDOW_GROUP "window"
#define SCRIVI_STATE_WINDOW_STATE "state"
#define SCRIVI_STATE_WINDOW_HEIGHT "height"
#define SCRIVI_STATE_WINDOW_WIDTH "width"
#define SCRIVI_STATE_SIDE_PANEL_SIZE "side_panel_size"
#define SCRIVI_STATE_BOTTOM_PANEL_SIZE "bottom_panel_size"
#define SCRIVI_STATE_SIDE_PANEL_ACTIVE_PAGE "side_panel_active_page"
#define SCRIVI_STATE_BOTTOM_PANEL_ACTIVE_PAGE "bottom_panel_active_page"

#define SCRIVI_STATE_FILEFILTER_GROUP "filefilter"
#define SCRIVI_STATE_FILEFILTER_ID "id"

static gint window_state = -1;
static gint window_height = -1;
static gint window_width = -1;
static gint side_panel_size = -1;
static gint bottom_panel_size = -1;
static gint side_panel_active_page = -1;
static gint bottom_panel_active_page = -1;
static gint active_file_filter = -1;


static gchar *
get_state_filename (void)
{
	gchar *config_dir;
	gchar *filename = NULL;

	config_dir = scrivi_dirs_get_user_config_dir ();

	if (config_dir != NULL)
	{
		filename = g_build_filename (config_dir,
					     SCRIVI_STATE_FILE_LOCATION,
					     NULL);
		g_free (config_dir);
	}

	return filename;
}

static GKeyFile *
get_scrivi_state_file (void)
{
	static GKeyFile *state_file = NULL;

	if (state_file == NULL)
	{
		gchar *filename;
		GError *err = NULL;

		state_file = g_key_file_new ();

		filename = get_state_filename ();

		if (!g_key_file_load_from_file (state_file,
						filename,
						G_KEY_FILE_NONE,
						&err))
		{
			if (err->domain != G_FILE_ERROR ||
			    err->code != G_FILE_ERROR_NOENT)
			{
				g_warning ("Could not load scrivi state file: %s\n",
					   err->message);
			}

			g_error_free (err);
		}

		g_free (filename);
	}

	return state_file;
}

static void
scrivi_state_get_int (const gchar *group,
		     const gchar *key,
		     gint         defval,
		     gint        *result)
{
	GKeyFile *state_file;
	gint res;
	GError *err = NULL;

	state_file = get_scrivi_state_file ();
	res = g_key_file_get_integer (state_file,
				      group,
				      key,
				      &err);

	if (err != NULL)
	{
		if ((err->domain != G_KEY_FILE_ERROR) ||
		    ((err->code != G_KEY_FILE_ERROR_GROUP_NOT_FOUND &&
		      err->code != G_KEY_FILE_ERROR_KEY_NOT_FOUND)))
		{
			g_warning ("Could not get state value %s::%s : %s\n",
				   group,
				   key,
				   err->message);
		}

		*result = defval;
		g_error_free (err);
	}
	else
	{
		*result = res;
	}
}

static void
scrivi_state_set_int (const gchar *group,
		     const gchar *key,
		     gint         value)
{
	GKeyFile *state_file;

	state_file = get_scrivi_state_file ();
	g_key_file_set_integer (state_file,
				group,
				key,
				value);
}

static gboolean
scrivi_state_file_sync (void)
{
	GKeyFile *state_file;
	gchar *config_dir;
	gchar *filename = NULL;
	gchar *content = NULL;
	gsize length;
	gint res;
	GError *err = NULL;
	gboolean ret = FALSE;

	state_file = get_scrivi_state_file ();
	g_return_val_if_fail (state_file != NULL, FALSE);

	config_dir = scrivi_dirs_get_user_config_dir ();
	if (config_dir == NULL)
	{
		g_warning ("Could not get config directory\n");
		return ret;
	}

	res = g_mkdir_with_parents (config_dir, 0755);
	if (res < 0)
	{
		g_warning ("Could not create config directory\n");
		goto out;
	}

	content = g_key_file_to_data (state_file,
				      &length,
				      &err);

	if (err != NULL)
	{
		g_warning ("Could not get data from state file: %s\n",
			   err->message);
		goto out;
	}

	if (content != NULL)
	{
		filename = get_state_filename ();
		if (!g_file_set_contents (filename,
					  content,
					  length,
					  &err))
		{
			g_warning ("Could not write scrivi state file: %s\n",
				   err->message);
			goto out;
		}
	}

	ret = TRUE;

 out:
	if (err != NULL)
		g_error_free (err);

	g_free (config_dir);
	g_free (filename);
	g_free (content);

	return ret;
}

/* Window state */
gint
scrivi_prefs_manager_get_window_state (void)
{
	if (window_state == -1)
	{
		scrivi_state_get_int (SCRIVI_STATE_WINDOW_GROUP,
				     SCRIVI_STATE_WINDOW_STATE,
				     SCRIVI_STATE_DEFAULT_WINDOW_STATE,
				     &window_state);
	}

	return window_state;
}

void
scrivi_prefs_manager_set_window_state (gint ws)
{
	g_return_if_fail (ws > -1);

	window_state = ws;

	scrivi_state_set_int (SCRIVI_STATE_WINDOW_GROUP,
			     SCRIVI_STATE_WINDOW_STATE,
			     ws);
}

gboolean
scrivi_prefs_manager_window_state_can_set (void)
{
	return TRUE;
}

/* Window size */
void
scrivi_prefs_manager_get_window_size (gint *width, gint *height)
{
	g_return_if_fail (width != NULL && height != NULL);

	if (window_width == -1)
	{
		scrivi_state_get_int (SCRIVI_STATE_WINDOW_GROUP,
				     SCRIVI_STATE_WINDOW_WIDTH,
				     SCRIVI_STATE_DEFAULT_WINDOW_WIDTH,
				     &window_width);
	}

	if (window_height == -1)
	{
		scrivi_state_get_int (SCRIVI_STATE_WINDOW_GROUP,
				     SCRIVI_STATE_WINDOW_HEIGHT,
				     SCRIVI_STATE_DEFAULT_WINDOW_HEIGHT,
				     &window_height);
	}

	*width = window_width;
	*height = window_height;
}

void
scrivi_prefs_manager_get_default_window_size (gint *width, gint *height)
{
	g_return_if_fail (width != NULL && height != NULL);

	*width = SCRIVI_STATE_DEFAULT_WINDOW_WIDTH;
	*height = SCRIVI_STATE_DEFAULT_WINDOW_HEIGHT;
}

void
scrivi_prefs_manager_set_window_size (gint width, gint height)
{
	g_return_if_fail (width > -1 && height > -1);

	window_width = width;
	window_height = height;

	scrivi_state_set_int (SCRIVI_STATE_WINDOW_GROUP,
			     SCRIVI_STATE_WINDOW_WIDTH,
			     width);
	scrivi_state_set_int (SCRIVI_STATE_WINDOW_GROUP,
			     SCRIVI_STATE_WINDOW_HEIGHT,
			     height);
}

gboolean
scrivi_prefs_manager_window_size_can_set (void)
{
	return TRUE;
}

/* Side panel */
gint
scrivi_prefs_manager_get_side_panel_size (void)
{
	if (side_panel_size == -1)
	{
		scrivi_state_get_int (SCRIVI_STATE_WINDOW_GROUP,
				     SCRIVI_STATE_SIDE_PANEL_SIZE,
				     SCRIVI_STATE_DEFAULT_SIDE_PANEL_SIZE,
				     &side_panel_size);
	}

	return side_panel_size;
}

gint
scrivi_prefs_manager_get_default_side_panel_size (void)
{
	return SCRIVI_STATE_DEFAULT_SIDE_PANEL_SIZE;
}

void
scrivi_prefs_manager_set_side_panel_size (gint ps)
{
	g_return_if_fail (ps > -1);

	if (side_panel_size == ps)
		return;

	side_panel_size = ps;
	scrivi_state_set_int (SCRIVI_STATE_WINDOW_GROUP,
			     SCRIVI_STATE_SIDE_PANEL_SIZE,
			     ps);
}

gboolean
scrivi_prefs_manager_side_panel_size_can_set (void)
{
	return TRUE;
}

gint
scrivi_prefs_manager_get_side_panel_active_page (void)
{
	if (side_panel_active_page == -1)
	{
		scrivi_state_get_int (SCRIVI_STATE_WINDOW_GROUP,
				     SCRIVI_STATE_SIDE_PANEL_ACTIVE_PAGE,
				     0,
				     &side_panel_active_page);
	}

	return side_panel_active_page;
}

void
scrivi_prefs_manager_set_side_panel_active_page (gint id)
{
	if (side_panel_active_page == id)
		return;

	side_panel_active_page = id;
	scrivi_state_set_int (SCRIVI_STATE_WINDOW_GROUP,
			     SCRIVI_STATE_SIDE_PANEL_ACTIVE_PAGE,
			     id);
}

gboolean
scrivi_prefs_manager_side_panel_active_page_can_set (void)
{
	return TRUE;
}

/* Bottom panel */
gint
scrivi_prefs_manager_get_bottom_panel_size (void)
{
	if (bottom_panel_size == -1)
	{
		scrivi_state_get_int (SCRIVI_STATE_WINDOW_GROUP,
				     SCRIVI_STATE_BOTTOM_PANEL_SIZE,
				     SCRIVI_STATE_DEFAULT_BOTTOM_PANEL_SIZE,
				     &bottom_panel_size);
	}

	return bottom_panel_size;
}

gint
scrivi_prefs_manager_get_default_bottom_panel_size (void)
{
	return SCRIVI_STATE_DEFAULT_BOTTOM_PANEL_SIZE;
}

void
scrivi_prefs_manager_set_bottom_panel_size (gint ps)
{
	g_return_if_fail (ps > -1);

	if (bottom_panel_size == ps)
		return;

	bottom_panel_size = ps;
	scrivi_state_set_int (SCRIVI_STATE_WINDOW_GROUP,
			     SCRIVI_STATE_BOTTOM_PANEL_SIZE,
			     ps);
}

gboolean
scrivi_prefs_manager_bottom_panel_size_can_set (void)
{
	return TRUE;
}

gint
scrivi_prefs_manager_get_bottom_panel_active_page (void)
{
	if (bottom_panel_active_page == -1)
	{
		scrivi_state_get_int (SCRIVI_STATE_WINDOW_GROUP,
				     SCRIVI_STATE_BOTTOM_PANEL_ACTIVE_PAGE,
				     0,
				     &bottom_panel_active_page);
	}

	return bottom_panel_active_page;
}

void
scrivi_prefs_manager_set_bottom_panel_active_page (gint id)
{
	if (bottom_panel_active_page == id)
		return;

	bottom_panel_active_page = id;
	scrivi_state_set_int (SCRIVI_STATE_WINDOW_GROUP,
			     SCRIVI_STATE_BOTTOM_PANEL_ACTIVE_PAGE,
			     id);
}

gboolean
scrivi_prefs_manager_bottom_panel_active_page_can_set (void)
{
	return TRUE;
}

/* File filter */
gint
scrivi_prefs_manager_get_active_file_filter (void)
{
	if (active_file_filter == -1)
	{
		scrivi_state_get_int (SCRIVI_STATE_FILEFILTER_GROUP,
				     SCRIVI_STATE_FILEFILTER_ID,
				     0,
				     &active_file_filter);
	}

	return active_file_filter;
}

void
scrivi_prefs_manager_set_active_file_filter (gint id)
{
	g_return_if_fail (id >= 0);

	if (active_file_filter == id)
		return;

	active_file_filter = id;
	scrivi_state_set_int (SCRIVI_STATE_FILEFILTER_GROUP,
			     SCRIVI_STATE_FILEFILTER_ID,
			     id);
}

gboolean
scrivi_prefs_manager_active_file_filter_can_set (void)
{
	return TRUE;
}

/* Normal prefs are stored in GSettings */

gboolean
scrivi_prefs_manager_app_init (void)
{
	scrivi_debug (DEBUG_PREFS);

	g_return_val_if_fail (scrivi_prefs_manager == NULL, FALSE);

	scrivi_prefs_manager_init ();

	if (scrivi_prefs_manager != NULL)
	{
		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_USE_DEFAULT_FONT,
				G_CALLBACK (scrivi_prefs_manager_editor_font_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_EDITOR_FONT,
				G_CALLBACK (scrivi_prefs_manager_editor_font_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->interface_settings,
				"changed::" GPM_SYSTEM_FONT,
				G_CALLBACK (scrivi_prefs_manager_system_font_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_TABS_SIZE,
				G_CALLBACK (scrivi_prefs_manager_tabs_size_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_INSERT_SPACES,
				G_CALLBACK (scrivi_prefs_manager_tabs_size_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_WRAP_MODE,
				G_CALLBACK (scrivi_prefs_manager_wrap_mode_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_DISPLAY_LINE_NUMBERS,
				G_CALLBACK (scrivi_prefs_manager_line_numbers_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_AUTO_INDENT,
				G_CALLBACK (scrivi_prefs_manager_auto_indent_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_UNDO_ACTIONS_LIMIT,
				G_CALLBACK (scrivi_prefs_manager_undo_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_DISPLAY_RIGHT_MARGIN,
				G_CALLBACK (scrivi_prefs_manager_right_margin_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_RIGHT_MARGIN_POSITION,
				G_CALLBACK (scrivi_prefs_manager_right_margin_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_SMART_HOME_END,
				G_CALLBACK (scrivi_prefs_manager_smart_home_end_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_HIGHLIGHT_CURRENT_LINE,
				G_CALLBACK (scrivi_prefs_manager_hl_current_line_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_BRACKET_MATCHING,
				G_CALLBACK (scrivi_prefs_manager_bracket_matching_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_SYNTAX_HL_ENABLE,
				G_CALLBACK (scrivi_prefs_manager_syntax_hl_enable_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_SEARCH_HIGHLIGHTING_ENABLE,
				G_CALLBACK (scrivi_prefs_manager_search_hl_enable_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_SOURCE_STYLE_SCHEME,
				G_CALLBACK (scrivi_prefs_manager_source_style_scheme_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_MAX_RECENTS,
				G_CALLBACK (scrivi_prefs_manager_max_recents_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_CREATE_BACKUP_COPY,
				G_CALLBACK (scrivi_prefs_manager_auto_save_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_AUTO_SAVE_INTERVAL,
				G_CALLBACK (scrivi_prefs_manager_auto_save_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_WRITABLE_VFS_SCHEMES,
				G_CALLBACK (scrivi_prefs_manager_auto_save_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->settings,
				"changed::" GPM_ACTIVE_PLUGINS,
				G_CALLBACK (scrivi_prefs_manager_active_plugins_changed),
				NULL);

		g_signal_connect (scrivi_prefs_manager->lockdown_settings,
				"changed",
				G_CALLBACK (scrivi_prefs_manager_lockdown_changed),
				NULL);
	}

	return scrivi_prefs_manager != NULL;
}

/* This function must be called before exiting scrivi */
void
scrivi_prefs_manager_app_shutdown (void)
{
	scrivi_debug (DEBUG_PREFS);

	scrivi_prefs_manager_shutdown ();

	scrivi_state_file_sync ();
}


static void
scrivi_prefs_manager_editor_font_changed (GSettings *settings,
					 gchar       *key,
					 gpointer     user_data)
{
	GList *views;
	GList *l;
	gchar *font = NULL;
	gboolean def = TRUE;
	gint ts;

	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_USE_DEFAULT_FONT) == 0)
	{
		def = g_settings_get_boolean (settings, key);

		if (def)
			font = scrivi_prefs_manager_get_system_font ();
		else
			font = scrivi_prefs_manager_get_editor_font ();
	}
	else if (strcmp (key, GPM_EDITOR_FONT) == 0)
	{
		font = g_settings_get_string (settings, key);

		def = scrivi_prefs_manager_get_use_default_font ();
	}
	else
		return;

	g_return_if_fail (font != NULL);

	ts = scrivi_prefs_manager_get_tabs_size ();

	views = scrivi_app_get_views (scrivi_app_get_default ());
	l = views;

	while (l != NULL)
	{
		/* Note: we use def=FALSE to avoid ScriviView to query GSettings */
		scrivi_view_set_font (SCRIVI_VIEW (l->data), FALSE,  font);
		gtk_source_view_set_tab_width (GTK_SOURCE_VIEW (l->data), ts);

		l = l->next;
	}

	g_list_free (views);
	g_free (font);
}

static void
scrivi_prefs_manager_system_font_changed (GSettings *settings,
					 gchar       *key,
					 gpointer     user_data)
{
	GList *views;
	GList *l;
	gchar *font;
	gint ts;

	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_SYSTEM_FONT) != 0)
		return;

	if (!scrivi_prefs_manager_get_use_default_font ())
		return;

	font = g_settings_get_string (settings, key);

	ts = scrivi_prefs_manager_get_tabs_size ();

	views = scrivi_app_get_views (scrivi_app_get_default ());
	l = views;

	while (l != NULL)
	{
		/* Note: we use def=FALSE to avoid ScriviView to query GSettings */
		scrivi_view_set_font (SCRIVI_VIEW (l->data), FALSE, font);

		gtk_source_view_set_tab_width (GTK_SOURCE_VIEW (l->data), ts);
		l = l->next;
	}

	g_list_free (views);
	g_free (font);
}

static void
scrivi_prefs_manager_tabs_size_changed (GSettings *settings,
				       gchar       *key,
				       gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_TABS_SIZE) == 0)
	{
		gint tab_width;
		GList *views;
		GList *l;

		tab_width = g_settings_get_int (settings, key);

		tab_width = CLAMP (tab_width, 1, 24);

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_source_view_set_tab_width (GTK_SOURCE_VIEW (l->data),
						       tab_width);

			l = l->next;
		}

		g_list_free (views);
	}
	else if (strcmp (key, GPM_INSERT_SPACES) == 0)
	{
		gboolean enable;
		GList *views;
		GList *l;

		enable = g_settings_get_boolean (settings, key);

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_source_view_set_insert_spaces_instead_of_tabs (
					GTK_SOURCE_VIEW (l->data),
					enable);

			l = l->next;
		}

		g_list_free (views);
	}
}

static GtkWrapMode
get_wrap_mode_from_string (const gchar* str)
{
	GtkWrapMode res;

	g_return_val_if_fail (str != NULL, GTK_WRAP_WORD);

	if (strcmp (str, "GTK_WRAP_NONE") == 0)
		res = GTK_WRAP_NONE;
	else
	{
		if (strcmp (str, "GTK_WRAP_CHAR") == 0)
			res = GTK_WRAP_CHAR;
		else
			res = GTK_WRAP_WORD;
	}

	return res;
}

static void
scrivi_prefs_manager_wrap_mode_changed (GSettings *settings,
	                               gchar         *key,
	                               gpointer       user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_WRAP_MODE) == 0)
	{
		GtkWrapMode wrap_mode;
		GList *views;
		GList *l;

		wrap_mode = get_wrap_mode_from_string (g_settings_get_string(settings, key));

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (l->data),
						     wrap_mode);

			l = l->next;
		}

		g_list_free (views);
	}
}

static void
scrivi_prefs_manager_line_numbers_changed (GSettings *settings,
					  gchar       *key,
					  gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_DISPLAY_LINE_NUMBERS) == 0)
	{
		gboolean dln;
		GList *views;
		GList *l;

		dln = g_settings_get_boolean (settings, key);

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_source_view_set_show_line_numbers (GTK_SOURCE_VIEW (l->data),
							       dln);

			l = l->next;
		}

		g_list_free (views);
	}
}

static void
scrivi_prefs_manager_hl_current_line_changed (GSettings *settings,
					     gchar       *key,
					     gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_HIGHLIGHT_CURRENT_LINE) == 0)
	{
		gboolean hl;
		GList *views;
		GList *l;

		hl = g_settings_get_boolean (settings, key);

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_source_view_set_highlight_current_line (GTK_SOURCE_VIEW (l->data),
								    hl);

			l = l->next;
		}

		g_list_free (views);
	}
}

static void
scrivi_prefs_manager_bracket_matching_changed (GSettings *settings,
					      gchar       *key,
					      gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_BRACKET_MATCHING) == 0)
	{
		gboolean enable;
		GList *docs;
		GList *l;

		enable = g_settings_get_boolean (settings, key);

		docs = scrivi_app_get_documents (scrivi_app_get_default ());
		l = docs;

		while (l != NULL)
		{
			gtk_source_buffer_set_highlight_matching_brackets (GTK_SOURCE_BUFFER (l->data),
									   enable);

			l = l->next;
		}

		g_list_free (docs);
	}
}

static void
scrivi_prefs_manager_auto_indent_changed (GSettings *settings,
					 gchar       *key,
					 gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_AUTO_INDENT) == 0)
	{
		gboolean enable;
		GList *views;
		GList *l;

		enable = g_settings_get_boolean (settings, key);

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_source_view_set_auto_indent (GTK_SOURCE_VIEW (l->data),
							 enable);

			l = l->next;
		}

		g_list_free (views);
	}
}

static void
scrivi_prefs_manager_undo_changed (GSettings *settings,
				  gchar       *key,
				  gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_UNDO_ACTIONS_LIMIT) == 0)
	{
		gint ul;
		GList *docs;
		GList *l;

		ul = g_settings_get_int (settings, key);

		ul = CLAMP (ul, -1, 250);

		docs = scrivi_app_get_documents (scrivi_app_get_default ());
		l = docs;

		while (l != NULL)
		{
			gtk_source_buffer_set_max_undo_levels (GTK_SOURCE_BUFFER (l->data),
							       ul);

			l = l->next;
		}

		g_list_free (docs);
	}
}

static void
scrivi_prefs_manager_right_margin_changed (GSettings *settings,
					  gchar *key,
					  gpointer user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_RIGHT_MARGIN_POSITION) == 0)
	{
		gint pos;
		GList *views;
		GList *l;

		pos = g_settings_get_int (settings, key);

		pos = CLAMP (pos, 1, 160);

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_source_view_set_right_margin_position (GTK_SOURCE_VIEW (l->data),
								   pos);

			l = l->next;
		}

		g_list_free (views);
	}
	else if (strcmp (key, GPM_DISPLAY_RIGHT_MARGIN) == 0)
	{
		gboolean display;
		GList *views;
		GList *l;

		display = g_settings_get_boolean (settings, key);

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_source_view_set_show_right_margin (GTK_SOURCE_VIEW (l->data),
							       display);

			l = l->next;
		}

		g_list_free (views);
	}
}

static GtkSourceSmartHomeEndType
get_smart_home_end_from_string (const gchar *str)
{
	GtkSourceSmartHomeEndType res;

	g_return_val_if_fail (str != NULL, GTK_SOURCE_SMART_HOME_END_AFTER);

	if (strcmp (str, "DISABLED") == 0)
		res = GTK_SOURCE_SMART_HOME_END_DISABLED;
	else if (strcmp (str, "BEFORE") == 0)
		res = GTK_SOURCE_SMART_HOME_END_BEFORE;
	else if (strcmp (str, "ALWAYS") == 0)
		res = GTK_SOURCE_SMART_HOME_END_ALWAYS;
	else
		res = GTK_SOURCE_SMART_HOME_END_AFTER;

	return res;
}

static void
scrivi_prefs_manager_smart_home_end_changed (GSettings *settings,
					    gchar       *key,
					    gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_SMART_HOME_END) == 0)
	{
		GtkSourceSmartHomeEndType smart_he;
		GList *views;
		GList *l;

		smart_he = get_smart_home_end_from_string (g_settings_get_string (settings, key));

		views = scrivi_app_get_views (scrivi_app_get_default ());
		l = views;

		while (l != NULL)
		{
			gtk_source_view_set_smart_home_end (GTK_SOURCE_VIEW (l->data),
							    smart_he);

			l = l->next;
		}

		g_list_free (views);
	}
}

static void
scrivi_prefs_manager_syntax_hl_enable_changed (GSettings *settings,
					      gchar       *key,
					      gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_SYNTAX_HL_ENABLE) == 0)
	{
		gboolean enable;
		GList *docs;
		GList *l;
		const GList *windows;

		enable = g_settings_get_boolean (settings, key);

		docs = scrivi_app_get_documents (scrivi_app_get_default ());
		l = docs;

		while (l != NULL)
		{
#if GTK_CHECK_VERSION (3, 0, 0)
			g_return_if_fail (GTK_SOURCE_IS_BUFFER (l->data));
#else
			g_return_if_fail (GTK_IS_SOURCE_BUFFER (l->data));
#endif

			gtk_source_buffer_set_highlight_syntax (GTK_SOURCE_BUFFER (l->data),
								enable);

			l = l->next;
		}

		g_list_free (docs);

		/* update the sensitivity of the Higlight Mode menu item */
		windows = scrivi_app_get_windows (scrivi_app_get_default ());
		while (windows != NULL)
		{
			GtkUIManager *ui;
			GtkAction *a;

			ui = scrivi_window_get_ui_manager (SCRIVI_WINDOW (windows->data));

			a = gtk_ui_manager_get_action (ui,
						       "/MenuBar/ViewMenu/ViewHighlightModeMenu");

			gtk_action_set_sensitive (a, enable);

			windows = g_list_next (windows);
		}
	}
}

static void
scrivi_prefs_manager_search_hl_enable_changed (GSettings *settings,
					      gchar       *key,
					      gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_SEARCH_HIGHLIGHTING_ENABLE) == 0)
	{
		gboolean enable;
		GList *docs;
		GList *l;

		enable = g_settings_get_boolean (settings, key);

		docs = scrivi_app_get_documents (scrivi_app_get_default ());
		l = docs;

		while (l != NULL)
		{
			g_return_if_fail (SCRIVI_IS_DOCUMENT (l->data));

			scrivi_document_set_enable_search_highlighting  (SCRIVI_DOCUMENT (l->data),
									enable);

			l = l->next;
		}

		g_list_free (docs);
	}
}

static void
scrivi_prefs_manager_source_style_scheme_changed (GSettings *settings,
						 gchar       *key,
						 gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_SOURCE_STYLE_SCHEME) == 0)
	{
		static gchar *old_scheme = NULL;
		gchar *scheme;
		GtkSourceStyleScheme *style;
		GList *docs;
		GList *l;

		scheme = g_settings_get_string (settings, key);

		if (old_scheme != NULL && (strcmp (scheme, old_scheme) == 0))
		    	return;

		g_free (old_scheme);
		old_scheme = scheme;

		style = gtk_source_style_scheme_manager_get_scheme (
				scrivi_get_style_scheme_manager (),
				scheme);

		if (style == NULL)
		{
			g_warning ("Default style scheme '%s' not found, falling back to 'classic'", scheme);

			style = gtk_source_style_scheme_manager_get_scheme (
				scrivi_get_style_scheme_manager (),
				"classic");

			if (style == NULL)
			{
				g_warning ("Style scheme 'classic' cannot be found, check your GtkSourceView installation.");
				return;
			}
		}

		docs = scrivi_app_get_documents (scrivi_app_get_default ());
		for (l = docs; l != NULL; l = l->next)
		{
#if GTK_CHECK_VERSION (3, 0, 0)
			g_return_if_fail (GTK_SOURCE_IS_BUFFER (l->data));
#else
			g_return_if_fail (GTK_IS_SOURCE_BUFFER (l->data));
#endif

			gtk_source_buffer_set_style_scheme (GTK_SOURCE_BUFFER (l->data),
							    style);
		}

		g_list_free (docs);
	}
}

static void
scrivi_prefs_manager_max_recents_changed (GSettings *settings,
					 gchar       *key,
					 gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_MAX_RECENTS) == 0)
	{
		const GList *windows;
		gint max;

		max = g_settings_get_int (settings, key);

		if (max < 0) {
			max = GPM_DEFAULT_MAX_RECENTS;
		}

		windows = scrivi_app_get_windows (scrivi_app_get_default ());
		while (windows != NULL)
		{
			ScriviWindow *w = windows->data;

			gtk_recent_chooser_set_limit (GTK_RECENT_CHOOSER (w->priv->toolbar_recent_menu),
						      max);

			windows = g_list_next (windows);
		}

		/* FIXME: we have no way at the moment to trigger the
		 * update of the inline recents in the File menu */
	}
}

static void
scrivi_prefs_manager_auto_save_changed (GSettings *settings,
				       gchar       *key,
				       gpointer     user_data)
{
	GList *docs;
	GList *l;

	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_AUTO_SAVE) == 0)
	{
		gboolean auto_save;

		auto_save = g_settings_get_boolean (settings, key);

		docs = scrivi_app_get_documents (scrivi_app_get_default ());
		l = docs;

		while (l != NULL)
		{
			ScriviDocument *doc = SCRIVI_DOCUMENT (l->data);
			ScriviTab *tab = scrivi_tab_get_from_document (doc);

			scrivi_tab_set_auto_save_enabled (tab, auto_save);

			l = l->next;
		}

		g_list_free (docs);
	}
	else if (strcmp (key,  GPM_AUTO_SAVE_INTERVAL) == 0)
	{
		gint auto_save_interval;

		auto_save_interval = g_settings_get_int (settings, key);

		if (auto_save_interval <= 0)
			auto_save_interval = GPM_DEFAULT_AUTO_SAVE_INTERVAL;

		docs = scrivi_app_get_documents (scrivi_app_get_default ());
		l = docs;

		while (l != NULL)
		{
			ScriviDocument *doc = SCRIVI_DOCUMENT (l->data);
			ScriviTab *tab = scrivi_tab_get_from_document (doc);

			scrivi_tab_set_auto_save_interval (tab, auto_save_interval);

			l = l->next;
		}

		g_list_free (docs);
	}
}

static void
scrivi_prefs_manager_active_plugins_changed (GSettings *settings,
					    gchar       *key,
					    gpointer     user_data)
{
	scrivi_debug (DEBUG_PREFS);

	if (strcmp (key, GPM_ACTIVE_PLUGINS) == 0)
	{
		ScriviPluginsEngine *engine;

		engine = scrivi_plugins_engine_get_default ();

		scrivi_plugins_engine_active_plugins_changed (engine);
	}
}

static void
scrivi_prefs_manager_lockdown_changed (GSettings *settings,
				      gchar       *key,
				      gpointer     user_data)
{
	ScriviApp *app;
	gboolean locked;

	scrivi_debug (DEBUG_PREFS);

	locked = g_settings_get_boolean (settings, key);

	app = scrivi_app_get_default ();

	if (strcmp (key, GPM_LOCKDOWN_COMMAND_LINE) == 0)
		_scrivi_app_set_lockdown_bit (app,
					     SCRIVI_LOCKDOWN_COMMAND_LINE,
					     locked);

	else if (strcmp (key, GPM_LOCKDOWN_PRINTING) == 0)
		_scrivi_app_set_lockdown_bit (app,
					     SCRIVI_LOCKDOWN_PRINTING,
					     locked);

	else if (strcmp (key, GPM_LOCKDOWN_PRINT_SETUP) == 0)
		_scrivi_app_set_lockdown_bit (app,
					     SCRIVI_LOCKDOWN_PRINT_SETUP,
					     locked);

	else if (strcmp (key, GPM_LOCKDOWN_SAVE_TO_DISK) == 0)
		_scrivi_app_set_lockdown_bit (app,
					     SCRIVI_LOCKDOWN_SAVE_TO_DISK,
					     locked);
}
