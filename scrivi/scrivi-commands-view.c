/*
 * scrivi-view-commands.c
 * This file is part of scrivi
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
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
 * Modified by the scrivi Team, 1998-2005. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id$
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>

#include "scrivi-commands.h"
#include "scrivi-debug.h"
#include "scrivi-window.h"
#include "scrivi-window-private.h"


void
_scrivi_cmd_view_show_toolbar (GtkAction   *action,
			     ScriviWindow *window)
{
	gboolean visible;

	scrivi_debug (DEBUG_COMMANDS);

	visible = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action));

	if (visible)
		gtk_widget_show (window->priv->toolbar);
	else
		gtk_widget_hide (window->priv->toolbar);
}

void
_scrivi_cmd_view_show_statusbar (GtkAction   *action,
			       ScriviWindow *window)
{
	gboolean visible;

	scrivi_debug (DEBUG_COMMANDS);

	visible = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action));

	if (visible)
		gtk_widget_show (window->priv->statusbar);
	else
		gtk_widget_hide (window->priv->statusbar);
}

void
_scrivi_cmd_view_show_side_pane (GtkAction   *action,
			       ScriviWindow *window)
{
	gboolean visible;
	ScriviPanel *panel;

	scrivi_debug (DEBUG_COMMANDS);

	visible = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action));

	panel = scrivi_window_get_side_panel (window);

	if (visible)
	{
		gtk_widget_show (GTK_WIDGET (panel));
		gtk_widget_grab_focus (GTK_WIDGET (panel));
	}
	else
	{
		gtk_widget_hide (GTK_WIDGET (panel));
	}
}

void
_scrivi_cmd_view_show_bottom_pane (GtkAction   *action,
				 ScriviWindow *window)
{
	gboolean visible;
	ScriviPanel *panel;

	scrivi_debug (DEBUG_COMMANDS);

	visible = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action));

	panel = scrivi_window_get_bottom_panel (window);

	if (visible)
	{
		gtk_widget_show (GTK_WIDGET (panel));
		gtk_widget_grab_focus (GTK_WIDGET (panel));
	}
	else
	{
		gtk_widget_hide (GTK_WIDGET (panel));
	}
}

void
_scrivi_cmd_view_toggle_fullscreen_mode (GtkAction *action,
					ScriviWindow *window)
{
	scrivi_debug (DEBUG_COMMANDS);

	if (_scrivi_window_is_fullscreen (window))
		_scrivi_window_unfullscreen (window);
	else
		_scrivi_window_fullscreen (window);
}

void
_scrivi_cmd_view_leave_fullscreen_mode (GtkAction *action,
				       ScriviWindow *window)
{
	GtkAction *view_action;

	view_action = gtk_action_group_get_action (window->priv->always_sensitive_action_group,
						   "ViewFullscreen");
	g_signal_handlers_block_by_func
		(view_action, G_CALLBACK (_scrivi_cmd_view_toggle_fullscreen_mode),
		 window);
	gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (view_action),
				      FALSE);
	_scrivi_window_unfullscreen (window);
	g_signal_handlers_unblock_by_func
		(view_action, G_CALLBACK (_scrivi_cmd_view_toggle_fullscreen_mode),
		 window);
}
