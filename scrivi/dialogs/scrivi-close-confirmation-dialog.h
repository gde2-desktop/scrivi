/*
 * scrivi-close-confirmation-dialog.h
 * This file is part of scrivi
 *
 * Copyright (C) 2004-2005 GNOME Foundation
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
 * Modified by the scrivi Team, 2004-2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 */

#ifndef __SCRIVI_CLOSE_CONFIRMATION_DIALOG_H__
#define __SCRIVI_CLOSE_CONFIRMATION_DIALOG_H__

#include <glib.h>
#include <gtk/gtk.h>

#include <scrivi/scrivi-document.h>

#define SCRIVI_TYPE_CLOSE_CONFIRMATION_DIALOG		(scrivi_close_confirmation_dialog_get_type ())
#define SCRIVI_CLOSE_CONFIRMATION_DIALOG(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_CLOSE_CONFIRMATION_DIALOG, ScriviCloseConfirmationDialog))
#define SCRIVI_CLOSE_CONFIRMATION_DIALOG_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_CLOSE_CONFIRMATION_DIALOG, ScriviCloseConfirmationDialogClass))
#define SCRIVI_IS_CLOSE_CONFIRMATION_DIALOG(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_CLOSE_CONFIRMATION_DIALOG))
#define SCRIVI_IS_CLOSE_CONFIRMATION_DIALOG_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_CLOSE_CONFIRMATION_DIALOG))
#define SCRIVI_CLOSE_CONFIRMATION_DIALOG_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj),SCRIVI_TYPE_CLOSE_CONFIRMATION_DIALOG, ScriviCloseConfirmationDialogClass))

typedef struct _ScriviCloseConfirmationDialog 		ScriviCloseConfirmationDialog;
typedef struct _ScriviCloseConfirmationDialogClass 	ScriviCloseConfirmationDialogClass;
typedef struct _ScriviCloseConfirmationDialogPrivate 	ScriviCloseConfirmationDialogPrivate;

struct _ScriviCloseConfirmationDialog 
{
	GtkDialog parent;

	/*< private > */
	ScriviCloseConfirmationDialogPrivate *priv;
};

struct _ScriviCloseConfirmationDialogClass 
{
	GtkDialogClass parent_class;
};

GType 		 scrivi_close_confirmation_dialog_get_type		(void) G_GNUC_CONST;

GtkWidget	*scrivi_close_confirmation_dialog_new			(GtkWindow     *parent,
									 GList         *unsaved_documents,
									 gboolean       logout_mode);
GtkWidget 	*scrivi_close_confirmation_dialog_new_single 		(GtkWindow     *parent, 
									 ScriviDocument *doc,
 									 gboolean       logout_mode);

const GList	*scrivi_close_confirmation_dialog_get_unsaved_documents  (ScriviCloseConfirmationDialog *dlg);

GList		*scrivi_close_confirmation_dialog_get_selected_documents	(ScriviCloseConfirmationDialog *dlg);

#endif /* __SCRIVI_CLOSE_CONFIRMATION_DIALOG_H__ */

