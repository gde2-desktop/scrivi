/*
 * scrivi-commands-file-print.c
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

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "scrivi-commands.h"
#include "scrivi-window.h"
#include "scrivi-tab.h"
#include "scrivi-debug.h"

void
_scrivi_cmd_file_print_preview (GtkAction   *action,
			       ScriviWindow *window)
{
	ScriviTab *tab;

	scrivi_debug (DEBUG_COMMANDS);

	tab = scrivi_window_get_active_tab (window);
	if (tab == NULL)
		return;

	_scrivi_tab_print_preview (tab);
}

void
_scrivi_cmd_file_print (GtkAction   *action,
		       ScriviWindow *window)
{
	ScriviTab *tab;

	scrivi_debug (DEBUG_COMMANDS);

	tab = scrivi_window_get_active_tab (window);
	if (tab == NULL)
		return;

	_scrivi_tab_print (tab);
}

