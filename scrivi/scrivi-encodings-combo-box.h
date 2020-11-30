/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * scrivi-encodings-combo-box.h
 * This file is part of scrivi
 *
 * Copyright (C) 2003-2005 - Paolo Maggi
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
 * $Id: scrivi-encodings-option-menu.h 4429 2005-12-12 17:28:04Z pborelli $
 */

#ifndef __SCRIVI_ENCODINGS_COMBO_BOX_H__
#define __SCRIVI_ENCODINGS_COMBO_BOX_H__

#include <gtk/gtk.h>
#include <scrivi/scrivi-encodings.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_ENCODINGS_COMBO_BOX             (scrivi_encodings_combo_box_get_type ())
#define SCRIVI_ENCODINGS_COMBO_BOX(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_ENCODINGS_COMBO_BOX, ScriviEncodingsComboBox))
#define SCRIVI_ENCODINGS_COMBO_BOX_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_ENCODINGS_COMBO_BOX, ScriviEncodingsComboBoxClass))
#define SCRIVI_IS_ENCODINGS_COMBO_BOX(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_ENCODINGS_COMBO_BOX))
#define SCRIVI_IS_ENCODINGS_COMBO_BOX_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_ENCODINGS_COMBO_BOX))
#define SCRIVI_ENCODINGS_COMBO_BOX_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_ENCODINGS_COMBO_BOX, ScriviEncodingsComboBoxClass))


typedef struct _ScriviEncodingsComboBox 	ScriviEncodingsComboBox;
typedef struct _ScriviEncodingsComboBoxClass 	ScriviEncodingsComboBoxClass;

typedef struct _ScriviEncodingsComboBoxPrivate	ScriviEncodingsComboBoxPrivate;

struct _ScriviEncodingsComboBox
{
	GtkComboBox			 parent;

	ScriviEncodingsComboBoxPrivate	*priv;
};

struct _ScriviEncodingsComboBoxClass
{
	GtkComboBoxClass		 parent_class;
};

GType		     scrivi_encodings_combo_box_get_type		(void) G_GNUC_CONST;

/* Constructor */
GtkWidget 	    *scrivi_encodings_combo_box_new 			(gboolean save_mode);

const ScriviEncoding *scrivi_encodings_combo_box_get_selected_encoding	(ScriviEncodingsComboBox *menu);
void		     scrivi_encodings_combo_box_set_selected_encoding	(ScriviEncodingsComboBox *menu,
									 const ScriviEncoding      *encoding);

G_END_DECLS

#endif /* __SCRIVI_ENCODINGS_COMBO_BOX_H__ */


