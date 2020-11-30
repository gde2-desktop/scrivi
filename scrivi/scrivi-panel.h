/*
 * scrivi-panel.h
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
 *
 * $Id$
 */

#ifndef __SCRIVI_PANEL_H__
#define __SCRIVI_PANEL_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_PANEL		(scrivi_panel_get_type())
#define SCRIVI_PANEL(obj)		(G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_PANEL, ScriviPanel))
#define SCRIVI_PANEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_PANEL, ScriviPanelClass))
#define SCRIVI_IS_PANEL(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_PANEL))
#define SCRIVI_IS_PANEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PANEL))
#define SCRIVI_PANEL_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_PANEL, ScriviPanelClass))

/* Private structure type */
typedef struct _ScriviPanelPrivate ScriviPanelPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviPanel ScriviPanel;

struct _ScriviPanel 
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBox vbox;
#else
	GtkVBox vbox;
#endif

	/*< private > */
	ScriviPanelPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviPanelClass ScriviPanelClass;

struct _ScriviPanelClass 
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBoxClass parent_class;
#else
	GtkVBoxClass parent_class;
#endif

	void (* item_added)     (ScriviPanel     *panel,
				 GtkWidget      *item);
	void (* item_removed)   (ScriviPanel     *panel,
				 GtkWidget      *item);

	/* Keybinding signals */
	void (* close)          (ScriviPanel     *panel);
	void (* focus_document) (ScriviPanel     *panel);

	/* Padding for future expansion */
	void (*_scrivi_reserved1) (void);
	void (*_scrivi_reserved2) (void);
	void (*_scrivi_reserved3) (void);
	void (*_scrivi_reserved4) (void);	
};

/*
 * Public methods
 */
GType 		 scrivi_panel_get_type 			(void) G_GNUC_CONST;

GtkWidget 	*scrivi_panel_new 			(GtkOrientation	 orientation);

void		 scrivi_panel_add_item			(ScriviPanel     *panel,
						      	 GtkWidget      *item,
						      	 const gchar    *name,
							 GtkWidget      *image);

void		 scrivi_panel_add_item_with_stock_icon	(ScriviPanel     *panel,
							 GtkWidget      *item,
						      	 const gchar    *name,
						      	 const gchar    *stock_id);

gboolean	 scrivi_panel_remove_item	(ScriviPanel     *panel,
					  	 GtkWidget      *item);

gboolean	 scrivi_panel_activate_item 	(ScriviPanel     *panel,
					    	 GtkWidget      *item);

gboolean	 scrivi_panel_item_is_active 	(ScriviPanel     *panel,
					    	 GtkWidget      *item);

GtkOrientation	 scrivi_panel_get_orientation	(ScriviPanel	*panel);

gint		 scrivi_panel_get_n_items	(ScriviPanel	*panel);


/*
 * Non exported functions
 */
gint		 _scrivi_panel_get_active_item_id	(ScriviPanel	*panel);

void		 _scrivi_panel_set_active_item_by_id	(ScriviPanel	*panel,
							 gint		 id);

G_END_DECLS

#endif  /* __SCRIVI_PANEL_H__  */
