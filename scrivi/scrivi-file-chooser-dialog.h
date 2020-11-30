/*
 * scrivi-file-chooser-dialog.h
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

#ifndef __SCRIVI_FILE_CHOOSER_DIALOG_H__
#define __SCRIVI_FILE_CHOOSER_DIALOG_H__

#include <gtk/gtk.h>

#include <scrivi/scrivi-encodings.h>
#include <scrivi/scrivi-enum-types.h>
#include <scrivi/scrivi-document.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_FILE_CHOOSER_DIALOG             (scrivi_file_chooser_dialog_get_type ())
#define SCRIVI_FILE_CHOOSER_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_FILE_CHOOSER_DIALOG, ScriviFileChooserDialog))
#define SCRIVI_FILE_CHOOSER_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_FILE_CHOOSER_DIALOG, ScriviFileChooserDialogClass))
#define SCRIVI_IS_FILE_CHOOSER_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_FILE_CHOOSER_DIALOG))
#define SCRIVI_IS_FILE_CHOOSER_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_FILE_CHOOSER_DIALOG))
#define SCRIVI_FILE_CHOOSER_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_FILE_CHOOSER_DIALOG, ScriviFileChooserDialogClass))

typedef struct _ScriviFileChooserDialog      ScriviFileChooserDialog;
typedef struct _ScriviFileChooserDialogClass ScriviFileChooserDialogClass;

typedef struct _ScriviFileChooserDialogPrivate ScriviFileChooserDialogPrivate;

struct _ScriviFileChooserDialogClass
{
	GtkFileChooserDialogClass parent_class;
};

struct _ScriviFileChooserDialog
{
	GtkFileChooserDialog parent_instance;

	ScriviFileChooserDialogPrivate *priv;
};

GType		 scrivi_file_chooser_dialog_get_type	(void) G_GNUC_CONST;

GtkWidget	*scrivi_file_chooser_dialog_new		(const gchar            *title,
							 GtkWindow              *parent,
							 GtkFileChooserAction    action,
							 const ScriviEncoding    *encoding,
							 const gchar            *first_button_text,
							 ...);

void		 scrivi_file_chooser_dialog_set_encoding (ScriviFileChooserDialog *dialog,
							 const ScriviEncoding    *encoding);

const ScriviEncoding
		*scrivi_file_chooser_dialog_get_encoding (ScriviFileChooserDialog *dialog);

void		 scrivi_file_chooser_dialog_set_newline_type (ScriviFileChooserDialog  *dialog,
							     ScriviDocumentNewlineType newline_type);

ScriviDocumentNewlineType
		 scrivi_file_chooser_dialog_get_newline_type (ScriviFileChooserDialog *dialog);

G_END_DECLS

#endif /* __SCRIVI_FILE_CHOOSER_DIALOG_H__ */
