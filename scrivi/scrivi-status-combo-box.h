/*
 * scrivi-status-combo-box.h
 * This file is part of scrivi
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
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

#ifndef __SCRIVI_STATUS_COMBO_BOX_H__
#define __SCRIVI_STATUS_COMBO_BOX_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_STATUS_COMBO_BOX		(scrivi_status_combo_box_get_type ())
#define SCRIVI_STATUS_COMBO_BOX(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_STATUS_COMBO_BOX, ScriviStatusComboBox))
#define SCRIVI_STATUS_COMBO_BOX_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_STATUS_COMBO_BOX, ScriviStatusComboBox const))
#define SCRIVI_STATUS_COMBO_BOX_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_STATUS_COMBO_BOX, ScriviStatusComboBoxClass))
#define SCRIVI_IS_STATUS_COMBO_BOX(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_STATUS_COMBO_BOX))
#define SCRIVI_IS_STATUS_COMBO_BOX_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_STATUS_COMBO_BOX))
#define SCRIVI_STATUS_COMBO_BOX_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_STATUS_COMBO_BOX, ScriviStatusComboBoxClass))

typedef struct _ScriviStatusComboBox		ScriviStatusComboBox;
typedef struct _ScriviStatusComboBoxClass	ScriviStatusComboBoxClass;
typedef struct _ScriviStatusComboBoxPrivate	ScriviStatusComboBoxPrivate;

struct _ScriviStatusComboBox {
	GtkEventBox parent;
	
	ScriviStatusComboBoxPrivate *priv;
};

struct _ScriviStatusComboBoxClass {
	GtkEventBoxClass parent_class;
	
	void (*changed) (ScriviStatusComboBox *combo,
			 GtkMenuItem         *item);
};

GType scrivi_status_combo_box_get_type 			(void) G_GNUC_CONST;
GtkWidget *scrivi_status_combo_box_new			(const gchar 		*label);

const gchar *scrivi_status_combo_box_get_label 		(ScriviStatusComboBox 	*combo);
void scrivi_status_combo_box_set_label 			(ScriviStatusComboBox 	*combo,
							 const gchar         	*label);

void scrivi_status_combo_box_add_item 			(ScriviStatusComboBox 	*combo,
							 GtkMenuItem         	*item,
							 const gchar         	*text);
void scrivi_status_combo_box_remove_item			(ScriviStatusComboBox    *combo,
							 GtkMenuItem            *item);

GList *scrivi_status_combo_box_get_items			(ScriviStatusComboBox    *combo);
const gchar *scrivi_status_combo_box_get_item_text 	(ScriviStatusComboBox	*combo,
							 GtkMenuItem		*item);
void scrivi_status_combo_box_set_item_text 		(ScriviStatusComboBox	*combo,
							 GtkMenuItem		*item,
							 const gchar            *text);

void scrivi_status_combo_box_set_item			(ScriviStatusComboBox	*combo,
							 GtkMenuItem		*item);

GtkLabel *scrivi_status_combo_box_get_item_label		(ScriviStatusComboBox	*combo);

G_END_DECLS

#endif /* __SCRIVI_STATUS_COMBO_BOX_H__ */
