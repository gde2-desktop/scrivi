/*
 * scrivi-notebook.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Maggi 
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

/* This file is a modified version of the epiphany file ephy-notebook.h
 * Here the relevant copyright:
 *
 *  Copyright (C) 2002 Christophe Fergeau
 *  Copyright (C) 2003 Marco Pesenti Gritti
 *  Copyright (C) 2003, 2004 Christian Persch
 *
 */
 
#ifndef SCRIVI_NOTEBOOK_H
#define SCRIVI_NOTEBOOK_H

#include <scrivi/scrivi-tab.h>

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_NOTEBOOK		(scrivi_notebook_get_type ())
#define SCRIVI_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCRIVI_TYPE_NOTEBOOK, ScriviNotebook))
#define SCRIVI_NOTEBOOK_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), SCRIVI_TYPE_NOTEBOOK, ScriviNotebookClass))
#define SCRIVI_IS_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCRIVI_TYPE_NOTEBOOK))
#define SCRIVI_IS_NOTEBOOK_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCRIVI_TYPE_NOTEBOOK))
#define SCRIVI_NOTEBOOK_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SCRIVI_TYPE_NOTEBOOK, ScriviNotebookClass))

/* Private structure type */
typedef struct _ScriviNotebookPrivate	ScriviNotebookPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviNotebook		ScriviNotebook;
 
struct _ScriviNotebook
{
	GtkNotebook notebook;

	/*< private >*/
        ScriviNotebookPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviNotebookClass	ScriviNotebookClass;

struct _ScriviNotebookClass
{
        GtkNotebookClass parent_class;

	/* Signals */
	void	 (* tab_added)      (ScriviNotebook *notebook,
				     ScriviTab      *tab);
	void	 (* tab_removed)    (ScriviNotebook *notebook,
				     ScriviTab      *tab);
	void	 (* tab_detached)   (ScriviNotebook *notebook,
				     ScriviTab      *tab);
	void	 (* tabs_reordered) (ScriviNotebook *notebook);
	void	 (* tab_close_request)
				    (ScriviNotebook *notebook,
				     ScriviTab      *tab);
};

/*
 * Public methods
 */
GType		scrivi_notebook_get_type		(void) G_GNUC_CONST;

GtkWidget      *scrivi_notebook_new		(void);

void		scrivi_notebook_add_tab		(ScriviNotebook *nb,
						 ScriviTab      *tab,
						 gint           position,
						 gboolean       jump_to);

void		scrivi_notebook_remove_tab	(ScriviNotebook *nb,
						 ScriviTab      *tab);

void		scrivi_notebook_remove_all_tabs 	(ScriviNotebook *nb);

void		scrivi_notebook_reorder_tab	(ScriviNotebook *src,
			    			 ScriviTab      *tab,
			    			 gint           dest_position);
			    			 
void            scrivi_notebook_move_tab		(ScriviNotebook *src,
						 ScriviNotebook *dest,
						 ScriviTab      *tab,
						 gint           dest_position);

/* FIXME: do we really need this function ? */
void		scrivi_notebook_set_always_show_tabs	
						(ScriviNotebook *nb,
						 gboolean       show_tabs);

void		scrivi_notebook_set_close_buttons_sensitive
						(ScriviNotebook *nb,
						 gboolean       sensitive);

gboolean	scrivi_notebook_get_close_buttons_sensitive
						(ScriviNotebook *nb);

void		scrivi_notebook_set_tab_drag_and_drop_enabled
						(ScriviNotebook *nb,
						 gboolean       enable);

gboolean	scrivi_notebook_get_tab_drag_and_drop_enabled
						(ScriviNotebook *nb);

G_END_DECLS

#endif /* SCRIVI_NOTEBOOK_H */
