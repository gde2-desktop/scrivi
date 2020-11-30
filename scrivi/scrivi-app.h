/*
 * scrivi-app.h
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

#ifndef __SCRIVI_APP_H__
#define __SCRIVI_APP_H__

#include <gtk/gtk.h>

#include <scrivi/scrivi-window.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_APP              (scrivi_app_get_type())
#define SCRIVI_APP(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_APP, ScriviApp))
#define SCRIVI_APP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_APP, ScriviAppClass))
#define SCRIVI_IS_APP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_APP))
#define SCRIVI_IS_APP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_APP))
#define SCRIVI_APP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_APP, ScriviAppClass))

/* Private structure type */
typedef struct _ScriviAppPrivate ScriviAppPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviApp ScriviApp;

struct _ScriviApp 
{
	GObject object;

	/*< private > */
	ScriviAppPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviAppClass ScriviAppClass;

struct _ScriviAppClass 
{
	GObjectClass parent_class;
};

/*
 * Lockdown mask definition
 */
typedef enum
{
	SCRIVI_LOCKDOWN_COMMAND_LINE	= 1 << 0,
	SCRIVI_LOCKDOWN_PRINTING		= 1 << 1,
	SCRIVI_LOCKDOWN_PRINT_SETUP	= 1 << 2,
	SCRIVI_LOCKDOWN_SAVE_TO_DISK	= 1 << 3,
	SCRIVI_LOCKDOWN_ALL		= 0xF
} ScriviLockdownMask;

/*
 * Public methods
 */
GType 		 scrivi_app_get_type 			(void) G_GNUC_CONST;

ScriviApp 	*scrivi_app_get_default			(void);

ScriviWindow	*scrivi_app_create_window		(ScriviApp  *app,
							 GdkScreen *screen);

const GList	*scrivi_app_get_windows			(ScriviApp *app);
ScriviWindow	*scrivi_app_get_active_window		(ScriviApp *app);

/* Returns a newly allocated list with all the documents */
GList		*scrivi_app_get_documents		(ScriviApp *app);

/* Returns a newly allocated list with all the views */
GList		*scrivi_app_get_views			(ScriviApp *app);

/* Lockdown state */
ScriviLockdownMask scrivi_app_get_lockdown		(ScriviApp *app);

/*
 * Non exported functions
 */
ScriviWindow	*_scrivi_app_restore_window		(ScriviApp    *app,
							 const gchar *role);
ScriviWindow	*_scrivi_app_get_window_in_viewport	(ScriviApp     *app,
							 GdkScreen    *screen,
							 gint          workspace,
							 gint          viewport_x,
							 gint          viewport_y);
void		 _scrivi_app_set_lockdown		(ScriviApp          *app,
							 ScriviLockdownMask  lockdown);
void		 _scrivi_app_set_lockdown_bit		(ScriviApp          *app,
							 ScriviLockdownMask  bit,
							 gboolean           value);
/*
 * This one is a scrivi-window function, but we declare it here to avoid
 * #include headaches since it needs the ScriviLockdownMask declaration.
 */
void		 _scrivi_window_set_lockdown		(ScriviWindow         *window,
							 ScriviLockdownMask    lockdown);

/* global print config */
GtkPageSetup		*_scrivi_app_get_default_page_setup	(ScriviApp         *app);
void			 _scrivi_app_set_default_page_setup	(ScriviApp         *app,
								 GtkPageSetup     *page_setup);
GtkPrintSettings	*_scrivi_app_get_default_print_settings	(ScriviApp         *app);
void			 _scrivi_app_set_default_print_settings	(ScriviApp         *app,
								 GtkPrintSettings *settings);

G_END_DECLS

#endif  /* __SCRIVI_APP_H__  */
