/*
 * scrivi-commands-edit.c
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
#include "scrivi-window.h"
#include "scrivi-debug.h"
#include "scrivi-view.h"
#include "dialogs/scrivi-preferences-dialog.h"

void
_scrivi_cmd_edit_undo (GtkAction   *action,
		     ScriviWindow *window)
{
	ScriviView *active_view;
	GtkSourceBuffer *active_document;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	g_return_if_fail (active_view);

	active_document = GTK_SOURCE_BUFFER (gtk_text_view_get_buffer (GTK_TEXT_VIEW (active_view)));

	gtk_source_buffer_undo (active_document);

	scrivi_view_scroll_to_cursor (active_view);

	gtk_widget_grab_focus (GTK_WIDGET (active_view));
}

void
_scrivi_cmd_edit_redo (GtkAction   *action,
		     ScriviWindow *window)
{
	ScriviView *active_view;
	GtkSourceBuffer *active_document;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	g_return_if_fail (active_view);

	active_document = GTK_SOURCE_BUFFER (gtk_text_view_get_buffer (GTK_TEXT_VIEW (active_view)));

	gtk_source_buffer_redo (active_document);

	scrivi_view_scroll_to_cursor (active_view);

	gtk_widget_grab_focus (GTK_WIDGET (active_view));
}

void
_scrivi_cmd_edit_cut (GtkAction   *action,
		    ScriviWindow *window)
{
	ScriviView *active_view;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	g_return_if_fail (active_view);

	scrivi_view_cut_clipboard (active_view);

	gtk_widget_grab_focus (GTK_WIDGET (active_view));
}

void
_scrivi_cmd_edit_copy (GtkAction   *action,
		     ScriviWindow *window)
{
	ScriviView *active_view;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	g_return_if_fail (active_view);

	scrivi_view_copy_clipboard (active_view);

	gtk_widget_grab_focus (GTK_WIDGET (active_view));
}

void
_scrivi_cmd_edit_paste (GtkAction   *action,
		      ScriviWindow *window)
{
	ScriviView *active_view;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	g_return_if_fail (active_view);

	scrivi_view_paste_clipboard (active_view);

	gtk_widget_grab_focus (GTK_WIDGET (active_view));
}

void
_scrivi_cmd_edit_delete (GtkAction   *action,
		       ScriviWindow *window)
{
	ScriviView *active_view;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	g_return_if_fail (active_view);

	scrivi_view_delete_selection (active_view);

	gtk_widget_grab_focus (GTK_WIDGET (active_view));
}

void
_scrivi_cmd_edit_select_all (GtkAction   *action,
			   ScriviWindow *window)
{
	ScriviView *active_view;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	g_return_if_fail (active_view);

	scrivi_view_select_all (active_view);

	gtk_widget_grab_focus (GTK_WIDGET (active_view));
}

void
_scrivi_cmd_edit_preferences (GtkAction   *action,
			    ScriviWindow *window)
{
	scrivi_debug (DEBUG_COMMANDS);

	scrivi_show_preferences_dialog (window);
}
