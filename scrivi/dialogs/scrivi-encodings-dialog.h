/*
 * scrivi-encodings-dialog.h
 * This file is part of scrivi
 *
 * Copyright (C) 2003-2005 Paolo Maggi 
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
 * Modified by the scrivi Team, 2003-2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id$
 */

#ifndef __SCRIVI_ENCODINGS_DIALOG_H__
#define __SCRIVI_ENCODINGS_DIALOG_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_ENCODINGS_DIALOG              (scrivi_encodings_dialog_get_type())
#define SCRIVI_ENCODINGS_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_ENCODINGS_DIALOG, ScriviEncodingsDialog))
#define SCRIVI_ENCODINGS_DIALOG_CONST(obj)        (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_ENCODINGS_DIALOG, ScriviEncodingsDialog const))
#define SCRIVI_ENCODINGS_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_ENCODINGS_DIALOG, ScriviEncodingsDialogClass))
#define SCRIVI_IS_ENCODINGS_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_ENCODINGS_DIALOG))
#define SCRIVI_IS_ENCODINGS_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_ENCODINGS_DIALOG))
#define SCRIVI_ENCODINGS_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_ENCODINGS_DIALOG, ScriviEncodingsDialogClass))


/* Private structure type */
typedef struct _ScriviEncodingsDialogPrivate ScriviEncodingsDialogPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviEncodingsDialog ScriviEncodingsDialog;

struct _ScriviEncodingsDialog 
{
	GtkDialog dialog;

	/*< private > */
	ScriviEncodingsDialogPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviEncodingsDialogClass ScriviEncodingsDialogClass;

struct _ScriviEncodingsDialogClass 
{
	GtkDialogClass parent_class;
};

/*
 * Public methods
 */
GType		 scrivi_encodings_dialog_get_type	(void) G_GNUC_CONST;

GtkWidget	*scrivi_encodings_dialog_new		(void);

G_END_DECLS

#endif /* __SCRIVI_ENCODINGS_DIALOG_H__ */

