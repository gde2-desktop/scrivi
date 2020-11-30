/*
 * scrivi-statusbar.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Borelli
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
 * Modified by the scrivi Team, 2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 */

#ifndef SCRIVI_STATUSBAR_H
#define SCRIVI_STATUSBAR_H

#include <gtk/gtk.h>
#include <scrivi/scrivi-window.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_STATUSBAR		(scrivi_statusbar_get_type ())
#define SCRIVI_STATUSBAR(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCRIVI_TYPE_STATUSBAR, ScriviStatusbar))
#define SCRIVI_STATUSBAR_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), SCRIVI_TYPE_STATUSBAR, ScriviStatusbarClass))
#define SCRIVI_IS_STATUSBAR(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCRIVI_TYPE_STATUSBAR))
#define SCRIVI_IS_STATUSBAR_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCRIVI_TYPE_STATUSBAR))
#define SCRIVI_STATUSBAR_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SCRIVI_TYPE_STATUSBAR, ScriviStatusbarClass))

typedef struct _ScriviStatusbar		ScriviStatusbar;
typedef struct _ScriviStatusbarPrivate	ScriviStatusbarPrivate;
typedef struct _ScriviStatusbarClass	ScriviStatusbarClass;

struct _ScriviStatusbar
{
        GtkStatusbar parent;

	/* <private/> */
        ScriviStatusbarPrivate *priv;
};

struct _ScriviStatusbarClass
{
        GtkStatusbarClass parent_class;
};

GType		 scrivi_statusbar_get_type		(void) G_GNUC_CONST;

GtkWidget	*scrivi_statusbar_new			(void);

/* FIXME: status is not defined in any .h */
#define ScriviStatus gint
void		 scrivi_statusbar_set_window_state	(ScriviStatusbar   *statusbar,
							 ScriviWindowState  state,
							 gint              num_of_errors);

void		 scrivi_statusbar_set_overwrite		(ScriviStatusbar   *statusbar,
							 gboolean          overwrite);

void		 scrivi_statusbar_set_cursor_position	(ScriviStatusbar   *statusbar,
							 gint              line,
							 gint              col);

void		 scrivi_statusbar_clear_overwrite 	(ScriviStatusbar   *statusbar);

void		 scrivi_statusbar_flash_message		(ScriviStatusbar   *statusbar,
							 guint             context_id,
							 const gchar      *format,
							 ...) G_GNUC_PRINTF(3, 4);

G_END_DECLS

#endif
