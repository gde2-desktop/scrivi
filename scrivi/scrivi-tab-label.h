/*
 * scrivi-tab-label.h
 * This file is part of scrivi
 *
 * Copyright (C) 2010 - Paolo Borelli
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

#ifndef __SCRIVI_TAB_LABEL_H__
#define __SCRIVI_TAB_LABEL_H__

#include <gtk/gtk.h>
#include <scrivi/scrivi-tab.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_TAB_LABEL		(scrivi_tab_label_get_type ())
#define SCRIVI_TAB_LABEL(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_TAB_LABEL, ScriviTabLabel))
#define SCRIVI_TAB_LABEL_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_TAB_LABEL, ScriviTabLabel const))
#define SCRIVI_TAB_LABEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_TAB_LABEL, ScriviTabLabelClass))
#define SCRIVI_IS_TAB_LABEL(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_TAB_LABEL))
#define SCRIVI_IS_TAB_LABEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_TAB_LABEL))
#define SCRIVI_TAB_LABEL_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_TAB_LABEL, ScriviTabLabelClass))

typedef struct _ScriviTabLabel		ScriviTabLabel;
typedef struct _ScriviTabLabelClass	ScriviTabLabelClass;
typedef struct _ScriviTabLabelPrivate	ScriviTabLabelPrivate;

struct _ScriviTabLabel {
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBox parent;
#else
	GtkHBox parent;
#endif
	
	ScriviTabLabelPrivate *priv;
};

struct _ScriviTabLabelClass {
#if GTK_CHECK_VERSION (3, 0, 0)
	GtkBoxClass parent_class;
#else
	GtkHBoxClass parent_class;
#endif

	void (* close_clicked)  (ScriviTabLabel *tab_label);
};

GType		 scrivi_tab_label_get_type (void) G_GNUC_CONST;

GtkWidget 	*scrivi_tab_label_new (ScriviTab *tab);

ScriviTab	*scrivi_tab_label_get_tab (ScriviTabLabel *tab_label);

void		scrivi_tab_label_set_close_button_sensitive (ScriviTabLabel *tab_label,
							    gboolean       sensitive);

G_END_DECLS

#endif /* __SCRIVI_TAB_LABEL_H__ */
