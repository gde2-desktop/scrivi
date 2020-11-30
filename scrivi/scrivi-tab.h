/*
 * scrivi-tab.h
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

#ifndef __SCRIVI_TAB_H__
#define __SCRIVI_TAB_H__

#include <gtk/gtk.h>

#include <scrivi/scrivi-view.h>
#include <scrivi/scrivi-document.h>

G_BEGIN_DECLS

typedef enum
{
	SCRIVI_TAB_STATE_NORMAL = 0,
	SCRIVI_TAB_STATE_LOADING,
	SCRIVI_TAB_STATE_REVERTING,
	SCRIVI_TAB_STATE_SAVING,	
	SCRIVI_TAB_STATE_PRINTING,
	SCRIVI_TAB_STATE_PRINT_PREVIEWING,
	SCRIVI_TAB_STATE_SHOWING_PRINT_PREVIEW,
	SCRIVI_TAB_STATE_GENERIC_NOT_EDITABLE,
	SCRIVI_TAB_STATE_LOADING_ERROR,
	SCRIVI_TAB_STATE_REVERTING_ERROR,	
	SCRIVI_TAB_STATE_SAVING_ERROR,
	SCRIVI_TAB_STATE_GENERIC_ERROR,
	SCRIVI_TAB_STATE_CLOSING,
	SCRIVI_TAB_STATE_EXTERNALLY_MODIFIED_NOTIFICATION,
	SCRIVI_TAB_NUM_OF_STATES /* This is not a valid state */
} ScriviTabState;

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_TAB              (scrivi_tab_get_type())
#define SCRIVI_TAB(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_TAB, ScriviTab))
#define SCRIVI_TAB_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_TAB, ScriviTabClass))
#define SCRIVI_IS_TAB(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_TAB))
#define SCRIVI_IS_TAB_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_TAB))
#define SCRIVI_TAB_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_TAB, ScriviTabClass))

/* Private structure type */
typedef struct _ScriviTabPrivate ScriviTabPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviTab ScriviTab;

struct _ScriviTab 
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBox vbox;
#else
	GtkVBox vbox;
#endif

	/*< private > */
	ScriviTabPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviTabClass ScriviTabClass;

struct _ScriviTabClass 
{
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBoxClass parent_class;
#else
	GtkVBoxClass parent_class;
#endif
};

/*
 * Public methods
 */
GType 		 scrivi_tab_get_type 		(void) G_GNUC_CONST;

ScriviView	*scrivi_tab_get_view		(ScriviTab            *tab);

/* This is only an helper function */
ScriviDocument	*scrivi_tab_get_document		(ScriviTab            *tab);

ScriviTab	*scrivi_tab_get_from_document	(ScriviDocument       *doc);

ScriviTabState	 scrivi_tab_get_state		(ScriviTab	     *tab);

gboolean	 scrivi_tab_get_auto_save_enabled	
						(ScriviTab            *tab); 

void		 scrivi_tab_set_auto_save_enabled	
						(ScriviTab            *tab, 
						 gboolean            enable);

gint		 scrivi_tab_get_auto_save_interval 
						(ScriviTab            *tab);

void		 scrivi_tab_set_auto_save_interval 
						(ScriviTab            *tab, 
						 gint                interval);

void		 scrivi_tab_set_info_bar		(ScriviTab            *tab,
						 GtkWidget           *info_bar);
/*
 * Non exported methods
 */
GtkWidget 	*_scrivi_tab_new 		(void);

/* Whether create is TRUE, creates a new empty document if location does 
   not refer to an existing file */
GtkWidget	*_scrivi_tab_new_from_uri	(const gchar         *uri,
						 const ScriviEncoding *encoding,
						 gint                 line_pos,
						 gboolean             create);
gchar 		*_scrivi_tab_get_name		(ScriviTab            *tab);
gchar 		*_scrivi_tab_get_tooltips	(ScriviTab            *tab);
GdkPixbuf 	*_scrivi_tab_get_icon		(ScriviTab            *tab);
void		 _scrivi_tab_load		(ScriviTab            *tab,
						 const gchar         *uri,
						 const ScriviEncoding *encoding,
						 gint                 line_pos,
						 gboolean             create);
void		 _scrivi_tab_revert		(ScriviTab            *tab);
void		 _scrivi_tab_save		(ScriviTab            *tab);
void		 _scrivi_tab_save_as		(ScriviTab            *tab,
						 const gchar         *uri,
						 const ScriviEncoding *encoding,
						 ScriviDocumentNewlineType newline_type);

void		 _scrivi_tab_print		(ScriviTab            *tab);
void		 _scrivi_tab_print_preview	(ScriviTab            *tab);

void		 _scrivi_tab_mark_for_closing	(ScriviTab	     *tab);

gboolean	 _scrivi_tab_can_close		(ScriviTab	     *tab);

G_END_DECLS

#endif  /* __SCRIVI_TAB_H__  */
