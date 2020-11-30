/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * scrivi-preferences-dialog.c
 * This file is part of scrivi
 *
 * Copyright (C) 2001-2005 Paolo Maggi 
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
 * Modified by the scrivi Team, 2003. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id$
 */

#ifndef __SCRIVI_PREFERENCES_DIALOG_H__
#define __SCRIVI_PREFERENCES_DIALOG_H__

#include "scrivi-window.h"

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_PREFERENCES_DIALOG              (scrivi_preferences_dialog_get_type())
#define SCRIVI_PREFERENCES_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_PREFERENCES_DIALOG, ScriviPreferencesDialog))
#define SCRIVI_PREFERENCES_DIALOG_CONST(obj)        (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_PREFERENCES_DIALOG, ScriviPreferencesDialog const))
#define SCRIVI_PREFERENCES_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_PREFERENCES_DIALOG, ScriviPreferencesDialogClass))
#define SCRIVI_IS_PREFERENCES_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_PREFERENCES_DIALOG))
#define SCRIVI_IS_PREFERENCES_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PREFERENCES_DIALOG))
#define SCRIVI_PREFERENCES_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_PREFERENCES_DIALOG, ScriviPreferencesDialogClass))


/* Private structure type */
typedef struct _ScriviPreferencesDialogPrivate ScriviPreferencesDialogPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviPreferencesDialog ScriviPreferencesDialog;

struct _ScriviPreferencesDialog 
{
	GtkDialog dialog;
	
	/*< private > */
	ScriviPreferencesDialogPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviPreferencesDialogClass ScriviPreferencesDialogClass;

struct _ScriviPreferencesDialogClass 
{
	GtkDialogClass parent_class;
};

/*
 * Public methods
 */
GType		 scrivi_preferences_dialog_get_type	(void) G_GNUC_CONST;

void		 scrivi_show_preferences_dialog		(ScriviWindow *parent);

G_END_DECLS

#endif /* __SCRIVI_PREFERENCES_DIALOG_H__ */

