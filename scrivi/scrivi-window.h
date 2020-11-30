/*
 * scrivi-window.h
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
 * MERCHANWINDOWILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
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

#ifndef __SCRIVI_WINDOW_H__
#define __SCRIVI_WINDOW_H__

#include <gio/gio.h>
#include <gtk/gtk.h>

#include <scrivi/scrivi-tab.h>
#include <scrivi/scrivi-panel.h>
#include <scrivi/scrivi-message-bus.h>

G_BEGIN_DECLS

typedef enum
{
	SCRIVI_WINDOW_STATE_NORMAL		= 0,
	SCRIVI_WINDOW_STATE_SAVING		= 1 << 1,
	SCRIVI_WINDOW_STATE_PRINTING		= 1 << 2,
	SCRIVI_WINDOW_STATE_LOADING		= 1 << 3,
	SCRIVI_WINDOW_STATE_ERROR		= 1 << 4,
	SCRIVI_WINDOW_STATE_SAVING_SESSION	= 1 << 5
} ScriviWindowState;
	
/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_WINDOW              (scrivi_window_get_type())
#define SCRIVI_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_WINDOW, ScriviWindow))
#define SCRIVI_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_WINDOW, ScriviWindowClass))
#define SCRIVI_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_WINDOW))
#define SCRIVI_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_WINDOW))
#define SCRIVI_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_WINDOW, ScriviWindowClass))

/* Private structure type */
typedef struct _ScriviWindowPrivate ScriviWindowPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviWindow ScriviWindow;

struct _ScriviWindow 
{
	GtkWindow window;

	/*< private > */
	ScriviWindowPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviWindowClass ScriviWindowClass;

struct _ScriviWindowClass 
{
	GtkWindowClass parent_class;
	
	/* Signals */
	void	 (* tab_added)      	(ScriviWindow *window,
				     	 ScriviTab    *tab);
	void	 (* tab_removed)    	(ScriviWindow *window,
				     	 ScriviTab    *tab);
	void	 (* tabs_reordered) 	(ScriviWindow *window);
	void	 (* active_tab_changed)	(ScriviWindow *window,
				     	 ScriviTab    *tab);
	void	 (* active_tab_state_changed)	
					(ScriviWindow *window);
};

/*
 * Public methods
 */
GType 		 scrivi_window_get_type 			(void) G_GNUC_CONST;

ScriviTab	*scrivi_window_create_tab		(ScriviWindow         *window,
							 gboolean             jump_to);
							 
ScriviTab	*scrivi_window_create_tab_from_uri	(ScriviWindow         *window,
							 const gchar         *uri,
							 const ScriviEncoding *encoding,
							 gint                 line_pos,
							 gboolean             create,
							 gboolean             jump_to);
							 
void		 scrivi_window_close_tab			(ScriviWindow         *window,
							 ScriviTab            *tab);
							 
void		 scrivi_window_close_all_tabs		(ScriviWindow         *window);

void		 scrivi_window_close_tabs		(ScriviWindow         *window,
							 const GList         *tabs);
							 
ScriviTab	*scrivi_window_get_active_tab		(ScriviWindow         *window);

void		 scrivi_window_set_active_tab		(ScriviWindow         *window,
							 ScriviTab            *tab);

/* Helper functions */
ScriviView	*scrivi_window_get_active_view		(ScriviWindow         *window);
ScriviDocument	*scrivi_window_get_active_document	(ScriviWindow         *window);

/* Returns a newly allocated list with all the documents in the window */
GList		*scrivi_window_get_documents		(ScriviWindow         *window);

/* Returns a newly allocated list with all the documents that need to be 
   saved before closing the window */
GList		*scrivi_window_get_unsaved_documents 	(ScriviWindow         *window);

/* Returns a newly allocated list with all the views in the window */
GList		*scrivi_window_get_views			(ScriviWindow         *window);

GtkWindowGroup  *scrivi_window_get_group			(ScriviWindow         *window);

ScriviPanel	*scrivi_window_get_side_panel		(ScriviWindow         *window);

ScriviPanel	*scrivi_window_get_bottom_panel		(ScriviWindow         *window);

GtkWidget	*scrivi_window_get_statusbar		(ScriviWindow         *window);

GtkUIManager	*scrivi_window_get_ui_manager		(ScriviWindow         *window);

ScriviWindowState scrivi_window_get_state 		(ScriviWindow         *window);

ScriviTab        *scrivi_window_get_tab_from_location	(ScriviWindow         *window,
							 GFile               *location);

ScriviTab        *scrivi_window_get_tab_from_uri		(ScriviWindow         *window,
							 const gchar         *uri);

/* Message bus */
ScriviMessageBus	*scrivi_window_get_message_bus		(ScriviWindow         *window);

/*
 * Non exported functions
 */
GtkWidget	*_scrivi_window_get_notebook		(ScriviWindow         *window);

ScriviWindow	*_scrivi_window_move_tab_to_new_window	(ScriviWindow         *window,
							 ScriviTab            *tab);
gboolean	 _scrivi_window_is_removing_tabs		(ScriviWindow         *window);

GFile		*_scrivi_window_get_default_location 	(ScriviWindow         *window);

void		 _scrivi_window_set_default_location 	(ScriviWindow         *window,
							 GFile               *location);

void		 _scrivi_window_set_saving_session_state	(ScriviWindow         *window,
							 gboolean             saving_session);

void		 _scrivi_window_fullscreen		(ScriviWindow         *window);

void		 _scrivi_window_unfullscreen		(ScriviWindow         *window);

gboolean	 _scrivi_window_is_fullscreen		(ScriviWindow         *window);

/* these are in scrivi-window because of screen safety */
void		 _scrivi_recent_add			(ScriviWindow	     *window,
							 const gchar         *uri,
							 const gchar         *mime);
void		 _scrivi_recent_remove			(ScriviWindow         *window,
							 const gchar         *uri);

G_END_DECLS

#endif  /* __SCRIVI_WINDOW_H__  */
