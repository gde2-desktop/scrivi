/*
 * scrivi-search-dialog.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 Paolo Maggi
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

#ifndef __SCRIVI_SEARCH_DIALOG_H__
#define __SCRIVI_SEARCH_DIALOG_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_SEARCH_DIALOG              (scrivi_search_dialog_get_type())
#define SCRIVI_SEARCH_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_SEARCH_DIALOG, ScriviSearchDialog))
#define SCRIVI_SEARCH_DIALOG_CONST(obj)        (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_SEARCH_DIALOG, ScriviSearchDialog const))
#define SCRIVI_SEARCH_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_SEARCH_DIALOG, ScriviSearchDialogClass))
#define SCRIVI_IS_SEARCH_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_SEARCH_DIALOG))
#define SCRIVI_IS_SEARCH_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_SEARCH_DIALOG))
#define SCRIVI_SEARCH_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_SEARCH_DIALOG, ScriviSearchDialogClass))

/* Private structure type */
typedef struct _ScriviSearchDialogPrivate ScriviSearchDialogPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviSearchDialog ScriviSearchDialog;

struct _ScriviSearchDialog 
{
	GtkDialog dialog;

	/*< private > */
	ScriviSearchDialogPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviSearchDialogClass ScriviSearchDialogClass;

struct _ScriviSearchDialogClass 
{
	GtkDialogClass parent_class;
	
	/* Key bindings */
	gboolean (* show_replace) (ScriviSearchDialog *dlg);
};

enum
{
	SCRIVI_SEARCH_DIALOG_FIND_RESPONSE = 100,
	SCRIVI_SEARCH_DIALOG_REPLACE_RESPONSE,
	SCRIVI_SEARCH_DIALOG_REPLACE_ALL_RESPONSE
};

/*
 * Public methods
 */
GType 		 scrivi_search_dialog_get_type 		(void) G_GNUC_CONST;

GtkWidget	*scrivi_search_dialog_new		(GtkWindow         *parent,
							 gboolean           show_replace);

void		 scrivi_search_dialog_present_with_time	(ScriviSearchDialog *dialog,
							 guint32 timestamp);

gboolean	 scrivi_search_dialog_get_show_replace	(ScriviSearchDialog *dialog);

void		 scrivi_search_dialog_set_show_replace	(ScriviSearchDialog *dialog,
							 gboolean           show_replace);


void		 scrivi_search_dialog_set_search_text	(ScriviSearchDialog *dialog,
							 const gchar       *text);
const gchar	*scrivi_search_dialog_get_search_text	(ScriviSearchDialog *dialog);

void		 scrivi_search_dialog_set_replace_text	(ScriviSearchDialog *dialog,
							 const gchar       *text);
const gchar	*scrivi_search_dialog_get_replace_text	(ScriviSearchDialog *dialog);

void		 scrivi_search_dialog_set_match_case	(ScriviSearchDialog *dialog,
							 gboolean           match_case);
gboolean	 scrivi_search_dialog_get_match_case	(ScriviSearchDialog *dialog);

void		 scrivi_search_dialog_set_entire_word	(ScriviSearchDialog *dialog,
							 gboolean           entire_word);
gboolean	 scrivi_search_dialog_get_entire_word	(ScriviSearchDialog *dialog);

void		 scrivi_search_dialog_set_backwards	(ScriviSearchDialog *dialog,
							 gboolean           backwards);
gboolean	 scrivi_search_dialog_get_backwards	(ScriviSearchDialog *dialog);

void		 scrivi_search_dialog_set_wrap_around	(ScriviSearchDialog *dialog,
							 gboolean           wrap_around);
gboolean	 scrivi_search_dialog_get_wrap_around	(ScriviSearchDialog *dialog);
   

void		scrivi_search_dialog_set_parse_escapes (ScriviSearchDialog *dialog,
                                    		       gboolean           parse_escapes);
gboolean	scrivi_search_dialog_get_parse_escapes (ScriviSearchDialog *dialog);

G_END_DECLS

#endif  /* __SCRIVI_SEARCH_DIALOG_H__  */
